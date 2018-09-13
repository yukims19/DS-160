let reportError = (~additionalContext=?, exn, request) => {
  let error = Core.Exn.to_string(exn);
  let req_text = Format.asprintf("%a@.", Httpaf.Request.pp_hum, request);
  ignore(
    {
      let emails =
        Str.split(Str.regexp(","), OneConfig.(details.errorReportEmails))
        |> List.map(email => ("to", email));
      let body =
        Printf.sprintf(
          "%s\n\nRequest:\n\n%s\n\nAdditional Context:\n\n%s\n\nInstance Config details:%s",
          error,
          req_text,
          switch (additionalContext) {
          | None => "None"
          | Some(context) => Yojson.Basic.pretty_to_string(context)
          },
          Yojson.Safe.pretty_to_string @@
          OneConfig.reportableDetails_to_yojson(OneConfig.reportableDetails),
        );
      let params =
        List.append(
          emails,
          [
            ("from", "OneGraph Errors <errors@onegraph.com>"),
            (
              "subject",
              Printf.sprintf(
                "[%s] Exception: %s",
                OneConfig.siteTitle,
                error,
              ),
            ),
            ("text", body),
          ],
        );
      ();
    },
  );
};

let reportException = (~additionalContext=?, exn_, reqd) => {
  open Core;
  let status = `Internal_server_error;
  let error = Exn.to_string(exn_);
  OneLog.error(error);
  OneLog.infof("Reporting errors? %b", OneConfig.(details.reportErrors));
  ignore(
    OneConfig.(
      details.reportErrors ? reportError(~additionalContext?, exn_, reqd) : ()
    ),
  );
  let pageBody =
    OneConfig.(details.showErrors) ?
      error :
      "We hit an internal error and have been notified, sorry about that";
  (status, pageBody);
};
