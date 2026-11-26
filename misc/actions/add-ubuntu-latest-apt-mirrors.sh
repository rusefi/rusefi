#!/usr/bin/env bash

# Run as root. With package arguments, retry a bounded download on an alternate
# official Ubuntu archive only after the runner-configured source attempt fails.
set -euo pipefail

sources_dir="${APT_SOURCES_DIR:-/etc/apt}"
packages=("$@")

attempt() {
    timeout --kill-after=10s "${APT_UPDATE_TIMEOUT:-2m}" \
        apt-get -o APT::Update::Error-Mode=any -o Acquire::Retries=3 update || return $?
    if ((${#packages[@]})); then
        timeout --kill-after=10s "${APT_DOWNLOAD_TIMEOUT:-3m}" \
            apt-get -o Acquire::Retries=3 --download-only install -y "${packages[@]}"
    fi
}

if attempt; then
    exit 0
fi

for mirror in azure.archive.ubuntu.com archive.ubuntu.com; do
    if ! python3 - "$sources_dir" "$mirror" <<'PY'
import pathlib
import re
import sys

root = pathlib.Path(sys.argv[1])
mirror = sys.argv[2]
host = r"(?:[a-z0-9-]+\.)*ec2\.archive\.ubuntu\.com|(?:security|azure\.archive|archive)\.ubuntu\.com"
url = re.compile(rf"(?P<scheme>https?)://(?:{host})(?=/ubuntu/?(?:\s|$))", re.IGNORECASE)

def replace(text):
    return url.sub(lambda match: f"{match['scheme']}://{mirror}", text)

files = [root / "sources.list"]
files += sorted((root / "sources.list.d").glob("*.list"))
files += sorted((root / "sources.list.d").glob("*.sources"))
files += [root / "apt-mirrors.txt"]
files += sorted((root / "mirrors").glob("ubuntu*.list"))
files += sorted((root / "mirrors").glob("ubuntu*.mirrors"))

changed = False
for source in files:
    if not source.is_file():
        continue
    original = source.read_text()
    if source.suffix == ".sources":
        lines = original.splitlines(keepends=True)
        in_uris = False
        for index, line in enumerate(lines):
            if not line.strip():
                in_uris = False
            elif not line[0].isspace():
                in_uris = bool(re.match(r"^URIs\s*:", line, re.IGNORECASE))
            if in_uris:
                lines[index] = replace(line)
        updated = "".join(lines)
    elif source.name == "apt-mirrors.txt" or source.parent == root / "mirrors":
        updated = replace(original)
    else:
        updated = "".join(
            replace(line) if re.match(r"^\s*deb(?:-src)?\s", line) else line
            for line in original.splitlines(keepends=True)
        )
    if updated != original:
        source.write_text(updated)
        changed = True

sys.exit(0 if changed else 1)
PY
    then
        continue
    fi
    echo "APT acquisition failed or timed out; retrying with $mirror" >&2
    if attempt; then
        exit 0
    fi
done

echo "APT acquisition failed on the available mirrors" >&2
exit 1
