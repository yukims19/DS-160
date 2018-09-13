#!/usr/bin/env bash
set -e;
set -x;

declare -a deps=(
    "async.v0.10.0"
    "async_ssl.v0.10.0"
    "atdgen.1.12.0"
    "cohttp-async.1.0.2"
    "crunch.2.1.0"
    "cryptokit.1.12"
    "dispatch.0.4.0"
    "ezxmlm.1.0.2"
    "ezgzip.0.2.1"
    "graphql.0.7.0"
    "graphql-async.0.1.0"
    "httpaf-async.0.2.0"
    "lambdasoup.0.6.3"
    "nocrypto.0.5.4"
    "ocurl.0.8.1"
    "postgresql.4.3.0"
    "ppx_deriving.4.2.1"
    "ppx_deriving_yojson.3.1"
    "ptime.0.8.3"
    "reason.3.2.0"
    "rresult.0.5.0"
    "safepass.2.0"
    "seq.0.1"
    "uri.1.9.6"
    "uuidm.0.9.6"
    "uutf.1.0.1"
)

# !!!
# Make sure to also update the docker container in .circleci/config.yml
OCAML_VERSION=4.06.1
OPAM_DEPS_STRING="${deps[@]}"
OPAM_SWITCH=ds160_ocaml_4_06_1

if [ -z ${CI+x} ]; then
    opam switch set --alias $OCAML_VERSION $OPAM_SWITCH
fi

eval `opam config env`

# opam update -y

opam install -y $OPAM_DEPS_STRING
