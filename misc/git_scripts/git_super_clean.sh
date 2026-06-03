#!/bin/bash
git reset --hard
# we are avoiding '--recursive' since we do not really need complete git dependency tree
# for instance wideband/ext/openblt is not needed
git submodule update --init
git clean -ffxd
