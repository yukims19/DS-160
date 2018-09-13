exception DbFailure(string);

type preparedQueryName = string;

type operationStats = {
  info: string,
  opSendQueryStart: float,
  opFetchQueryStart: float,
  opFinish: float,
  resultStatus: Postgresql.result_status,
};

type thunkResult = {
  operationStats,
  result: Postgresql.result,
};

type thunkResults = list(thunkResult);

type stats = {
  info: string,
  txId: string,
  enqueueStart: float,
  runStart: float,
  runFinish: float,
  thunkResults,
};

type preparedStatement = {
  name: string,
  statement: string,
};

type executeStatement = {
  statement: preparedStatement,
  params: array(string),
};

type recognizedFieldType =
  | BOOL
  | BYTEA
  | CHAR
  | NAME
  | INT8
  | INT2
  | INT2VECTOR
  | INT4
  | REGPROC
  | TEXT
  | OID
  | TID
  | XID
  | CID
  | OIDVECTOR
  | JSON
  | POINT
  | LSEG
  | PATH
  | BOX
  | POLYGON
  | LINE
  | FLOAT4
  | FLOAT8
  | ABSTIME
  | RELTIME
  | TINTERVAL
  | UNKNOWN
  | CIRCLE
  | CASH
  | MACADDR
  | INET
  | CIDR
  | ACLITEM
  | BPCHAR
  | VARCHAR
  | DATE
  | TIME
  | TIMESTAMP
  | TIMESTAMPTZ
  | INTERVAL
  | TIMETZ
  | BIT
  | VARBIT
  | NUMERIC
  | REFCURSOR
  | REGPROCEDURE
  | REGOPER
  | REGOPERATOR
  | REGCLASS
  | REGTYPE
  | RECORD
  | CSTRING
  | ANY
  | ANYARRAY
  | VOID
  | TRIGGER
  | LANGUAGE_HANDLER
  | INTERNAL
  | OPAQUE
  | ANYELEMENT
  | JSONB
  | UUID;

let toOid = fieldType =>
  switch (fieldType) {
  | BOOL => 16
  | BYTEA => 17
  | CHAR => 18
  | NAME => 19
  | INT8 => 20
  | INT2 => 21
  | INT2VECTOR => 22
  | INT4 => 23
  | REGPROC => 24
  | TEXT => 25
  | OID => 26
  | TID => 27
  | XID => 28
  | CID => 29
  | OIDVECTOR => 30
  | JSON => 114
  | POINT => 600
  | LSEG => 601
  | PATH => 602
  | BOX => 603
  | POLYGON => 604
  | LINE => 628
  | FLOAT4 => 700
  | FLOAT8 => 701
  | ABSTIME => 702
  | RELTIME => 703
  | TINTERVAL => 704
  | UNKNOWN => 705
  | CIRCLE => 718
  | CASH => 790
  | MACADDR => 829
  | INET => 869
  | CIDR => 650
  | ACLITEM => 1033
  | BPCHAR => 1042
  | VARCHAR => 1043
  | DATE => 1082
  | TIME => 1083
  | TIMESTAMP => 1114
  | TIMESTAMPTZ => 1184
  | INTERVAL => 1186
  | TIMETZ => 1266
  | BIT => 1560
  | VARBIT => 1562
  | NUMERIC => 1700
  | REFCURSOR => 1790
  | REGPROCEDURE => 2202
  | REGOPER => 2203
  | REGOPERATOR => 2204
  | REGCLASS => 2205
  | REGTYPE => 2206
  | RECORD => 2249
  | CSTRING => 2275
  | ANY => 2276
  | ANYARRAY => 2277
  | VOID => 2278
  | TRIGGER => 2279
  | LANGUAGE_HANDLER => 2280
  | INTERNAL => 2281
  | OPAQUE => 2282
  | ANYELEMENT => 2283
  | JSONB => 3802
  | UUID => 2950
  };

type fieldType =
  | Recognized(recognizedFieldType)
  | Unrecognized(int);

