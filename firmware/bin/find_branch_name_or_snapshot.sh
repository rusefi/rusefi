#!/usr/bin/env bash

BRANCH_NAME=$(git rev-parse --abbrev-ref HEAD)

if [ "$BRANCH_NAME" = "master" ]; then
  echo "snapshot"
else
  echo "$BRANCH_NAME"
fi
