#!/usr/bin/env bash

if (( $# != 1 )); then
    echo "Syntax: ./parity.sh <path to client>"
fi

CLIENT=$1

RESULT=$(echo -n "01010101" | ${CLIENT} -)
RET=$?

if (( $RET != 2 )); then
    echo "Expected exit code 2 but got ${RET}"
    exit 1
fi

if [[ "${RESULT}" != *""* ]]; then
    echo "Expected parity check failure but got: ${RESULT}"
    exit 1
fi
