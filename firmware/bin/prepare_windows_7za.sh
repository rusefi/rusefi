#!/usr/bin/env bash
set -euo pipefail

VERSION=26.06
ARCHIVE_NAME="7z${VERSION//./}-extra.7z"
URL="https://github.com/ip7z/7zip/releases/download/$VERSION/$ARCHIVE_NAME"
ARCHIVE_SHA256=396c180f12d2247c24b0b1fcd482c6fc2a2700f61211ffede1061dd971f5e104

fail() {
  echo "prepare_windows_7za.sh: $*" >&2
  exit 1
}

output_dir="${1:?usage: prepare_windows_7za.sh <console-dir> [--cache-dir dir] [--archive file]}"
shift
cache_dir="${WINDOWS_7ZA_CACHE:-${XDG_CACHE_HOME:-$HOME/.cache}/rusefi/7za}"
archive="${WINDOWS_7ZA_ARCHIVE:-}"
while (($#)); do
  case "$1" in
    --cache-dir) (($# >= 2)) || fail "missing --cache-dir value"; cache_dir="$2"; shift 2 ;;
    --archive) (($# >= 2)) || fail "missing --archive value"; archive="$2"; shift 2 ;;
    *) fail "unknown argument: $1" ;;
  esac
done

[[ -d "$output_dir" && ! -L "$output_dir" ]] || fail "output must be an existing real directory: $output_dir"
for name in 7za.exe 7za-LICENSE.txt 7za-readme.txt; do
  [[ ! -e "$output_dir/$name" && ! -L "$output_dir/$name" ]] || fail "refusing to overwrite $output_dir/$name"
done

check_hash() {
  local actual
  actual="$(sha256sum "$1")"
  actual="${actual%% *}"
  [[ "$actual" == "$ARCHIVE_SHA256" ]] || fail "7-Zip $VERSION archive SHA-256 mismatch: $1"
}

if [[ -z "$archive" ]]; then
  mkdir -p "$cache_dir"
  archive="$cache_dir/$ARCHIVE_NAME"
  if [[ ! -f "$archive" ]]; then
    download="$(mktemp "$cache_dir/$ARCHIVE_NAME.XXXXXX")"
    trap 'rm -f -- "$download"' EXIT
    curl -fsSL --proto '=https' --tlsv1.2 "$URL" -o "$download"
    check_hash "$download"
    mv -- "$download" "$archive"
    trap - EXIT
  fi
fi
[[ -f "$archive" ]] || fail "archive does not exist: $archive"
check_hash "$archive"

seven_zip="$(command -v 7z || command -v 7zz || true)"
[[ -n "$seven_zip" ]] || fail "7z build tool missing; run in the repository's Pixi environment"
stage="$(mktemp -d "$output_dir/.7za.XXXXXX")"
trap 'rm -rf -- "$stage"' EXIT
"$seven_zip" e -so "$archive" x64/7za.exe > "$stage/7za.exe"
"$seven_zip" e -so "$archive" License.txt > "$stage/7za-LICENSE.txt"
"$seven_zip" e -so "$archive" readme.txt > "$stage/7za-readme.txt"
mv -- "$stage/7za.exe" "$stage/7za-LICENSE.txt" "$stage/7za-readme.txt" "$output_dir/"