let ofOid = fieldOid =>
  switch (fieldOid) {
  | 16 => Recognized(BOOL)
  | 17 => Recognized(BYTEA)
  | 18 => Recognized(CHAR)
  | 19 => Recognized(NAME)
  | 20 => Recognized(INT8)
  | 21 => Recognized(INT2)
  | 22 => Recognized(INT2VECTOR)
  | 23 => Recognized(INT4)
  | 24 => Recognized(REGPROC)
  | 25 => Recognized(TEXT)
  | 26 => Recognized(OID)
  | 27 => Recognized(TID)
  | 28 => Recognized(XID)
  | 29 => Recognized(CID)
  | 30 => Recognized(OIDVECTOR)
  | 114 => Recognized(JSON)
  | 600 => Recognized(POINT)
  | 601 => Recognized(LSEG)
  | 602 => Recognized(PATH)
  | 603 => Recognized(BOX)
  | 604 => Recognized(POLYGON)
  | 628 => Recognized(LINE)
  | 700 => Recognized(FLOAT4)
  | 701 => Recognized(FLOAT8)
  | 702 => Recognized(ABSTIME)
  | 703 => Recognized(RELTIME)
  | 704 => Recognized(TINTERVAL)
  | 705 => Recognized(UNKNOWN)
  | 718 => Recognized(CIRCLE)
  | 790 => Recognized(CASH)
  | 829 => Recognized(MACADDR)
  | 869 => Recognized(INET)
  | 650 => Recognized(CIDR)
  | 1033 => Recognized(ACLITEM)
  | 1042 => Recognized(BPCHAR)
  | 1043 => Recognized(VARCHAR)
  | 1082 => Recognized(DATE)
  | 1083 => Recognized(TIME)
  | 1114 => Recognized(TIMESTAMP)
  | 1184 => Recognized(TIMESTAMPTZ)
  | 1186 => Recognized(INTERVAL)
  | 1266 => Recognized(TIMETZ)
  | 1560 => Recognized(BIT)
  | 1562 => Recognized(VARBIT)
  | 1700 => Recognized(NUMERIC)
  | 1790 => Recognized(REFCURSOR)
  | 2202 => Recognized(REGPROCEDURE)
  | 2203 => Recognized(REGOPER)
  | 2204 => Recognized(REGOPERATOR)
  | 2205 => Recognized(REGCLASS)
  | 2206 => Recognized(REGTYPE)
  | 2249 => Recognized(RECORD)
  | 2275 => Recognized(CSTRING)
  | 2276 => Recognized(ANY)
  | 2277 => Recognized(ANYARRAY)
  | 2278 => Recognized(VOID)
  | 2279 => Recognized(TRIGGER)
  | 2280 => Recognized(LANGUAGE_HANDLER)
  | 2281 => Recognized(INTERNAL)
  | 2282 => Recognized(OPAQUE)
  | 2283 => Recognized(ANYELEMENT)
  | 2950 => Recognized(UUID)
  | 3802 => Recognized(JSONB)
  | oid => Unrecognized(oid)
  };

