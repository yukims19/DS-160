let getEnv = name =>
  try (Sys.getenv(name)) {
  | Not_found => raise(Invalid_argument(name ++ " not present in env vars"))
  };

let getEnvOpt = name =>
  try (Some(Sys.getenv(name))) {
  | Not_found => None
  };

let uuidOfString = s =>
  switch (Uuidm.of_string(s)) {
  | Some(u) => u
  | None => raise(Failure("invalid uuid " ++ s))
  };

let version = "placeholder";

let dsAppDomain = "ds160.com";

let isProd = getEnv("DS_ENV") == "prod";

let isDev = getEnv("DS_ENV") == "dev";

let isDebug =
  switch (getEnvOpt("DS_DEBUG")) {
  | Some(debug) => bool_of_string(debug)
  | None => true
  };

let randomSeedInts = [|int_of_float(Unix.time()), 0, 99, 2, (-342323)|];

let randomSeed = Random.State.make(randomSeedInts);

let connPoolSize = 5;

let dbVarName = "DS_DB_" ++ (isProd ? "PROD" : "LOCAL");

let serverPort = isProd ? 443 : int_of_string(getEnv("DS_PORT"));

let protocol = serverPort == 443 ? "https" : "https";

let hostname = isProd ? "serve.dsgraph.com" : "serve.dsgraph.io";

let serverUri =
  Uri.with_port(
    Uri.make(~scheme=protocol, ~host=hostname, ()),
    serverPort == 443 ? None : None,
  );

let encryptionKey = getEnv("DS_AES_KEY");

let errorReportEmails = "errors@dsgraph.com";

let reportErrors = isProd;

let showErrors = !isProd;

let siteTitle = "DsGraph " ++ (isProd ? "Prod" : "Dev");

type details = {
  isProd: bool,
  randomSeedInts: array(int),
  /* randomSeed: Random.State.t, */
  dbVarName: string,
  protocol: string,
  errorReportEmails: string,
  reportErrors: bool,
  showErrors: bool,
  siteTitle: string,
  dsAppDomain: string,
  version: string,
  serverUri: Uri.t,
};

let details = {
  isProd,
  randomSeedInts,
  dbVarName,
  protocol,
  errorReportEmails,
  reportErrors,
  showErrors,
  siteTitle,
  dsAppDomain,
  version,
  serverUri,
};

let serverUriString = Uri.to_string(serverUri);

[@deriving yojson]
type reportableDetails = {
  isProd: bool,
  randomSeedInts: array(int),
  dbVarName: string,
  protocol: string,
  errorReportEmails: string,
  reportErrors: bool,
  showErrors: bool,
  siteTitle: string,
  dsAppDomain: string,
  version: string,
  serverUri: string,
};

let reportableDetails: reportableDetails = {
  isProd,
  randomSeedInts,
  dbVarName,
  protocol,
  errorReportEmails,
  reportErrors,
  showErrors,
  siteTitle,
  dsAppDomain,
  version,
  serverUri: serverUriString,
};
