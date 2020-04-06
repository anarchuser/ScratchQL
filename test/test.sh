#!/bin/bash
export GLOG_stderrtreshold="$1"
export GLOG_minloglevel="$1"
export SCRATCHQL_ROOT=$PWD
cmake --build cmake-build-debug --target Test
result=$?
if test $result -ne 0
then echo "Building failed!"
exit $result
fi
./cmake-build-debug/Test
