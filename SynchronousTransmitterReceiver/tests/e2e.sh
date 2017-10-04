#!/bin/bash

if (( $# != 2 )); then
    echo "Syntax: ./e2e.sh <path to client> <path to server>"
fi

CLIENT=$1
SERVER=$2

echo "Using client at ${CLIENT}"
echo "Using server at ${SERVER}"

FAILURES=()
SIZES=(7 8 9 63 64 65 127 128 129 254 255 256)

for len in "${SIZES[@]}"; do
    DATA=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w ${len} | head -n 1)

    ENCODED=$(echo -n ${DATA} | ${SERVER} -)
    DECODED=$(echo -n ${ENCODED} | ${CLIENT} -)

    if [[ "${DATA}" != "${DECODED}" ]]; then
        echo "Failure at data length ${len}: Expected '${DATA}' but got '${DECODED}'" 1>&2
        FAILURES+=("${len}")
    fi
done

if (( ${#FAILURES[@]} > 0 )); then
    echo "There are test failures" 1>&2
    exit 1
fi
