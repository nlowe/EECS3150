#!/bin/bash
set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

rm -rf "${DIR}/build" > /dev/null
mkdir "${DIR}/build"
cmake -H. -B"${DIR}/build" && make -C "${DIR}/build" && CTEST_OUTPUT_ON_FAILURE=1 make -C "${DIR}/build" test
