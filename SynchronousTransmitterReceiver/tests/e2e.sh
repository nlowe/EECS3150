#!/usr/bin/env bash

if (( $# != 4 )); then
    echo "Syntax: ./e2e.sh <path to client> <path to server> <len> <-ec|-eh>"
    exit 1
fi

CLIENT=$1
SERVER=$2
LEN=$3
CORRECTION_MODE=$4

echo "Using client at ${CLIENT}"
echo "Using server at ${SERVER}"
echo "Testing input with length ${LEN}"
echo "Using error-correction mode ${CORRECTION_MODE}"

DATA=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w ${LEN} | head -n 1)

SERVER_OUTPUT=$(mktemp)
on_exit() {
    rm -f "${SERVER_OUTPUT}"
}
trap on_exit EXIT

${SERVER} ${CORRECTION_MODE} 5555 >"${SERVER_OUTPUT}" &
SERVER_PID=$!
DECODED=$(echo -n ${DATA} | ${CLIENT} ${CORRECTION_MODE} localhost 5555 - 2>/dev/null)
wait ${SERVER_PID}

RECEIVED=$(cat "${SERVER_OUTPUT}")

if [[ "${DATA}" != "${RECEIVED}" ]]; then
    echo "Failure at data length ${LEN}: Expected '${DATA}' but got '${DECODED}'" 1>&2
    echo "Got '${DECODED}' from the client"
    exit 1
fi
