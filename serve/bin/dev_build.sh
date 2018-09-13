set -x
set -e

eval $(opam config env --switch ds160_ocaml_4_06_1);

./bin/build.sh && env OCAMLRUNPARAM=b DS_ENV=dev DS_DB_LOCAL="postgres://localhost:5432/ds_local?user=postgres&password=" DS_PORT=443 ./_build/default/src/ds160.bc
