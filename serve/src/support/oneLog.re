open Core;

open Async;

let debugf = Log.Global.debug;

let infof = Log.Global.info;

let errorf = Log.Global.error;

let fatalf = fmt =>
  ksprintf(msg => Log.Global.string("MUSTFIX " ++ msg), fmt);

let debug = debugf("%s");

let info = infof("%s");

let error = errorf("%s");

let fatal = fatalf("%s");

let logPsqlResult = (e, status) => {
  let isError =
    String.length(e) > 0
    || Postgresql.(
         switch (status) {
         | Bad_response
         | Nonfatal_error
         | Fatal_error => true
         | _ => false
         }
       );
  (isError ? errorf : infof)(
    "%s :=> %s",
    e,
    Postgresql.(
      switch (status) {
      | Command_ok => "Success"
      | Empty_query => "Empty_query"
      | Tuples_ok => "Tuples_ok"
      | Copy_out => "Copy_out"
      | Copy_in => "Copy_in"
      | Bad_response => "Bad_response"
      | Nonfatal_error => "Nonfatal_error"
      | Fatal_error => "Fatal_error"
      | Copy_both => "Copy_both"
      | Single_tuple => "Single_tuple"
      }
    ),
  );
};

let setup = () => {
  let rotatingFile =
    Log.Output.rotating_file(
      `Text,
      ~basename="onegraph",
      Log.Rotation.create(
        ~size=Byte_units.create(`Megabytes, 100.0),
        ~keep=`At_least(3),
        ~naming_scheme=`Numbered,
        (),
      ),
    );
  Log.Global.set_output(
    List.concat([
      [rotatingFile],
      OneConfig.isDev ? [Log.Output.stdout()] : [],
    ]),
  );
  Log.Global.set_level(OneConfig.isDebug ? `Debug : `Info);
  info("Logger is set up");
  debug("Logging at debug level");
};
