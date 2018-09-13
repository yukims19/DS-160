#!/usr/bin/env sh

set -e

eval `opam config env`

if [ -z ${CI+x} ]
then
    EXTENSION="bc"
else
    EXTENSION="exe"
fi

SCRIPT_PATH=$(dirname "$0")

cd "${SCRIPT_PATH}/.."

dune build "src/ds160.${EXTENSION}"
