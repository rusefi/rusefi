#!/bin/bash
set -e

cd "$(dirname "$0")/../.."

git reset --hard
# we are avoiding '--recursive' since we do not really need complete git dependency tree
# for instance wideband/ext/openblt is not needed
git submodule update --init

# a directory holding files tracked by this repo must never be its own git repo: such a '.git' is a
# leftover of a submodule converted to plain files (e.g. cmsis-svd) and would hide the directory
# from 'git status' and 'git clean' forever; purge the stale metadata so the clean below reaches it
topgit=$(git rev-parse --absolute-git-dir)
git ls-files | sed 's|/[^/]*$||' | sort -u | while IFS= read -r dir; do
  if [ -e "$dir/.git" ]; then
    gitdir=$(cd "$dir" && git rev-parse --absolute-git-dir 2>/dev/null || true)
    if [ -n "$gitdir" ] && [ "$gitdir" != "$topgit" ]; then
      rm -rf "$gitdir"
    fi
    rm -rf "$dir/.git"
    echo "removed stale git metadata from $dir"
  fi
done

# remove untracked/ignored files, '-ff' also removes nested git checkouts which are not submodules
git clean -ffxd
# top-level 'git clean' does not descend into submodule working trees, clean those explicitly
git submodule foreach --recursive 'git reset --hard && git clean -ffxd'
