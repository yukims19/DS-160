open Async;

module I = {
  type t = string;
  let compare = (x, y) => x == y ? 0 : 1;
  let witness = "";
};

let rec take = (b, e, l) =>
  switch (l) {
  | [] => l
  | [h, ...t] =>
    let tail =
      if (e == 0) {
        [];
      } else {
        take(b - 1, e - 1, t);
      };
    if (b > 0) {
      tail;
    } else {
      [h, ...tail];
    };
  };

let now = () => Int64.of_float(Unix.time());

let getExn = (name, opt) =>
  switch (opt) {
  | Some(v) => v
  | None =>
    raise(Invalid_argument("unwrapUnsafely called on None: " ++ name))
  };

let okExn = (name, result) =>
  switch (result) {
  | Ok(v) => v
  | Error(_err) =>
    raise(Invalid_argument("okExn called on Error: " ++ name))
  };

let rec distinct = lst =>
  switch (lst) {
  | [] => []
  | [h, ...t] => [h, ...distinct(List.filter(x => x != h, t))]
  };

let ffilter = (f: 'a => option('a), list): option('a) => {
  let result = ref(None);
  List.iter(
    item =>
      switch (result^) {
      | None =>
        switch (f(item)) {
        | None => ()
        | Some(value) => result := Some(value)
        }
      | Some(_) => ()
      },
    list,
  );
  result^;
};

/* Renamed from http://www.codecodex.com/wiki/Generate_a_random_password_or_random_string#OCaml */
let randomString = length => {
  let gen = () =>
    switch (Random.int(26 + 26 + 10)) {
    | n when n < 26 => int_of_char('a') + n
    | n when n < 26 + 26 => int_of_char('A') + n - 26
    | n => int_of_char('0') + n - 26 - 26
    };
  let gen = _ => String.make(1, char_of_int(gen()));
  String.concat("", Array.to_list(Array.init(length, gen)));
};

let formBodyToAssoc = body =>
  Str.split(Str.regexp("&"), body)
  |> List.map(s =>
       switch (Str.split(Str.regexp("="), s)) {
       | [key, value] => (key, value)
       | _ => failwith("bad formBodyToAssoc")
       }
     );

let formBodyOfFullAssoc = assoc =>
  assoc
  |> List.map(((key, values)) =>
       values
       |> List.map(value =>
            Printf.sprintf(
              "%s=%s",
              key,
              Uri.pct_encode(~component=`Query, value),
            )
          )
     )
  |> List.concat
  |> String.concat("&");

let formBodyOfSimpleAssoc = assoc =>
  assoc
  |> List.map(((key, value)) =>
       Printf.sprintf("%s=%s", key, Uri.pct_encode(~component=`Query, value))
     )
  |> String.concat("&");

let rec subList = (b, e, l) =>
  switch (l) {
  | [] => failwith("subList")
  | [h, ...t] =>
    let tail =
      if (e == 0) {
        [];
      } else {
        subList(b - 1, e - 1, t);
      };
    if (b > 0) {
      tail;
    } else {
      [h, ...tail];
    };
  };

let optionCoerce = (converter, optionThing) =>
  switch (optionThing) {
  | None => None
  | Some(thing) => Some(converter(thing))
  };

let default = (default: 'a, optional: option('a)) =>
  switch (optional) {
  | None => default
  | Some(value) => value
  };

let uriAddQueryParams = (queryParams, uri) =>
  List.fold_left(
    (run, next) => Uri.add_query_param(run, next),
    uri,
    queryParams,
  );

let ptimeNow = () =>
  getExn("Ptime.of_float_s", Ptime.of_float_s(Unix.gettimeofday()));

/* Produces date in rfc 7231 format, suitable for use as a Date in an http header */
let headerDateOfPtime = (t: Ptime.t): string =>
  Core.Unix.strftime(
    Unix.gmtime(Ptime.to_float_s(t)),
    "%a, %d %b %Y %H:%M:%S",
  )
  ++ " GMT";

let ptimeFromExpiresIn = (expiresInSeconds: int): Ptime.t => {
  let span = Ptime.Span.of_int_s(expiresInSeconds);
  let now = ptimeNow();
  getExn("getExpiresPtime", Ptime.add_span(now, span));
};

let genUuid = () => Uuidm.v4_gen(OneConfig.randomSeed, ());

let stringOfArray = (arr: array('a)) =>
  Printf.sprintf(
    "[|%s|]",
    Array.fold_left(
      (acc, item) => acc ++ (acc == "" ? "" : ", ") ++ item,
      "",
      arr,
    ),
  );

let stringOfList = (l: list('a)) =>
  Printf.sprintf(
    "[%s]",
    List.fold_left(
      (acc, item) => acc ++ (acc == "" ? "" : ", ") ++ item,
      "",
      l,
    ),
  );

let stringOfUuidList = l =>
  stringOfList(List.map(x => Uuidm.to_string(x), l));

let coHttpHeadersOfHttpafHeaders = httpAfHeaders => {
  let headers = ref([]);
  Httpaf.Headers.iter(
    ~f=(name, value) => headers := List.append(headers^, [(name, value)]),
    httpAfHeaders,
  );
  Cohttp.Header.of_list(headers^);
};

type jwtToken = {
  header: string,
  payload: string,
  signature: string,
};

let b64UrlDecode = s => B64.decode(~alphabet=B64.uri_safe_alphabet, s);

let decodeJwtToken = (token: string) =>
  switch (Str.split(Str.regexp("\\."), token)) {
  | [header, payload, signature] =>
    Ok({
      header: b64UrlDecode(header),
      payload: b64UrlDecode(payload),
      signature: b64UrlDecode(signature),
    })
  | _ => Error(`InvalidJWTToken)
  };

let camelCase = (str: string): string =>
  switch (str) {
  | "" => ""
  | str =>
    let cased =
      Re.replace(
        ~all=true,
        Re.Pcre.regexp("[-_ ]([A-Za-z0-9])"),
        ~f=sub => String.capitalize_ascii(Re.Group.get(sub, 1)),
        str,
      );
    String.lowercase_ascii(String.sub(cased, 0, 1))
    ++ String.sub(cased, 1, String.length(cased) - 1);
  };

let isNone = (a: option('a)): bool =>
  switch (a) {
  | None => true
  | Some(_) => false
  };

/* Removes [] around variants converted to yojson, ["ASC"] => ASC */
let stringOfYojsonVariant = s =>
  s
  |> Yojson.Safe.to_string
  |> Core.String.drop_prefix(_, 2)
  |> Core.String.drop_suffix(_, 2);

let yojsonVariantOfString = s =>
  Printf.sprintf("[\"%s\"]", s) |> Yojson.Safe.from_string;

let hdOpt = l =>
  try (Some(List.hd(l))) {
  | _ => None
  };
