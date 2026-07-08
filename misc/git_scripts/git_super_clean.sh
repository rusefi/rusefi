#!/bin/bash
set -e

cd "$(dirname "$0")/../.."

git reset --hard
# we are avoiding '--recursive' since we do not really need complete git dependency tree
# for instance wideband/ext/openblt is not needed
git submodule update --init
# remove untracked/ignored files, '-ff' also removes nested git checkouts which are not submodules
git clean -ffxd
# top-level 'git clean' does not descend into submodule working trees, clean those explicitly
git submodule foreach --recursive 'git reset --hard && git clean -ffxd'
