#!/bin/bash

echo "gen_live_documentation.sh as part of everything"
./gen_live_documentation.sh

# TODO: there is a chance that we can simply remove this invocation? let's try once we fix https://github.com/rusefi/rusefi/issues/7604
echo "gen_config_default.sh as part of everything"
./gen_config_default.sh
