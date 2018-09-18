open Async;

let registerAllQueries = (conn, connId) =>
  ModelCommon.insertPreparedStatements(
    ~conn,
    ~connId,
    ~statements=List.concat([DsUserModel.preparedStatements]),
  );

let initLogs = () => OneLog.setup();

let initNoCrypto = () => Nocrypto_entropy_unix.initialize();

let initDb = () =>
  Deferred.all_unit(
    List.map(
      ((connId, conn)) => registerAllQueries(conn, connId),
      Lazy.force(OneDb.conns),
    ),
  );

let initServer = portNumber => ServerHttpaf.start(portNumber);

let start = (spec, ()) => {
  initLogs();
  initNoCrypto();
  initDb() >>= (() => initServer(spec));
};
