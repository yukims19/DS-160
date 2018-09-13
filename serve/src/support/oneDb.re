open Postgresql;

let dbVarName = OneConfig.(details.dbVarName);

let dbUri =
  try (Sys.getenv(dbVarName) |> Uri.of_string) {
  | Not_found => raise(Invalid_argument(dbVarName ++ " must be set in ENV"))
  };

let getExn = (name, opt) =>
  switch (opt) {
  | Some(v) => v
  | None =>
    raise(Invalid_argument("unwrapUnsafely called on None: " ++ name))
  };

let param = name => Uri.get_query_param(dbUri, name) |> getExn(name);

let dbname =
  String.sub(Uri.path(dbUri), 1, String.length(Uri.path(dbUri)) - 1);

let makeConn = () =>
  try (
    (new connection)(
      ~host=getExn("host", Uri.host(dbUri)),
      ~port=string_of_int(getExn("port", Uri.port(dbUri))),
      ~user=param("user"),
      ~password=param("password"),
      ~dbname,
      (),
    )
  ) {
  | exc =>
    OneConfig.(details.isProd) ?
      raise(exc) :
      (new connection)(
        ~host=getExn("host", Uri.host(dbUri)),
        ~port=string_of_int(getExn("port", Uri.port(dbUri))),
        ~dbname,
        (),
      )
  };

type connPool = Async.Throttle.t((int, Postgresql.connection));

let conns =
  lazy (
    try (List.init(OneConfig.connPoolSize, i => (i, makeConn()))) {
    | Postgresql.Error(e) =>
      OneLog.errorf(
        "Top level postgres failure error=%s",
        string_of_error(e),
      );
      raise(Failure("Top level postgres failure: " ++ string_of_error(e)));
    }
  );

let connPool: Lazy.t(connPool) =
  lazy (
    Async.Throttle.create_with(~continue_on_error=true, Lazy.force(conns))
  );
