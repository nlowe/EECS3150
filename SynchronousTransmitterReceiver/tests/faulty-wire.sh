#!/usr/bin/env bash

if (( $# != 1 )); then
    echo "Syntax: ./faulty-wire.sh <path to faulty-wire>"
    exit 1
fi

FAULTY_WIRE=$1

OUT0=$(echo -n "1" | ${FAULTY_WIRE} - 1)
OUT1=$(echo -n "0" | ${FAULTY_WIRE} - 1)

FAILED=0

if [[ "${OUT0}" != "0" ]]; then
    echo "Failed to flip 1. Expected '0' but got '${OUT0}'"
    FAILED=1
fi

if [[ "${OUT1}" != "1" ]]; then
    echo "Failed to flip 0. Expected '1' but got '${OUT1}'"
    FAILED=1
fi

if (( $FAILED > 0 )); then
    echo "There are test failures"
    exit 1
fi
