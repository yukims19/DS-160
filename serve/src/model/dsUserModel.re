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

let preparedStatements =
  OP.[
    {
      name: "ds_user_by_username_and_password",
      statement:
        Printf.sprintf(
          "SELECT * FROM users WHERE username = $1 AND password = $2",
        ),
    },
  ];
