module OP = Onepostsql;

open Async;

open Common;

let defer = Deferred.return;

let emailRe = Re.Pcre.regexp(~flags=[`CASELESS], "(.+)@(.+)\\.(.+)");

let isValidEmail = emailString =>
  try (
    switch (
      Array.length(Re.Group.all(List.hd(Re.all(emailRe, emailString))))
    ) {
    | length when length > 3 => true
    | _ => false
    }
  ) {
  | _ => false
  };

[@deriving yojson]
type settings = {
  tours: list(string),
  version: int,
};

type userId = Uuidm.t;

type user = {
  id: userId,
  fullName: string,
  email: string,
  password: string,
  confirmed: bool,
  resetPasswordToken: option(string),
  resetPasswordLinkSentAt: option(Ptime.t),
  settings,
  createdAt: Ptime.t,
  updatedAt: Ptime.t,
};

type createUser = {
  id: Uuidm.t,
  fullName: string,
  email: string,
  password: string,
  confirmed: bool,
  settings: option(settings),
};

type publicUser = {
  id: string,
  fullName: string,
  email: string,
  confirmed: bool,
  settings,
};

let publicUserOfUser = (user: user) : publicUser => {
  id: Uuidm.to_string(user.id),
  fullName: user.fullName,
  email: user.email,
  confirmed: user.confirmed,
  settings: user.settings,
};

let fieldsList = "dsu.id, dsu.full_name, dsu.email, dsu.password, dsu.created_at, dsu.updated_at, dsu.confirmed, dsu.reset_password_token, dsu.reset_password_link_sent_at, dsu.settings";

let emptySettings = {tours: [], version: 0};

let ofDbResult = tuple =>
  switch (tuple) {
  | [|
      id,
      fullName,
      email,
      password,
      createdAt,
      updatedAt,
      confirmed,
      resetPasswordToken,
      resetPasswordLinkSentAt,
      settingsString,
    |] =>
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
    let confirmed = confirmed == "true";
    let resetPasswordToken =
      switch (resetPasswordToken) {
      | "" => None
      | value => Some(value)
      };
    let resetPasswordLinkSentAt =
      switch (resetPasswordLinkSentAt) {
      | "" => None
      | value => Some(OP.ptimeOfDbTs(value))
      };
    let createdAt = OP.ptimeOfDbTs(createdAt);
    let updatedAt = OP.ptimeOfDbTs(updatedAt);
    let settings =
      switch (settings_of_yojson @@ Yojson.Safe.from_string(settingsString)) {
      | Ok(settings) => settings
      | Error(_message) =>
        failPublic(
          ~internal=
            "Badly formatted user settings json string: " ++ settingsString,
          ~public="We hit an internal error",
          (),
        )
      };
    {
      id,
      fullName,
      email,
      password,
      confirmed,
      resetPasswordToken,
      resetPasswordLinkSentAt,
      settings,
      createdAt,
      updatedAt,
    };
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

let arrayAllByUserId = (conn: OneDb.connPool, id) => {
  let stringId = Uuidm.to_string(id);
  let params = [|stringId|];
  OP.sendPrepared(conn, ~params, ~name="ds_user_by_id", ())
  >>| (res => Array.map(record => ofDbResult(record), res#get_all));
};

let allByUserId = (conn: OneDb.connPool, id) =>
  arrayAllByUserId(conn, id) >>| Array.to_list;

let byId = (conn: OneDb.connPool, id) : Deferred.t(option(user)) => {
  let stringId = Uuidm.to_string(id);
  let params = [|stringId|];
  Async.(
    OP.sendPrepared(conn, ~params, ~name="ds_user_by_id", ())
    >>| (
      res =>
        try (Some(ofDbResult(res#get_all[0]))) {
        | _ => None
        }
    )
  );
};

let byEmail = (conn: OneDb.connPool, email) : Deferred.t(option(user)) => {
  open Async;
  let params = [|email|];
  OP.sendPrepared(conn, ~params, ~name="ds_user_by_email", ())
  >>| (
    res =>
      try (Some(ofDbResult(res#get_all[0]))) {
      | _ => None
      }
  );
};

let byEmailAndPassword =
    (conn: OneDb.connPool, email, password)
    : Deferred.t(option(user)) => {
  let params = [|email|];
  OP.sendPrepared(conn, ~params, ~name="ds_user_by_email", ())
  >>| (
    res =>
      try (
        {
          let user = ofDbResult(res#get_all[0]);
          Bcrypt.verify(password, Bcrypt.hash_of_string(user.password)) ?
            Some(user) : None;
        }
      ) {
      | exn =>
        OneLog.infof(
          "Exception trying to get user: %s",
          Core.Exn.to_string(exn),
        );
        None;
      }
  );
};

let byPasswordResetToken =
    (conn: OneDb.connPool, token)
    : Deferred.t(option(user)) => {
  let params = [|token|];
  OP.sendPrepared(conn, ~params, ~name="ds_user_by_reset_password_token", ())
  >>| (
    res =>
      try (Some(ofDbResult(res#get_all[0]))) {
      | _ => None
      }
  );
};

let resetPasswordResetTokenByEmail =
    (conn: OneDb.connPool, email)
    : Deferred.t(option(Uuidm.t)) => {
  let params = [|email|];
  /* Use this prepared query to rate-limit reset tokens */
  OP.sendPrepared(
    conn,
    ~params,
    ~name="ds_user_valid_by_email_for_password_reset",
    (),
  )
  >>= (
    res =>
      switch (res#ntuples) {
      | 0 => defer(None)
      | _ =>
        let status = res#status;
        Postgresql.(
          switch (status) {
          | Tuples_ok =>
            try (
              {
                let user = ofDbResult(res#get_all[0]);
                let token = Utils.genUuid();
                let params = [|user.email, Uuidm.to_string(token)|];
                OP.sendPrepared(
                  conn,
                  ~params,
                  ~name="ds_user_refresh_reset_password_token_by_email",
                  (),
                )
                >>| (_ => Some(token));
              }
            ) {
            | _ => defer(None)
            }
          | _ => defer(None)
          }
        );
      }
  );
};

let resetPasswordByResetPasswordToken =
    (conn: OneDb.connPool, token, password, passwordConfirm)
    : Deferred.t(result(string, string)) => {
  let params = [|token|];
  OP.sendPrepared(conn, ~params, ~name="ds_user_by_reset_password_token", ())
  >>= (
    res =>
      switch (res#ntuples) {
      | 0 => defer(Result.Error("No user with that token"))
      | _ =>
        let status = res#status;
        Postgresql.(
          switch (status) {
          | Tuples_ok =>
            switch (
              password,
              passwordConfirm,
              password == passwordConfirm,
              String.length(password),
              String.length(passwordConfirm),
            ) {
            | (_, _, _, passwordLength, _) when passwordLength < 6 =>
              defer(
                Result.Error("Password must be at least 6 characters long"),
              )
            | (_, _, false, _, _) =>
              defer(Result.Error("Password does not match confirmation"))
            | (password, _, _, _, _) =>
              let newToken = Utils.genUuid();
              /* 12 count/rounds taks ~250ms on 2014 mbp */
              BlockingHashing.nonBlockHash(~count=12, password)
              >>= (
                hashedPassword =>
                  OP.sendPrepared(
                    conn,
                    ~params=[|
                      token,
                      hashedPassword,
                      Uuidm.to_string(newToken),
                    |],
                    ~name="ds_user_reset_password_via_reset_token",
                    (),
                  )
                  >>| (
                    res => {
                      let status = res#status;
                      Postgresql.(
                        switch (status) {
                        | Command_ok =>
                          let user = ofDbResult(res#get_all[0]);
                          Result.Ok("Reset password");
                        | _ => Result.Error("Couldn't reset password")
                        }
                      );
                    }
                  )
              );
            }
          | _ => defer(Result.Error("Couldn't reset password"))
          }
        );
      }
  )
  >>| (
    fun
    | Result.Ok(value) => Result.Ok(value)
    | Result.Error(_exn) => Result.Error("Couldn't reset password")
  );
};

let insert =
    (
      conn: OneDb.connPool,
      fullName: string,
      email: string,
      password: string,
      passwordConfirm: string,
      confirmed: bool,
      agreeToTOS: bool,
      id: option(Uuidm.t),
    )
    : Deferred.t(result(createUser, string)) =>
  switch (
    email,
    password,
    passwordConfirm,
    password == passwordConfirm,
    String.length(email),
    String.length(password),
    String.length(passwordConfirm),
    agreeToTOS,
    String.length(String.trim(fullName)),
  ) {
  | (email, _, _, _, _, _, _, _, _) when ! isValidEmail(email) =>
    defer(Error("Email is invalid"))
  | (_, _, _, _, _, passwordLength, _, _, _) when passwordLength < 6 =>
    defer(Error("Password must be at least 6 characters long"))
  | (_, _, _, _, _, _, _, _, nameLength) when nameLength < 1 =>
    defer(Error("Name cannot be blank"))
  | (_, _, _, _, _, _, _, false, _) =>
    defer(Error("You must agree to the TOS"))
  | (_, _, _, false, _, _, _, _, _) =>
    defer(Error("Password does not match confirmation"))
  | (email, password, _, _, _, _, _, _, _) =>
    byEmail(conn, email)
    >>= (
      fun
      | Some(_) =>
        defer(Error("A user with that email is already registered"))
      | None => {
          let hashedPassword =
            Bcrypt.hash(~count=15, password) |> Bcrypt.string_of_hash;
          let user = {
            id:
              switch (id) {
              | Some(id) => id
              | None => Utils.genUuid()
              },
            fullName,
            email,
            confirmed,
            password: hashedPassword,
            settings: Some(emptySettings),
          };
          /* print_endline ("inserting: " ^ show_createUser user); */
          OP.sendPrepared(
            conn,
            ~params=[|
              Uuidm.to_string(user.id),
              user.fullName,
              user.email,
              user.password,
              string_of_bool(user.confirmed),
              Yojson.Safe.to_string(settings_to_yojson(emptySettings)),
            |],
            ~name="insert_ds_user",
            (),
          )
          >>| (
            res => {
              let status = res#status;
              Postgresql.(
                switch (status) {
                | Command_ok => Result.Ok(user)
                | _ => Result.Error("Couldn't create user")
                }
              );
            }
          );
        }
    )
  };

let updateCompletedToursById =
    (conn: OneDb.connPool, id, tours: list(string))
    : Deferred.t(option(user)) => {
  let stringId = Uuidm.to_string(id);
  let stringTours =
    Yojson.Basic.to_string @@ `List(List.map(tour => `String(tour), tours));
  let params = [|stringId, stringTours|];
  Async.(
    OP.sendPrepared(
      conn,
      ~params,
      ~name="ds_user_set_completed_tours_by_id",
      ~expectStatus=Tuples_ok,
      (),
    )
    >>| (
      res =>
        try (Some(ofDbResult(res#get_all[0]))) {
        | _ => None
        }
    )
  );
};

let preparedStatements =
  OP.[
    {
      name: "insert_ds_user",
      statement: "INSERT INTO ds_user (id, full_name, email, password, confirmed, settings) VALUES ($1, $2, $3, $4, $5, $6)",
    },
    {
      name: "ds_user_by_id",
      statement:
        Printf.sprintf(
          "SELECT %s FROM ds_user dsu WHERE id = $1",
          fieldsList,
        ),
    },
    {
      name: "ds_user_by_email",
      statement:
        Printf.sprintf(
          "SELECT %s FROM ds_user dsu WHERE email = $1",
          fieldsList,
        ),
    },
    {
      name: "all_ds_users_by_org_id",
      statement:
        Printf.sprintf(
          "SELECT %s FROM ds_user dsu JOIN org_user ON (org_user.ds_user_id = dsu.id) WHERE org_user.org_id = $1",
          fieldsList,
        ),
    },
    {
      name: "ds_user_refresh_reset_password_token_by_email",
      statement: "UPDATE ds_user SET reset_password_token=$2, reset_password_link_sent_at=now() where email=$1",
    },
    {
      name: "ds_user_reset_password_via_reset_token",
      statement: "UPDATE ds_user SET password=$2, reset_password_token=$3 where reset_password_token=$1",
    },
    {
      name: "ds_user_by_reset_password_token",
      statement:
        Printf.sprintf(
          "SELECT %s FROM ds_user dsu WHERE dsu.reset_password_token IS NOT NULL AND dsu.reset_password_token = $1",
          fieldsList,
        ),
    },
    {
      name: "ds_user_valid_by_email_for_password_reset",
      statement:
        Printf.sprintf(
          "SELECT %s FROM ds_user dsu WHERE dsu.email = $1 AND (dsu.reset_password_link_sent_at IS NULL OR dsu.reset_password_link_sent_at < NOW() - INTERVAL '1 minute')",
          fieldsList,
        ),
    },
    {
      name: "ds_user_set_completed_tours_by_id",
      statement:
        Printf.sprintf(
          "UPDATE ds_user dsu SET settings = JSONB_SET(settings, '{tours}', $2) WHERE dsu.id = $1 RETURNING %s",
          fieldsList,
        ),
    },
  ];

let adminEmails = ["yukims19@gmail.com"];

let isAdmin = (user: option(user)) =>
  switch (user) {
  | Some(user) => List.exists(email => email == user.email, adminEmails)
  | None => false
  };
