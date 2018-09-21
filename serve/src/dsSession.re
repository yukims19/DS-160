module OP = Onepostsql;
open Async;
open Common;
let defer = Deferred.return;

type session = {
  id: Uuidm.t,
  userId: option(Uuidm.t),
  expireAt: option(Ptime.t),
  createdAt: option(Ptime.t),
  updatedAt: option(Ptime.t),
};

let ofDbResult = tuple =>
  switch (tuple) {
  | [|id, userId, expireAt, createdAt, updatedAt|] =>
    let id =
      switch (Uuidm.of_string(id)) {
      | None =>
        failPublic(
          ~internal="Invalid uuid from db in oneUser (id): " ++ id,
          ~public="We hit an internal error",
          (),
        )
      | Some(uuid) => uuid
      };

    let userId =
      if (String.length(userId) > 0) {
        switch (Uuidm.of_string(userId)) {
        | None =>
          failPublic(
            ~internal="Invalid uuid from db in dsUser (id): " ++ userId,
            ~public="We hit an internal error",
            (),
          )
        | Some(uuid) => Some(uuid)
        };
      } else {
        None;
      };
    let expireAt = Some(OP.ptimeOfDbTs(expireAt));
    let createdAt = Some(OP.ptimeOfDbTs(createdAt));
    let updatedAt = Some(OP.ptimeOfDbTs(updatedAt));
    {id, userId, expireAt, createdAt, updatedAt};
  | resFields =>
    failPublic(
      ~internal=
        Printf.sprintf(
          "Invalid DB tuple shape for dsUser: fields=%s",
          String.concat(", ", Array.to_list(resFields)),
        ),
      ~public="We hit an internal error",
      (),
    )
  };

let setUserIdBySessionId =
    (conn: OneDb.connPool, sessionId, userId)
    : Deferred.t(result(string, string)) => {
  let params = [|Uuidm.to_string(userId), Uuidm.to_string(sessionId)|];
  OP.sendPrepared(conn, ~params, ~name="ds_session_set_userid", ())
  >>= (
    res => {
      let status = res#status;
      Postgresql.(
        switch (status) {
        | Tuples_ok =>
          Deferred.return(
            Result.Ok("Successfully updated userId in session"),
          )
        | _ =>
          Deferred.return(
            Result.Error(
              "Some error when setting userId in session table I guess...",
            ),
          )
        }
      );
    }
  );
};
let generateNewSession = () => {
  let time = Unix.gettimeofday();
  let newTimeNow = Ptime.of_float_s(time);
  let newSession = {
    id: Uuidm.ns_X500,
    userId: None,
    expireAt: Some(Ptime.max),
    createdAt: newTimeNow,
    updatedAt: newTimeNow,
  };
  newSession;
};

let storeAndReturnGenerateNewSession = conn => {
  let newSession = generateNewSession();
  let params = [|
    Uuidm.to_string(newSession.id),
    switch (newSession.expireAt) {
    | Some(ptime) => Ptime.to_rfc3339(ptime)
    | None => ""
    },
    switch (newSession.createdAt) {
    | Some(ptime) => Ptime.to_rfc3339(ptime)
    | None => ""
    },
    switch (newSession.updatedAt) {
    | Some(ptime) => Ptime.to_rfc3339(ptime)
    | None => ""
    },
  |];
  OP.sendPrepared(conn, ~params, ~name="ds_add_new_session", ())
  >>= (
    res => {
      let status = res#status;

      Postgresql.(
        switch (status) {
        | Tuples_ok => Deferred.return(newSession)
        /*
           Deferred.return(
         Result.Ok("Successfully updated userId in session"),
         )*/
        | _ => Deferred.return(newSession)
        /* TODO: Need to deal with error cases here!!!         raise(Not_found)*/
        /*Deferred.return(Result.Error("Some kind of error I guess..."))*/
        }
      );
    }
  );
};

let findOrCreateSessionByCookie =
    (conn: OneDb.connPool, sessionId: option(string))
    : Deferred.t(session) =>
  switch (sessionId) {
  | Some(id) =>
    let params = [|id|];
    Async.(
      OP.sendPrepared(conn, ~params, ~name="ds_find_session_by_id", ())
      >>= (
        res =>
          if (Array.length(res#get_all) > 0) {
            let session = ofDbResult(res#get_all[0]);
            Deferred.return(session);
          } else {
            storeAndReturnGenerateNewSession(conn);
          }
      )
    );
  | None => storeAndReturnGenerateNewSession(conn)
  };

let preparedStatements =
  OP.[
    {
      name: "ds_session_set_userid",
      statement:
        Printf.sprintf("UPDATE sessions set user_id = $1 where id = $2;"),
    },
    {
      name: "ds_find_session_by_id",
      statement: Printf.sprintf("SELECT * FROM sessions WHERE id = $1;"),
      /*TODO: and expire_at >now()*/
    },
    {
      name: "ds_add_new_session",
      statement:
        Printf.sprintf(
          "INSERT INTO sessions (id, expire_at, created_at ,updated_at) VALUES ($1, $2, $3, $4);",
        ),
    },
  ];
