#!/bin/bash

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:../bin

ps aux | grep "./online ../conf/bench.conf" | grep -v grep | awk '{print $2}' | xargs -I^ kill -9 -^

case "$1" in
    "c" | "C" | "clean" | "Clean" ) ./clean_log ; rm -rf core.*;;
    * ) ;;
esac

cd ../bin ; ./online ../conf/bench.conf

