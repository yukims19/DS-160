open Async;
open Graphql_async;

type user = {
  id: int,
  username: string,
  password: string,
  status: bool,
};

type client = {
  id: int,
  name: string,
  dataSheet: string,
  applicationId: string,
  createdAt: option(Ptime.t),
  updatedAt: option(Ptime.t),
};

type gqlContext = {db: OneDb.connPool};

let client =
  Schema.(
    obj("Client", ~fields=_storyType =>
      [
        field("id", ~args=Arg.([]), ~typ=non_null(int), ~resolve=(_ctx, c) =>
          c.id
        ),
        field(
          "name", ~args=Arg.([]), ~typ=non_null(string), ~resolve=(_ctx, c) =>
          c.name
        ),
        field(
          "dataSheet",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, c) =>
          c.dataSheet
        ),
        field(
          "applicationId",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, c) =>
          c.applicationId
        ),
        field(
          "createdAt",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, c) =>
          switch (c.createdAt) {
          | Some(time) => Ptime.to_rfc3339(time)
          | None => ""
          }
        ),
        field(
          "updatedAt",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, c) =>
          switch (c.updatedAt) {
          | Some(time) => Ptime.to_rfc3339(time)
          | None => ""
          }
        ),
      ]
    )
  );

let schema =
  Schema.(
    schema([
      io_field(
        "users",
        ~args=
          Arg.[
            arg("username", ~typ=non_null(string)),
            arg("password", ~typ=non_null(string)),
          ],
        ~typ=bool,
        ~resolve=(ctx, (), username, password) => {
          let userDBResults =
            DsUserModel.byUsernameAndPassword(ctx.db, username, password);
      ~mutations=[
        io_field(
          "addNewClient",
          ~typ=non_null(string),
          ~args=
            Arg.[
              arg("name", ~typ=non_null(string)),
              arg("userId", ~typ=non_null(string)),
              arg("dataSheet", ~typ=non_null(string)),
            ],
          ~resolve=(ctx, (), name, userId, dataSheet) => {
            let addClientResults =
              DsUserModel.addNewClient(ctx.db, userId, name, dataSheet);

            let returnResults =
              addClientResults
              >>= (
                result =>
                  Deferred.return(result)
                  /*
                    switch (result) {
                   | Ok(value) => Deferred.return(value)
                    | Error(value) => Deferred.return(value)
                    }
                   */
              );
            returnResults;
          },
        ),
      ],

          let returnResults =
            userDBResults
            >>= (
              result =>
                switch (result) {
                | Some(_result) => Deferred.return(Ok(Some(true)))
                | None => Deferred.return(Ok(Some(false)))
                }
            );
          returnResults;
        },
      ),
      field(
        "clients",
        ~typ=list(non_null(client)),
        ~args=Arg.([]),
        ~resolve=(_ctx, ()) => {
          let time = Unix.gettimeofday();
          let re = [
            {
              id: 1,
              name: "Client1",
              dataSheet: "www.1.com",
              applicationId: "A1234",
              createdAt: Ptime.of_float_s(time),
              updatedAt: Ptime.of_float_s(time),
            },
          ];
          Some(re);
        },
      ),
      field(
        "greeter", ~typ=non_null(string), ~args=Arg.([]), ~resolve=(_ctx, ()) =>
        "Hello Just Greeting"
      ),
    ])
  );

/*
   let user =
   Schema.(
   obj("User", ~fields=_userGqlType =>
   [
   field("id", ~args=Arg.([]), ~typ=non_null(int), ~resolve=(_ctx, u) =>
   u.id
   ),
   field(
   "username",
   ~args=Arg.([]),
   ~typ=non_null(string),
   ~resolve=(_ctx, u) =>
   u.username
   ),
   field(
   "password",
   ~args=Arg.([]),
   ~typ=non_null(string),
   ~resolve=(_ctx, u) =>
   u.password
   ),
   field(
   "status", ~args=Arg.([]), ~typ=non_null(bool), ~resolve=(_ctx, u) =>
   u.status
   ),
   ]
   )
   );
 */
