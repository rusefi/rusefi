#!/usr/bin/env bash

cd "$(dirname "$0")"

#
# fact: it's nice to have history of code-generated files
# problem: we are all made out of mean and want to manually commit generated files
#

if [[ "$GITHUB_ACTIONS" == "true" ]]; then
  echo "This script is running within a GitHub Actions workflow."
  # Add your specific GitHub Actions logic here
else
  echo "This script is NOT running within a GitHub Actions workflow."
  cd ..
  # bash bin/assume_unchaged.sh
fi
