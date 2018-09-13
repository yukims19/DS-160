module OP = Onepostsql;

open Async;

let sendStatement =
    (
      ~dbConn: OneDb.connPool,
      ~statement: OP.preparedStatement,
      ~params: array(string),
    ) =>
  OP.sendPrepared(dbConn, ~name=statement.name, ~params, ());

let insertPreparedStatements =
    (
      ~conn: Postgresql.connection,
      ~connId: int,
      ~statements: list(OP.preparedStatement),
    ) =>
  OP.prepareStatements(conn, connId, statements) |> Deferred.ignore;

let ensureUuidOfString = (id: string) =>
  switch (Uuidm.of_string(id)) {
  | None =>
    OneBase.failPublic(
      ~internal="Invalid uuid from db (id): " ++ id,
      ~public="We hit an internal error",
      (),
    )
  | Some(uuid) => uuid
  };
