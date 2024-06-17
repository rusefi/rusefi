#!/bin/bash

SUBMODULE=$1
BRANCH=$2

SCRIPT_NAME=$(basename "$0")

if [[ -z "$SUBMODULE" || -z "$BRANCH" ]]; then
  echo "${SCRIPT_NAME}: Two arguments expected: submodule path and branch"
  exit -1
fi

echo "${SCRIPT_NAME}: Switching SUBMODULE=[$SUBMODULE] to BRANCH=[$BRANCH]"
git submodule set-branch -b $BRANCH $SUBMODULE
git submodule sync

git status

git add $SUBMODULE
git add .gitmodules
git commit -am "GHA says switch $SUBMODULE branch to $BRANCH"

echo Done, plaese take of push!
git status
