module OP = Onepostsql;
open Async;
open Common;
let defer = Deferred.return;

type user = {
  id: int,
  username: string,
  password: string,
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
        if (Array.length(res#get_all) >= 1) {
          Some({
            id: int_of_string(res#get_all[0][0]),
            username: res#get_all[0][1],
            password: res#get_all[0][2],
          });
        } else {
          None;
        }
    )
  );
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
      name: "ds_add_new_client",
      statement:
        Printf.sprintf(
          "INSERT INTO clients(user_id , name , data_sheet, time_created , time_last_updated) VALUES ($1, $2, $3, $4, $5);",
        ),
    },
  ];
