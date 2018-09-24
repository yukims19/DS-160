open Async;
open Graphql_async;

/*
 type client = {
   id: Uuidm.t,
   name: string,
   dataSheet: string,
   applicationId: string,
   createdAt: option(Ptime.t),
   updatedAt: option(Ptime.t),
 };*/

type gqlContext = {
  db: OneDb.connPool,
  user: option(DsUserModel.user),
  sessionId: Uuidm.t,
};

let user =
  Schema.(
    obj("User", ~fields=_storyType =>
      [
        field(
          "id",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, u: DsUserModel.user) =>
          Uuidm.to_string(u.id)
        ),
        field(
          "username",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, u: DsUserModel.user) =>
          u.username
        ),
      ]
    )
  );

let client =
  Schema.(
    obj("Client", ~fields=_client =>
      [
        field(
          "id",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, c: DsUserModel.client) =>
          Uuidm.to_string(c.id)
        ),
        field(
          "name",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, c: DsUserModel.client) =>
          c.name
        ),
        field(
          "dataSheet",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, c: DsUserModel.client) =>
          c.dataSheet
        ),
        field(
          "applicationId",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, c: DsUserModel.client) =>
          c.applicationId
        ),
        field(
          "createdAt",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, c: DsUserModel.client) =>
          c.createdAt
        ),
        field(
          "updatedAt",
          ~args=Arg.([]),
          ~typ=non_null(string),
          ~resolve=(_ctx, c: DsUserModel.client) =>
          c.updatedAt
        ),
      ]
    )
  );

let schema =
  Schema.(
    schema(
      ~mutations=[
        io_field(
          "addNewClient",
          ~typ=non_null(string),
          ~args=
            Arg.[
              arg("name", ~typ=non_null(string)),
              arg("dataSheet", ~typ=non_null(string)),
            ],
          ~resolve=(ctx, (), name, dataSheet) =>
          switch (ctx.user) {
          | Some(user) =>
            let addClientResults =
              DsUserModel.addNewClient(
                ctx.db,
                Uuidm.to_string(user.id),
                name,
                dataSheet,
              );

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

          | None => Deferred.return(Error("Please login in"))
          }
        ),
        io_field(
          "login",
          ~typ=non_null(string),
          ~args=
            Arg.[
              arg("username", ~typ=non_null(string)),
              arg("password", ~typ=non_null(string)),
            ],
          ~resolve=(ctx, (), username, password) => {
            let user =
              DsUserModel.byUsernameAndPassword(ctx.db, username, password);
            let returnResults =
              user
              >>= (
                result =>
                  switch (result) {
                  | Some(user) =>
                    DsSession.setUserIdBySessionId(
                      ctx.db,
                      ctx.sessionId,
                      user.id,
                    )
                    >>= (
                      result =>
                        switch (result) {
                        | Ok(message) => Deferred.return(Ok(message))
                        | Error(message) => Deferred.return(Error(message))
                        }
                    )

                  /*                    Deferred.return(
                                          Ok("Successfully logged in as " ++ user.username),
                                        );*/
                  | None => Deferred.return(Error("No User Found!!!"))
                  }
              );
            returnResults;
          },
        ),
      ],
      [
        field("me", ~args=Arg.([]), ~typ=user, ~resolve=(ctx, ()) => ctx.user),
        io_field(
          "clients",
          ~typ=list(non_null(client)),
          ~args=Arg.([]),
          ~resolve=(ctx: gqlContext, ()) => {
            let clientDBResults =
              DsUserModel.allClientsByUserId(ctx.db, ctx.user);
            let returnResults =
              clientDBResults
              >>= (
                result => {
                  /*                  let clientList =
                                       Array.map(
                                         (res: DsUserModel.client) => {
                                           let returnClient = {
                                             id: res.id,
                                             name: res.name,
                                             dataSheet: res.dataSheet,
                                             applicationId: res.applicationId,
                                             createdAt: Ptime.of_float_s(time),
                                             updatedAt: Ptime.of_float_s(time),
                                           };

                                           returnClient;
                                         },
                                         result,
                                      )*/
                  let clientList = result |> Array.to_list;

                  Ok(Some(clientList)) |> Deferred.return;
                }
              );
            returnResults;
          },
        ),
        field(
          "greeter",
          ~typ=non_null(string),
          ~args=Arg.([]),
          ~resolve=(_ctx, ()) =>
          "Hello Just Greeting"
        ),
      ],
    )
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
   ;)
 */
