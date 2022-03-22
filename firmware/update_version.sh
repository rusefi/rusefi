#!/bin/bash
# Let's regenerate 'svnversion.h'
# TODO: handle std err - for example, in case svn needs upgrade

#
# 2022: is it time to rename svn_version.h into something else?
# we should probably remove VCS_VERSION and keep only GIT_HASH
#

GIT_HASH=$(git rev-parse HEAD)
echo "Git hash=$GIT_HASH"
java -jar ../java_tools/version2header.jar 10000 https://github.com/rusefi/rusefi $GIT_HASH
