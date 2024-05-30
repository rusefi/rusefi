#!/bin/bash

SUBMODULE=$1
BRANCH=$2

if [[ -z "$SUBMODULE" || -z "$BRANCH" ]]; then
  echo "Two arguments expected: submodule path and branch"
  exit -1
fi

echo "Running with SUBMODULE=[$SUBMODULE] and BRANCH=[$BRANCH]"
git submodule set-branch -b $BRANCH $SUBMODULE
git submodule sync

git add $SUBMODULE
git add .gitmodules
git commit -am "GHA says switch $SUBMODULE branch to $BRANCH"
