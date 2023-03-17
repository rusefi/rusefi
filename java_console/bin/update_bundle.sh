#!/bin/bash

cd console
java -jar fome_autoupdate.jar version

# https://github.com/rusefi/rusefi/issues/2601
chmod +x ../bin/*.sh
