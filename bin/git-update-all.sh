#!/usr/bin/env bash
# Traverse all subfolders of the specified folder (default: '..') and run
# 'git fetch' followed by 'git pull' in each one that is a git repository.

set -u

root="${1:-..}"

if [ ! -d "$root" ]; then
    echo "Error: '$root' is not a directory" >&2
    exit 1
fi

for dir in "$root"/*/; do
    [ -d "$dir" ] || continue
    if [ -d "$dir/.git" ] || git -C "$dir" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
        echo "=== $dir ==="
        git -C "$dir" fetch
        git -C "$dir" pull
    else
        echo "--- $dir (not a git repo, skipping)"
    fi
done
