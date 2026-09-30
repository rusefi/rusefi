#!/usr/bin/bash

# Trying to convert float -> long suggests you're probably trying to compute a
# time period in float, then add it to a timestamp to get some time in the future.
# If you do this naively, it'll try and do
# float offset;
# int64_t stamp;
# int64_t result = (int64_t)((float)stamp + offset);
# The resulting loss of precision is unacceptable.

set -euo pipefail

# CI bundle builds (build_both_bundles) only produce build/rusefi.list for
# INCLUDE_ELF=yes boards, and a missing file used to make these checks silently
# vacuous (grep's exit 2 inside 'if' reads as "no match"). Generate the listing
# from the elf instead of checking nothing; fail loudly if even the elf is absent.
if [ ! -f build/rusefi.list ]; then
	arm-none-eabi-objdump -d build/rusefi.elf > build/rusefi.list
fi

if grep "bl.*<__aeabi_f2lz>" build/rusefi.list; then
	echo "Illegal float-to-long conversion detected!"
	exit 1
fi

# Double-precision math is prohibited: F4 has a single-precision FPU, so doubles
# become slow soft-float libcalls; F7/H7 have a double-precision FPU, so doubles
# silently "work" but waste cycles and hide precision bugs vs the F4 build.
# This is a link-time check on purpose: -Werror=double-promotion cannot be used
# because it also fires on C varargs default promotion, i.e. every legal
# `efiPrintf("%f", someFloat)` - see the note in the firmware Makefile.
#
# Note we deliberately do NOT ban conversions INTO double (__aeabi_f2d/i2d,
# vcvt.f64.f32): C varargs default promotion converts every float passed to a
# printf-style function to double, so those appear in perfectly legal code.
# Actual arithmetic/comparison on doubles, or converting a double back to
# something else, means real double math happened somewhere.
#
# Functions allowed to contain double math:
# - __aeabi_* soft-float helpers call each other internally
# - chvprintf (ChibiOS) and luaO_pushvfstring (Lua) unpack varargs-promoted
#   doubles for %f - third-party printf machinery
# - Timer::hasElapsedUs(float) / Timer::getElapsedUs(efitick_t) intentionally
#   use double on a cold path for precision beyond float's 24-bit range,
#   see comments in libfirmware/util/src/timer.cpp
# - newlib float functions (sinf, powf, ...): the double-FPU multilib (F7/H7,
#   fpv5-d16) intentionally implements them with fast hardware double math
#   internally; the single-precision multilib (F4) uses float-only code. The
#   double-argument functions (sin, cos, ...) are NOT allowed - firmware code
#   must call the f-suffixed variants, so the double libm never gets linked.
# Caveat: attribution is by enclosing function in the disassembly, so if LTO
# ever inlines an allowed function's double math into a caller, this check
# fires there - inspect the reported site before extending the allowlist.
ALLOWED_DOUBLE_FUNCS='^(__aeabi_|chvprintf|luaO_pushvfstring|_ZNK5Timer12hasElapsedUsEf|_ZNK5Timer12getElapsedUsEx|(sin|cos|tan|asin|acos|atan|atan2|pow|exp|exp2|expm1|log|log2|log10|log1p|fmod|cbrt|hypot|erf|sqrt)f$)'

check_double_ops() {
	local pattern=$1
	local message=$2
	local hits
	hits=$(awk -v pat="$pattern" -v allow="$ALLOWED_DOUBLE_FUNCS" '
		/^[0-9a-f]+ <[^>]+>:$/ { fn = substr($2, 2, length($2) - 3) }
		$0 ~ pat && fn !~ allow { print "  in " fn ":" $0 }
	' build/rusefi.list)
	if [ -n "$hits" ]; then
		echo "$hits"
		echo "$message"
		exit 1
	fi
}

# Soft-float double ops (single-precision-FPU targets, e.g. F4)
check_double_ops '(bl|blx|b[.]w)[[:space:]].*<__aeabi_(d|cd)[a-z0-9]+>' \
	"Prohibited double-precision soft-float call detected!"

# Hardware double-precision arithmetic (F7/H7 with fpv5-d16)
check_double_ops '\tv(add|sub|mul|div|nmul|sqrt|neg|abs|fma|fms|fnma|fnms|mla|mls|nmla|nmls|cmp|cmpe)[.]f64' \
	"Prohibited hardware double-precision instruction detected!"
