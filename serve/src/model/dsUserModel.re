module OP = Onepostsql;
open Async;
open Common;
let defer = Deferred.return;

type user = {
  id: Uuidm.t,
  username: string,
  password: string,
  createdAt: option(Ptime.t),
  lastLoggedIn: option(Ptime.t),
};
type client = {
  id: int,
  userId: int,
  name: string,
  dataSheet: string,
  applicationId: string,
  createdAt: string,
  updatedAt: string,
};

let ofDbResult = tuple =>
  switch (tuple) {
  | [|id, userId, name, dataSheet, applicationId, createdAt, updatedAt|] =>
    let id = int_of_string(id);
    let userId = int_of_string(userId);
    {id, userId, name, dataSheet, applicationId, createdAt, updatedAt};
  | resFields =>
    failPublic(
      ~internal=
        Printf.sprintf(
          "Invalid DB tuple shape for oneUser: fields=%s",
          String.concat(", ", Array.to_list(resFields)),
        ),
      ~public="We hit an internal error",
      (),
    )
  };

let ofDbResultUser = tuple =>
  switch (tuple) {
  | [|id, username, password, createdAt, lastLoggedIn|] =>
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
    let createdAt = Some(OP.ptimeOfDbTs(createdAt));
    let lastLoggedIn = Some(OP.ptimeOfDbTs(lastLoggedIn));
    {id, username, password, createdAt, lastLoggedIn};
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

let byUsernameAndPassword =
    (conn: OneDb.connPool, username: string, password: string)
    : Deferred.t(option(user)) => {
  let params = [|username, password|];
  Async.(
    OP.sendPrepared(
      conn,
      ~params,
      ~name="ds_user_by_username_and_password",
      (),
    )
    >>| (
      res =>
        try (Some(ofDbResultUser(res#get_all[0]))) {
        | _ => None
        }
    )
  );
};

let getUserById =
    (conn: OneDb.connPool, userId: Uuidm.t)
    : Deferred.t(option(user)) => {
  let params = [|Uuidm.to_string(userId)|];
  Async.(
    OP.sendPrepared(conn, ~params, ~name="ds_user_by_id", ())
    >>| (
      res =>
        try (Some(ofDbResultUser(res#get_all[0]))) {
        | _ => None
        }
    )
  );
};

let allClientsByUserId = (conn: OneDb.connPool, userId) => {
  let params = [|userId|];
  OP.sendPrepared(conn, ~params, ~name="ds_all_clients_by_userid", ())
  >>| (res => Array.map(record => ofDbResult(record), res#get_all));
};

let addNewClient = (conn: OneDb.connPool, userId, name, datasheet) => {
  let time = Unix.gettimeofday();
  let currentTime = Ptime.of_float_s(time);
  let insertingTime =
    switch (currentTime) {
    | Some(time) => Ptime.to_rfc3339(time)
    | None => ""
    };
  let params = [|userId, name, datasheet, insertingTime, insertingTime|];
  OP.sendPrepared(conn, ~params, ~name="ds_add_new_client", ())
  >>| (
    res => {
      let status = res#status;
      Postgresql.(
        switch (status) {
        | Command_ok => Result.Ok("Success")
        | _ => Result.Error("Couldn't create user")
        }
      );
    }
  );
};

let preparedStatements =
  OP.[
    {
      name: "ds_user_by_username_and_password",
      statement:
        Printf.sprintf(
          "SELECT * FROM users WHERE username = $1 AND password = $2",
        ),
    },
    {
      name: "ds_user_by_id",
      statement: Printf.sprintf("SELECT * FROM users WHERE id = $1;"),
    },
    {
      name: "ds_all_clients_by_userid",
      statement: Printf.sprintf("SELECT * FROM clients WHERE user_id = $1"),
    },
    {
      name: "ds_add_new_client",
      statement:
        Printf.sprintf(
          "INSERT INTO clients(user_id , name , data_sheet, time_created , time_last_updated) VALUES ($1, $2, $3, $4, $5);",
        ),
    },
  ];
