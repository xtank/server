#!/bin/bash
cd `dirname $0`
pkill -9 proxy
#rm -rf ../log/*
sleep 1
./bin/proxy ./etc/bench.lua
