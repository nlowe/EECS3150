#!/usr/bin/env bash

if (( $# != 3 )); then
    echo "Syntax: ./e2e.sh <path to client> <path to server> <len>"
fi

CLIENT=$1
SERVER=$2
LEN=$3

echo "Using client at ${CLIENT}"
echo "Using server at ${SERVER}"
echo "Testing input with length ${LEN}"

DATA=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w ${LEN} | head -n 1)

ENCODED=$(echo -n ${DATA} | ${SERVER} -)
DECODED=$(echo -n ${ENCODED} | ${CLIENT} -)

if [[ "${DATA}" != "${DECODED}" ]]; then
    echo "Failure at data length ${LEN}: Expected '${DATA}' but got '${DECODED}'" 1>&2
    exit 1
fi