let toString = fieldType =>
  switch (fieldType) {
  | Recognized(BOOL) => "BOOL"
  | Recognized(BYTEA) => "BYTEA"
  | Recognized(CHAR) => "CHAR"
  | Recognized(NAME) => "NAME"
  | Recognized(INT8) => "INT8"
  | Recognized(INT2) => "INT2"
  | Recognized(INT2VECTOR) => "INT2VECTOR"
  | Recognized(INT4) => "INT4"
  | Recognized(REGPROC) => "REGPROC"
  | Recognized(TEXT) => "TEXT"
  | Recognized(OID) => "OID"
  | Recognized(TID) => "TID"
  | Recognized(XID) => "XID"
  | Recognized(CID) => "CID"
  | Recognized(OIDVECTOR) => "OIDVECTOR"
  | Recognized(JSON) => "JSON"
  | Recognized(POINT) => "POINT"
  | Recognized(LSEG) => "LSEG"
  | Recognized(PATH) => "PATH"
  | Recognized(BOX) => "BOX"
  | Recognized(POLYGON) => "POLYGON"
  | Recognized(LINE) => "LINE"
  | Recognized(FLOAT4) => "FLOAT4"
  | Recognized(FLOAT8) => "FLOAT8"
  | Recognized(ABSTIME) => "ABSTIME"
  | Recognized(RELTIME) => "RELTIME"
  | Recognized(TINTERVAL) => "TINTERVAL"
  | Recognized(UNKNOWN) => "UNKNOWN"
  | Recognized(CIRCLE) => "CIRCLE"
  | Recognized(CASH) => "CASH"
  | Recognized(MACADDR) => "MACADDR"
  | Recognized(INET) => "INET"
  | Recognized(CIDR) => "CIDR"
  | Recognized(ACLITEM) => "ACLITEM"
  | Recognized(BPCHAR) => "BPCHAR"
  | Recognized(VARCHAR) => "VARCHAR"
  | Recognized(DATE) => "DATE"
  | Recognized(TIME) => "TIME"
  | Recognized(TIMESTAMP) => "TIMESTAMP"
  | Recognized(TIMESTAMPTZ) => "TIMESTAMPTZ"
  | Recognized(INTERVAL) => "INTERVAL"
  | Recognized(TIMETZ) => "TIMETZ"
  | Recognized(BIT) => "BIT"
  | Recognized(VARBIT) => "VARBIT"
  | Recognized(NUMERIC) => "NUMERIC"
  | Recognized(REFCURSOR) => "REFCURSOR"
  | Recognized(REGPROCEDURE) => "REGPROCEDURE"
  | Recognized(REGOPER) => "REGOPER"
  | Recognized(REGOPERATOR) => "REGOPERATOR"
  | Recognized(REGCLASS) => "REGCLASS"
  | Recognized(REGTYPE) => "REGTYPE"
  | Recognized(RECORD) => "RECORD"
  | Recognized(CSTRING) => "CSTRING"
  | Recognized(ANY) => "ANY"
  | Recognized(ANYARRAY) => "ANYARRAY"
  | Recognized(VOID) => "VOID"
  | Recognized(TRIGGER) => "TRIGGER"
  | Recognized(LANGUAGE_HANDLER) => "LANGUAGE_HANDLER"
  | Recognized(INTERNAL) => "INTERNAL"
  | Recognized(OPAQUE) => "OPAQUE"
  | Recognized(ANYELEMENT) => "ANYELEMENT"
  | Recognized(JSONB) => "JSONB"
  | Recognized(UUID) => "UUID"
  | Unrecognized(oid) => string_of_int(oid)
  };

open Postgresql;

let stringOfStatus = status =>
  switch (status) {
  | Command_ok => "Command_ok"
  | Empty_query => "Empty_query"
  | Tuples_ok => "Tuples_ok"
  | Copy_out => "Copy_out"
  | Copy_in => "Copy_in"
  | Bad_response => "Bad_response"
  | Nonfatal_error => "Nonfatal_error"
  | Fatal_error => "Fatal_error"
  | Copy_both => "Copy_both"
  | Single_tuple => "Single_tuple"
  };

