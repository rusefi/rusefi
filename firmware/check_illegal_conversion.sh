#!/usr/bin/bash

# Trying to convert float -> long suggests you're probably trying to compute a
# time period in float, then add it to a timestamp to get some time in the future.
# If you do this naively, it'll try and do
# float offset;
# int64_t stamp;
# int64_t result = (int64_t)((float)stamp + offset);
# The resulting loss of precision is unacceptable.

set -euo pipefail

if grep "bl.*<__aeabi_f2lz>" build/fome.list; then
	echo "Illegal float-to-long conversion detected!"
	exit 1
fi
