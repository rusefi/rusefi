#!/bin/bash
# "One console for all boards" (#9714): build a board-agnostic "universal" console bundle FROM SCRATCH.
#
# It gathers the console + launchers + drivers directly from the repo and does NOT depend on any
# per-board bundle artifact, so the universal bundle is still produced even if every board build fails.
#
# The console jar carries board_compatibility=* (shared_io.properties), resolves the .ini from the
# connected ECU's signature, and downloads per-board firmware on demand (Autoupdate.ensureFirmwareForTarget),
# so this bundle ships NO firmware and NO .ini.
#
# Requires the console jar to be built first:
#   ./gradlew :ui:shadowJar :ts_plugin_launcher:shadowJar
# The full bundle includes a pinned Windows x64 JRE; the autoupdate ZIP never replaces Java.
#
# Usage: assemble_universal_bundle.sh <output-dir> [platform-name] [release-name]
set -euo pipefail

OUT_DIR="$1"
PLATFORM="${2:-universal}"
RELEASE="${3:-development}"
WHITE_LABEL="${WHITE_LABEL:-rusefi}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

CONSOLE_JAR="$REPO_ROOT/console/rusefi_console.jar"
if [ ! -f "$CONSOLE_JAR" ]; then
  echo "::error::$CONSOLE_JAR not found - build it first: ./gradlew :ui:shadowJar" >&2
  exit 1
fi

work="$(mktemp -d)"
trap 'rm -rf "$work"' EXIT
folder="$work/rusefi.snapshot.$PLATFORM"
mkdir -p "$folder/console" "$folder/drivers"

# --- console/ (the console's working dir) ---
cp "$CONSOLE_JAR" "$folder/console/"
TS_LAUNCHER="$REPO_ROOT/java_tools/ts_plugin_launcher/build/jar/rusefi_ts_plugin_launcher.jar"
[ -f "$TS_LAUNCHER" ] && cp "$TS_LAUNCHER" "$folder/console/"

# PCAN-Basic Java needs both the JNI bridge and the vendor API DLL. The Windows
# launcher sets its working directory to console/, so keep the DLLs beside the jar.
for dll in PCANBasic_JNI.dll PCANBasic.dll; do
  source="$REPO_ROOT/java_console/$dll"
  if [ ! -f "$source" ]; then
    echo "::error::$source not found - Windows PCAN support would be unusable" >&2
    exit 1
  fi
  cp "$source" "$folder/console/"
done

# release.txt lives in console/ - that is where BundleUtil reads it (console CWD).
printf 'platform=%s\nrelease=%s\n' "$PLATFORM" "$RELEASE" > "$folder/console/release.txt"

# STM32_Programmer_CLI (Windows DFU flashing) - optional/large, include unless told otherwise.
if [ "${DO_NOT_BUNDLE_STM32_PROG:-}" != "yes" ] && [ -d "$REPO_ROOT/misc/install/STM32_Programmer_CLI" ]; then
  cp -a "$REPO_ROOT/misc/install/STM32_Programmer_CLI" "$folder/console/"
fi

# --- drivers/ ---
DRIVERS="$REPO_ROOT/firmware/ext/rusefi-gha/static-content/silent_st_drivers2.exe"
if [ -f "$DRIVERS" ]; then
  cp "$DRIVERS" "$folder/drivers/"
else
  rmdir "$folder/drivers" 2>/dev/null || true
fi

# --- bundle root: launchers + readme ---
for f in rusefi_updater.exe rusefi_updater.sh readme.html; do
  [ -f "$REPO_ROOT/misc/console_launcher/$f" ] && cp "$REPO_ROOT/misc/console_launcher/$f" "$folder/"
done

# [tag:bundled_windows_jre] Keep the running JVM out of console autoupdates.
jre_args=(--cache-dir "${WINDOWS_JRE_CACHE:-${XDG_CACHE_HOME:-$HOME/.cache}/rusefi/jre}")
[ -z "${WINDOWS_JRE_ARCHIVE:-}" ] || jre_args+=(--archive "$WINDOWS_JRE_ARCHIVE")
[ -z "${WINDOWS_JRE_MANIFEST:-}" ] || jre_args+=(--manifest "$WINDOWS_JRE_MANIFEST")
python3 "$SCRIPT_DIR/prepare_windows_jre.py" "$folder/jre" "${jre_args[@]}"

mkdir -p "$OUT_DIR"
OUT_ABS="$(cd "$OUT_DIR" && pwd)"

# Full bundle: folder wrapper with everything.
( cd "$work" && zip -qr "$work/full.zip" "rusefi.snapshot.$PLATFORM" )

# Autoupdate zip: no folder wrapper; only the files the console self-update refreshes
# (console jar + ts launcher + release.txt + readme) - no firmware, no ini for the universal bundle.
au="$work/au"
mkdir -p "$au/console"
cp "$folder/console/rusefi_console.jar" "$au/console/"
[ -f "$folder/console/rusefi_ts_plugin_launcher.jar" ] && cp "$folder/console/rusefi_ts_plugin_launcher.jar" "$au/console/"
cp "$folder/console/PCANBasic_JNI.dll" "$folder/console/PCANBasic.dll" "$au/console/"
cp "$folder/console/release.txt" "$au/console/"
[ -f "$folder/readme.html" ] && cp "$folder/readme.html" "$au/"
( cd "$au" && zip -qr "$work/autoupdate.zip" . )
# Updating an existing ZIP in place retains removed files, including obsolete runtime files.
mv "$work/full.zip" "$OUT_ABS/${WHITE_LABEL}_bundle_${PLATFORM}.zip"
mv "$work/autoupdate.zip" "$OUT_ABS/${WHITE_LABEL}_bundle_${PLATFORM}_autoupdate.zip"

echo "Universal bundle written to $OUT_ABS:"
echo "  ${WHITE_LABEL}_bundle_${PLATFORM}.zip"
echo "  ${WHITE_LABEL}_bundle_${PLATFORM}_autoupdate.zip"