let resultStatus = result => stringOfStatus(result#status);

/* Unregisters the fd from Async's tracking (without closing the underlying fd).
   We'll likely register the same fd again, but this prevents us from holding
   on to an fd that postgres closes */
let releaseFd = fd =>
  Async.(
    Unix.Fd.close(
      ~file_descriptor_handling=Unix.Fd.Do_not_close_file_descriptor,
      fd,
    )
  );

let rec waitForResult = (conn: Postgresql.connection) => {
  open Async;
  conn#consume_input;
  if (! conn#is_busy) {
    Deferred.return();
  } else {
    let fd =
      Unix.Fd.create(
        Unix.Fd.Kind.Socket(`Active),
        Core.Unix.File_descr.of_int(conn#socket),
        Core.Info.of_string("conn#socket"),
      );
    let rec waitForReady = () =>
      Unix.Fd.ready_to(fd, `Read)
      >>= (
        fun
        | `Bad_fd => {
            OneLog.fatalf("Bad_fd waiting for Postgres fd");
            raise(Failure("Database error"));
          }
        | `Closed => {
            OneLog.fatalf("fd closed while waiting to consume input");
            raise(Failure("Database error"));
          }
        | `Ready => {
            conn#consume_input;
            conn#is_busy ? waitForReady() : releaseFd(fd);
          }
      );
    waitForReady();
  };
};

open Core;

open Async;

let fetchResult = (c: Postgresql.connection) =>
  waitForResult(c) >>| (() => c#get_result);

let fetchSingleResult = (c: Postgresql.connection) =>
  fetchResult(c)
  >>= (
    fun
    | None => {
        OneLog.error("fetchSingleResult: no results waiting");
        raise(Failure("fetchSingleResult: no results waiting"));
      }
    | Some(r) =>
      fetchResult(c)
      >>| (
        fun
        | None => r
        | _ => {
            OneLog.error("fetchSingleResult: multiple results waiting");
            raise(Failure("fetchSingleResult: multiple results waiting"));
          }
      )
  );

let logDbStats = (stats: stats) => {
  List.iter(
    stats.thunkResults,
    ({operationStats, result}: thunkResult) => {
      OneLog.debugf(
        "dbOp %s, status=%s, opSendQueryMs=%.2f, opFetchResultMs=%.2f, opTotalMs=%.2f, txId=%s",
        operationStats.info,
        stringOfStatus(operationStats.resultStatus),
        operationStats.opFetchQueryStart -. operationStats.opSendQueryStart,
        operationStats.opFinish -. operationStats.opFetchQueryStart,
        operationStats.opFinish -. operationStats.opSendQueryStart,
        stats.txId,
      );
      try (
        switch (result#error) {
        | "" => ()
        | e =>
          OneLog.errorf(
            "error in operation %s, e=%s, txId=%s",
            operationStats.info,
            e,
            stats.txId,
          )
        }
      ) {
      | Error(err) =>
        OneLog.errorf(
          "PG error e=%s, txId=%s",
          Postgresql.string_of_error(err),
          stats.txId,
        )
      };
    },
  );
  OneLog.debugf(
    "db end %s, runQueuedMs=%.2f, runExecuteMs=%.2f, runTotalMs=%.2f, txId=%s",
    stats.info,
    stats.runStart -. stats.enqueueStart,
    stats.runFinish -. stats.runStart,
    stats.runFinish -. stats.enqueueStart,
    stats.txId,
  );
};

let runInPool = (~connPool: OneDb.connPool, ~info: string, ~thunk) => {
  let txId = Uuidm.to_string(Utils.genUuid());
  OneLog.debugf("db start %s, txId=%s", info, txId);
  let enqueueStart = Unix.gettimeofday();
  Throttle.enqueue(
    connPool,
    ((_connId, conn)) => {
      let runStart = Unix.gettimeofday();
      try_with(() => thunk(conn))
      >>| (
        fun
        | Ok(thunkResults) => {
            let stats = {
              txId,
              info,
              enqueueStart,
              runStart,
              runFinish: Unix.gettimeofday(),
              thunkResults,
            };
            logDbStats(stats);
            Ok(List.map(thunkResults, tr => tr.result));
          }
        | Error(e) => {
            OneLog.errorf(
              "db end in error, exn=%s, txId=%s",
              Core.Exn.to_string(e),
              txId,
            );
            Error(e);
          }
      );
    },
  );
};

let executeCommand =
    (~conn: Postgresql.connection, ~info: string, ~command)
    : Deferred.t(thunkResult) => {
  let sendQueryStart = Unix.gettimeofday();
  command(conn);
  let fetchResultStart = Unix.gettimeofday();
  fetchSingleResult(conn)
  >>| (
    value => {
      operationStats: {
        info,
        opSendQueryStart: sendQueryStart,
        opFetchQueryStart: fetchResultStart,
        opFinish: Unix.gettimeofday(),
        resultStatus: value#status,
      },
      result: value,
    }
  );
};

/* Executes statements in a transaction */
let sendMultiplePrepared =
    (
      connPool: OneDb.connPool,
      ~info: string,
      ~statements: list(executeStatement),
    ) =>
  runInPool(~connPool, ~info, ~thunk=conn =>
    executeCommand(~conn, ~info="begin", ~command=conn =>
      conn#send_query("BEGIN")
    )
    >>= (
      beginResult =>
        try_with(() =>
          Deferred.List.map(~how=`Sequential, statements, ~f=statement =>
            executeCommand(
              ~conn, ~info=statement.statement.name, ~command=conn =>
              conn#send_query_prepared(
                ~params=statement.params,
                statement.statement.name,
              )
            )
            >>| (
              res =>
                switch (res.result#error) {
                | "" => res
                | error => raise(Failure(error))
                }
            )
          )
        )
        >>= (
          fun
          | Error(exn) =>
            executeCommand(~conn, ~info="rollback", ~command=conn =>
              conn#send_query("ROLLBACK")
            )
            >>| (_res => raise(exn))
          | Ok(statementResults) =>
            executeCommand(~conn, ~info="commit", ~command=conn =>
              conn#send_query("COMMIT")
            )
            >>| (
              commitResult =>
                List.concat([
                  [beginResult],
                  statementResults,
                  [commitResult],
                ])
            )
        )
    )
  );

