#!/usr/bin/env bash

set -e
set -x

SCRIPT_PATH=$(dirname "$0")
cd "${SCRIPT_PATH}/.."

rm gomigrate
