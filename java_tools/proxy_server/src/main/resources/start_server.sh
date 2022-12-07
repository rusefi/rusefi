#!/bin/bash
mkdir log
./run_server.sh > log/proxy.stdout 2>log/proxy.stderr &