let runDbCommand = (~connPool, ~info, ~command) =>
  runInPool(~connPool, ~info, ~thunk=conn =>
    executeCommand(~conn, ~info, ~command) >>| (res => [res])
  )
  >>| (
    fun
    | Ok([res]) => res
    | Ok(_) => raise(Failure("expected single result from runDbCommand"))
    | Error(exn) => raise(exn)
  );

let sendPrepared =
    (
      connPool: OneDb.connPool,
      ~name: preparedQueryName,
      ~expectStatus=?,
      ~params: array(string),
      ~suppressLog=true,
      (),
    ) => {
  let resVar =
    runDbCommand(
      ~connPool,
      ~info=
        Printf.sprintf(
          "statement=%s%s",
          name,
          suppressLog ? "" : " " ++ Utils.stringOfArray(params),
        ),
      ~command=conn =>
      conn#send_query_prepared(~params, name)
    );
  switch (expectStatus) {
  | None => resVar
  | Some(status) =>
    resVar
    >>| (
      res => {
        /* Allows the caller to delegate basic expected result-status checking.
           If you're reasonably confident that a routine DB call should always
           have an expected status and anything else is an exceptional status,
           specify ~expectStatus */
        res#status == status ?
          () :
          OneBase.raiseErr(
            Printf.sprintf(
              "Exceptional DB result status, queryName=%s;params=%s;expected=%s;actual=%s;error=%s",
              name,
              String.concat(~sep=", ", Array.to_list(params)),
              stringOfStatus(status),
              stringOfStatus(res#status),
              res#error,
            ),
          );
        res;
      }
    )
  };
};

let sendQuery =
    (connPool: OneDb.connPool, ~suppressLog=false, ~params=[||], ~query, ()) =>
  runDbCommand(
    ~connPool,
    ~info=Printf.sprintf("query=%s", suppressLog ? "[suppressed]" : query),
    ~command=conn =>
    conn#send_query(~params, query)
  );

let prepareStatements = (conn, connId, statements: list(preparedStatement)) => {
  let enqueueStart = Unix.gettimeofday();
  let txId = Uuidm.to_string(Utils.genUuid());
  Deferred.List.map(~how=`Sequential, statements, ~f=({name, statement}) =>
    executeCommand(conn, "prepare " ++ name, conn =>
      conn#send_prepare(name, statement)
    )
    >>| (
      res =>
        switch (res.result#status) {
        | Command_ok => res
        | status =>
          OneLog.errorf(
            "Unexpected result=%s, error=%s",
            stringOfStatus(status),
            res.result#error,
          );
          raise(Failure(res.result#error));
        }
    )
  )
  >>| (
    thunkResults => {
      let stats = {
        txId,
        info: "prepare statements on " ++ string_of_int(connId),
        enqueueStart,
        runStart: enqueueStart,
        runFinish: Unix.gettimeofday(),
        thunkResults,
      };
      logDbStats(stats);
      List.map(thunkResults, tr => tr.result);
    }
  );
};

let ptimeOfDbTs = dbTs =>
  switch (Str.split(Str.regexp("[T]"), dbTs)) {
  | [pre, post] =>
    switch (Ptime.of_rfc3339(pre ++ " " ++ post ++ "+00:00")) {
    | Ok((t, _, _)) => t
    | Error(_) =>
      OneBase.raiseErr("invalid datetime for ptime: " ++ dbTs ++ "+00:00")
    }
  | _ =>
    switch (Ptime.of_rfc3339(dbTs ++ "+00:00")) {
    | Ok((t, _, _)) => t
    | Error(_) =>
      OneBase.raiseErr("invalid datetime for ptime: " ++ dbTs ++ "+00:00")
    }
  };

let dbTsOfPtime = ptime => Ptime.to_rfc3339(ptime);

let listParam = list =>
  Printf.sprintf("{%s}", String.concat(list, ~sep=","));

let optionalStringResult = str =>
  switch (str) {
  | "" => None
  | value => Some(value)
  };

let uuidResult = (fieldName, str) =>
  switch (Uuidm.of_string(str)) {
  | None =>
    raise(
      Failure(
        Printf.sprintf("Invalid UUID from DB (%s): %s", fieldName, str),
      ),
    )
  | Some(uuid) => uuid
  };

let intResult = str =>
  switch (str) {
  | "" => None
  | value => Some(int_of_string(value))
  };

let listResult = str =>
  switch (str) {
  | "" => []
  | _ =>
    /* Get rid of surrounding {}, then split on comma */
    String.sub(str, 1, String.length(str) - 2)
    |> Str.split(Str.regexp(","))
  };

let uuidListResult = (debugFieldName, str) =>
  listResult(str)
  |> List.map(~f=appId =>
       switch (Uuidm.of_string(appId)) {
       | None =>
         raise(
           Failure(
             Printf.sprintf(
               "Invalid UUID from DB (%s): %s",
               debugFieldName,
               appId,
             ),
           ),
         )

       | Some(uuid) => uuid
       }
     );

type psqlErrorString = string;

type psqlConstraint = {constraintName: string};

type betterError =
  | UniqueConstraintViolation(psqlErrorString, psqlConstraint)
  | Unknown(psqlErrorString);

let extractBetterError = psqlErrorString => {
  let uniqueConstraintRe =
    Re.Pcre.regexp(
      "duplicate key value violates unique constraint \"([^\"]+)\"",
    );
  /* UniqueConstraintViolation(psqlErrorString, {constraintName: (Re.Group.all( substrings)).(1)}) */
  switch (Re.all(uniqueConstraintRe, psqlErrorString)) {
  | [substrings] =>
    UniqueConstraintViolation(
      psqlErrorString,
      {constraintName: Re.Group.all(substrings)[0]},
    )
  | _ => Unknown(psqlErrorString)
  };
};

let stringOfBetterError = betterError =>
  switch (betterError) {
  | UniqueConstraintViolation(psqlErrorString, _psqlConstraint) => psqlErrorString
  | Unknown(psqlErrorString) => psqlErrorString
  };

/* https://www.postgresql.org/docs/9.5/static/datatype-boolean.html */
let boolOfPostgresString = s =>
  switch (s) {
  | "TRUE"
  | "t"
  | "true"
  | "y"
  | "yes"
  | "on"
  | "1" => true
  | "FALSE"
  | "f"
  | "false"
  | "n"
  | "no"
  | "off"
  | "0" => false
  | _ =>
    raise(
      Invalid_argument("invalid argument for boolOfPostgresString, " ++ s),
    )
  };

/* Example of more specific error handling we can do */
/* ------------------------------------------------- */
/* let error = */
/*   try res#error { */
/*   | Error(err) => "Some PG Error: " ++ Postgresql.string_of_error(err) */
/*   }; */
/* let error_field = */
/*   try (res#error_field(PG_DIAG_SEVERITY)) { */
/*   | Error(err) => "Some PG Error: " ++ Postgresql.string_of_error(err) */
/*   }; */
/* print_endline( */
/*   "Error type PG_DIAG_SEVERITY: " ++ res#error_field(PG_DIAG_SEVERITY) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_SEVERITY_NONLOCALIZED: " */
/*   ++ res#error_field(PG_DIAG_SEVERITY_NONLOCALIZED) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_SQLSTATE: " ++ res#error_field(PG_DIAG_SQLSTATE) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_MESSAGE_PRIMARY: " */
/*   ++ res#error_field(PG_DIAG_MESSAGE_PRIMARY) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_MESSAGE_DETAIL: " */
/*   ++ res#error_field(PG_DIAG_MESSAGE_DETAIL) */
/* ); */
/* let _ = */
/*   Postgresql.Error_code.condition_name_of_error_code( */
/*     res#error_field(PG_DIAG_SQLSTATE) */
/*   ); */
/* print_endline( */
/*   "\tCondition: " */
/*   ++ ( */
/*     switch res#error_field_condition_name { */
/*     | UNIQUE_VIOLATION => "Pretty unique, right?" */
/*     | _ => "What?" */
/*     } */
/*   ) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_MESSAGE_HINT: " */
/*   ++ res#error_field(PG_DIAG_MESSAGE_HINT) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_STATEMENT_POSITION: " */
/*   ++ res#error_field(PG_DIAG_STATEMENT_POSITION) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_INTERNAL_POSITION: " */
/*   ++ res#error_field(PG_DIAG_INTERNAL_POSITION) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_INTERNAL_QUERY: " */
/*   ++ res#error_field(PG_DIAG_INTERNAL_QUERY) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_CONTEXT: " ++ res#error_field(PG_DIAG_CONTEXT) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_SCHEMA_NAME: " ++ res#error_field(PG_DIAG_SCHEMA_NAME) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_TABLE_NAME: " ++ res#error_field(PG_DIAG_TABLE_NAME) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_COLUMN_NAME: " ++ res#error_field(PG_DIAG_COLUMN_NAME) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_DATATYPE_NAME: " */
/*   ++ res#error_field(PG_DIAG_DATATYPE_NAME) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_CONSTRAINT_NAME: " */
/*   ++ res#error_field(PG_DIAG_CONSTRAINT_NAME) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_SOURCE_FILE: " ++ res#error_field(PG_DIAG_SOURCE_FILE) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_SOURCE_LINE: " ++ res#error_field(PG_DIAG_SOURCE_LINE) */
/* ); */
/* print_endline( */
/*   "Error type PG_DIAG_SOURCE_FUNCTION: " */
/*   ++ res#error_field(PG_DIAG_SOURCE_FUNCTION) */
/* ); */
/* switch status { */
/* | Command_ok => () */
/* | _ => OneLog.logPsqlResult(error, status) */
/* }; */
/* switch status { */
/* | Tuples_ok => */
/*   let resultantId = */
/*     try ( */
/*       switch res#get_all[0] { */
/*       | [|id|] => */
/*         let id = */
/*           switch (Uuidm.of_string(id)) { */
/*           | None => failPublic("invalid uuid") */
/*           | Some(uuid) => uuid */
/*           }; */
/*         Result.Ok(id); */
/*       | _ => failPublic("invalid db tuple shape") */
/*       } */
/*     ) { */
/*     | _ => Error(InsertFailed) */
/*     }; */
/*   resultantId; */
/* | _ => Error(InsertFailed) */
/* }; */
