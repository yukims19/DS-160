[@deriving_inline sexp]
exception TestingError(string);

open Async;

let defer = Deferred.return;

open Httpaf;

type request = {
  dbConn: OneDb.connPool,
  reqId: Uuidm.t,
  req: Httpaf.Request.t,
  origin: string,
  bodyString: string,
  uri: Uri.t,
};

let bofs = x => Bigstring.of_string(x);

let raiseErr = err => raise(Failure(err));

[@deriving yojson]
type graphqlError = {errors: list(string)};

type ogResponse = (Httpaf.Headers.t, Httpaf.Status.t, Httpaf.Bigstring.t);

let cookieKey = "__one_graph";

let defer = Deferred.return;

let text = "hi world";

let exampleHeaders =
  Headers.of_list([
    (
      "content-length",
      string_of_int(Bigstring.length(Bigstring.of_string(text))),
    ),
  ]);

let contentLengthHeader = content =>
  Headers.of_list([
    ("content-length", string_of_int(Bigstring.length(content))),
  ]);

let contentTypeHeader = kind => Headers.of_list([("content-type", kind)]);

let withContentType = (kind, headers) =>
  Headers.add(headers, "content-type", kind);

let sessionlessHeaders = Httpaf.Headers.empty;

open Graphql_async;

let fetchOrCreateSession = (connPool, headers) => {
  let sessionId = Httpaf.Headers.get(headers, "Cookie");
  print_endline(
    "Headers:::::::~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    ++ Headers.to_string(headers),
let coHttpHeadersOfHttpafHeaders = httpAfHeaders => {
  let headers = ref([]);
  Httpaf.Headers.iter(
    ~f=(name, value) => headers := List.append(headers^, [(name, value)]),
    httpAfHeaders,
  );
  Cohttp.Header.of_list(headers^);
};

let ofHeader = (conn, cookieFieldKey, header: Cohttp.Header.t) => {
  let cookieValues = Cohttp.Cookie.Cookie_hdr.extract(header);
  try (
    {
      let sessionKey = List.assoc(cookieFieldKey, cookieValues);
      Some(sessionKey);
      /* Look up session with sessionKey here */
    }
  ) {
  | Not_found => None
  };
};

  DsSession.findOrCreateSessionByCookie(connPool, sessionId);
  /*
   (
     switch (sessionId) {
     | Some(sessionId) =>
       print_endline("Reached ExecuteqQuery, Cookie:" ++ sessionId)
     | None => print_endline("No Cookies, No sessionId")
     }
   );
   */
};

let createGqlCtx = (connPool, headers) =>
  fetchOrCreateSession(connPool, headers)
  >>= (
    session => {
      let userId = session.userId;
      print_endline(
        "Found or Created sessionId:" ++ Uuidm.to_string(session.id),
      );
      switch (userId) {
      | Some(userId) =>
        print_endline("Found some userId");
        DsUserModel.getUserById(connPool, userId)
        >>= (
          fetchedUser => {
            let ctx =
              DsSchema.{
                db: connPool,
                user: fetchedUser,
                sessionId: session.id,
              };
            switch (ctx.user) {
            | Some(user) =>
              print_endline("Found a user" ++ Uuidm.to_string(user.id))
            | None =>
              print_endline(
                "No user found with the userId" ++ Uuidm.to_string(userId),
              )
            };
            Deferred.return(ctx);
          }
        );
      | None =>
        print_endline("No userId in the session table found");
        let ctx = DsSchema.{db: connPool, user: None, sessionId: session.id};
        Deferred.return(ctx);
      };
    }
  );

let executeQuery = (connPool, queryBody, variables, headers: Headers.t) =>
  createGqlCtx(connPool, headers)
  >>= (
    ctx => {
      let r = Graphql_parser.parse(queryBody);
      switch (r) {
      | Ok(query) =>
        Graphql_async.Schema.execute(DsSchema.schema, ctx, ~variables, query)
      | Error(err) => failwith(err)
      };
    }
  );

let requestOrigin = (req: Httpaf.Request.t) =>
  switch (Httpaf.Headers.get(req.headers, "origin")) {
  | None => "unknown"
  | Some(origin) => origin
  };

let getCORSHeaders = req => {
  let origin = requestOrigin(req);
  let headers =
    Headers.of_list([
      (
        "Access-Control-Allow-Headers",
        "content-type,app-id,auth-token,show_beta_schema,authentication",
      ),
      ("Access-Control-Allow-Methods", "POST"),
      ("Access-Control-Allow-Credentials", "true"),
      ("Access-Control-Allow-Origin", origin),
    ]);
  headers;
};

let toCookieHeaders =
    (
      ~discard=false,
      ~path=?,
      ~domain=?,
      ~secure=?,
      ~http_only=?,
      ~expireDate,
      cookieKey: string,
      sessionKey: string,
    ) => {
  let cookie = (cookieKey, sessionKey)
  and expiration =
    switch (discard) {
    | false => `Max_age(Int64.of_float(Ptime.to_float_s(expireDate)))
    | true => `Session
    };
  let (hdr, val_) =
    Cohttp.Cookie.Set_cookie_hdr.serialize(
      Cohttp.Cookie.Set_cookie_hdr.make(
        ~expiration,
        ~path?,
        ~domain?,
        ~secure?,
        ~http_only?,
        cookie,
      ),
    );
  [(hdr, val_), ("vary", "cookie")];
};

let dynamicQueryHandler = (~readOnly as _, _keys, _rest, request) => {
  let {bodyString} = request;
  let bodyJson = Yojson.Basic.from_string(bodyString);
  let {req} = request;
  let sessionIo = fetchOrCreateSession(request.dbConn, req.headers);

  let headersCORS = getCORSHeaders(req);
  sessionIo
  >>| (
    session => {
      let headersResponse =
        Headers.add_list(
          headersCORS,
          toCookieHeaders(
            ~path="/",
            ~domain="localhost",
            ~secure=false,
            ~expireDate=
              Utils.getExn(
                "Ptime.of_float for expireDate",
                Ptime.of_float_s(999999999.0),
              ),
            "ds160_sid",
            Uuidm.to_string(session.id),
          ),
        );
      headersResponse;
    }
  )
  >>= (
    headersResponse => {
      print_endline("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
      print_endline(Httpaf.Headers.to_string(headersResponse));
      let queryString =
        Yojson.Basic.Util.(bodyJson |> member("query") |> to_string);
      let queryVariablesJson =
        try (
          Yojson.Basic.(
            (bodyString == "" ? "{}" : bodyString)
            |> from_string
            |> Util.member("variables")
            |> Util.to_assoc
          )
        ) {
        | Yojson.Basic.Util.Type_error(_, _) => []
        };

      let queryVariables = (
        queryVariablesJson :> list((string, Graphql_parser.const_value))
      );
      executeQuery(request.dbConn, queryString, queryVariables, req.headers)
      >>| (
        result =>
          switch (result) {
          | Ok(data) =>
            let jsonString = Yojson.Basic.to_string(data);
            (headersResponse, `OK, bofs(jsonString));
          | Error(errorData) =>
            let jsonString = Yojson.Basic.to_string(errorData);
            (Headers.empty, `OK, bofs(jsonString));
          }
      );
    }
  );
};

let allowCORSRequest = (_keys, _rest, request) => {
  let {req} = request;
  let headers = getCORSHeaders(req);
  (headers, `OK, bofs("")) |> Deferred.return;
};

let routes = [
  (`OPTIONS, "/dynamic", allowCORSRequest),
  (`POST, "/dynamic", dynamicQueryHandler(~readOnly=false)),
];

let notFoundHandler = () =>
  defer((
    contentTypeHeader("text/html"),
    `OK,
    bofs("We can't find that route, sorry"),
  ));

let notImplementedHandler = () =>
  defer((Httpaf.Headers.empty, `Not_implemented, bofs("")));

let dispatch =
  OneDispatch.create(routes, notFoundHandler, notImplementedHandler);

let handlers = (request: request) =>
  dispatch(request, request.req.meth, Uri.path(request.uri));

let drainRequestBody = (reqd, onEOF) => {
  let request = Reqd.request(reqd);
  let requestBody = Reqd.request_body(reqd);
  let _bodySize =
    switch (Httpaf.Request.body_length(request)) {
    | `Fixed(length) => length
    | `Chunked => raise(Failure("Chunked request bodies are not supported"))
    | `Error(_) => raise(Failure("Failure to read request body"))
    };
  let acc = ref("");
  let rec on_read = (buffer, ~off, ~len) => {
    acc := acc^ ++ Bigstring.to_string(~off, ~len, buffer);
    Body.schedule_read(requestBody, ~on_eof, ~on_read);
  }
  and on_eof = () => onEOF(acc^);
  Body.schedule_read(requestBody, ~on_eof, ~on_read);
};

let shouldUseGzip = reqd => {
  let encoding =
    Httpaf.Headers.get(Reqd.request(reqd).headers, "accept-encoding");
  switch (encoding) {
  | None => false
  | Some(encoding') =>
    Str.string_match(Str.regexp(".*gzip.*"), encoding', 0)
  };
};

let respond = (reqd, (headers, status, content)) => {
  let useGzip = shouldUseGzip(reqd);
  let bodyString =
    useGzip ?
      Ezgzip.compress(Bigstring.to_string(content)) :
      Bigstring.to_string(content);
  let headers =
    Httpaf.Headers.add_list(
      headers,
      List.concat([
        [("Content-Length", string_of_int(String.length(bodyString)))],
        useGzip ? [("Content-Encoding", "gzip")] : [],
      ]),
    );

  let requestBody = Reqd.request_body(reqd);
  let responseBody =
    Reqd.respond_with_streaming(reqd, Response.create(~headers, status));
  Body.write_string(responseBody, bodyString);
  Body.close(requestBody);
  Body.close(responseBody);
};

let routes = (dbConn, reqId, reqd) => {
  let finished = Ivar.create();
  let req = Reqd.request(reqd);
  let uri = Uri.of_string(req.target);
  let quietRoute = false;
  let origin = "hardcoded";
  let start = Unix.gettimeofday();

  /* TODO: Logging in the code that calls the handler */
  quietRoute ?
    () :
    OneLog.infof(
      "Request start method=%s path=%s req_id=%s",
      Method.to_string(req.meth),
      req.target,
      Uuidm.to_string(reqId),
    );

  let onBodyRead = bodyString => {
    let request = {dbConn, reqId, req, origin, bodyString, uri};
    try_with(
      ~extract_exn=false,
      ~name="webRoutesHttpaf",
      ~rest=
        `Call(
          exn_ =>
            OneLog.infof(
              "Unexpected and non-understood exception raised after route result was returned: %s",
              Core.Exn.to_string(exn_),
            ),
        ),
      ~run=`Now,
      () =>
      handlers(request) >>| (response => respond(reqd, response))
    )
    >>= (
      result =>
        switch (result) {
        | Ok () => Deferred.return()
        | Error(exn_) =>
          raise(Failure("Bad error: " ++ Core.Exn.to_string(exn_)))
        }
    )
    >>| (() => Async.Ivar.fill(finished, ()))
    |> ignore;
  };
  drainRequestBody(reqd, onBodyRead);
  Ivar.read(finished)
  >>| (
    () =>
      quietRoute ?
        () :
        OneLog.infof(
          "Request end method=%s path=%s req_id=%s req_ms=%0.2f",
          Method.to_string(req.meth),
          Uri.path(uri),
          Uuidm.to_string(reqId),
          (Unix.gettimeofday() -. start) *. 1000.0,
        )
  );
};
/*
 let username = "yuki";
 let password = "123";

 let userDBResults =
 DsUserModel.byUsernameAndPassword(connPool, username, password);

 userDBResults
 >>= (
 result => (
 {
 let ctx = DsSchema.{db: connPool, user: result, sessionId};
 Deferred.return(ctx);
 }:
 Deferred.t(DsSchema.gqlContext)
 )
 )*/
