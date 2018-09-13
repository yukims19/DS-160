open Core;

open Async;

let spec =
  Command.Spec.(
    empty
    +> flag(
         ~doc="int Source port to listen on",
         "-l",
         optional_with_default(8082, int),
       )
  );

let () =
  Command.async_spec(~summary="Start DS160", spec, Startup.start)
  |> Command.run;
