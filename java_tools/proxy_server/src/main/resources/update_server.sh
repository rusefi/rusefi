#!/bin/bash
echo Stopping...
./stop_server.sh

echo Downloading...
rm -rf rusefi_server.jar
wget https://rusefi.com/build_server/autoupdate/rusefi_server.jar

echo Starting...
./start_server.sh
