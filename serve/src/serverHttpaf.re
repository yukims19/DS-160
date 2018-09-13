open Core;

open Async;

open Httpaf;

let error_handler = (_, ~request=?, error, startResponse) => {
  let responseBody = startResponse(Headers.of_list([("status", "500")]));
  switch (error) {
  | `Exn(exn) =>
    switch (request) {
    | None => ()
    | Some(request) => ignore(OneError.reportException(exn, request))
    };
    OneLog.errorf("In ErrorHandler: %s", Exn.to_string(exn));
  | #Status.standard as error =>
    OneLog.error("ServerHttpaf error_handler:");
    OneLog.error(Status.default_reason_phrase(error));
  };
  Body.close(responseBody);
};

let callback = (connPool, _, reqd) => {
  let reqId = Utils.genUuid();
  ignore(WebRoutesHttpaf.routes(connPool, reqId, reqd));
};

let start = portNumber =>
  {
    let maxAcceptsPerBatch = 255;
    let connPool = Lazy.force(OneDb.connPool);
    /* XXX: Careful about binding to all addresses, could be
       dangerous. Probably should explicitly bind to 127.0.0.1 */
    let port = Async.Tcp.Bind_to_port.(On_port(portNumber));
    let whereToListen =
      Tcp.(Where_to_listen.(bind_to(Bind_to_address.(All_addresses), port)));
    let reqHandler = callback(connPool);
    let connectionConfig =
      Httpaf.Server_connection.Config.{
        read_buffer_size: 128000,
        response_buffer_size: 1024,
        response_body_buffer_size: 4096,
      };
    let connectionHandler =
      Httpaf_async.Server.create_connection_handler(
        ~config=connectionConfig,
        ~request_handler=reqHandler,
        ~error_handler,
      );
    Tcp.Server.create_sock(
      ~backlog=11000,
      ~max_connections=10000,
      ~max_accepts_per_batch=maxAcceptsPerBatch,
      ~on_handler_error=
        `Call(
          (_address, e) =>
            OneLog.fatalf(
              "uncaught error in Tcp.Server.create_sock error=%s",
              Core.Exn.to_string(e),
            ),
        ),
      whereToListen,
      connectionHandler,
    );
  }
  >>= (
    server => {
      Deferred.forever((), () =>
        Clock.after(Time.Span.(of_sec(60.0)))
        >>| (
          () => {
            ();
            ();
            OneLog.infof("conns: %d", Tcp.Server.num_connections(server));
          }
        )
      );
      OneLog.info(
        "Server bound to port, OneGraph started, accepting connections now",
      );
      Deferred.never();
    }
  );
