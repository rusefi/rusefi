#!/usr/bin/env bash
# Trigger the "Update ChibiOS Reference" GitHub Actions workflow
# (.github/workflows/update-chibios-reference.yaml), which advances the
# firmware/ChibiOS submodule to the tip of its tracked branch and pushes the
# bump commit back to the branch the workflow runs on.
#
# This PUSHES a real commit to the chosen branch. By default the workflow runs
# on origin's default branch; pass a branch name to run it on another branch
# (which must exist on origin and contain the workflow file).
#
# Usage: bin/update_chibios_reference.sh [branch]

set -euo pipefail

WORKFLOW=update-chibios-reference.yaml
REF=${1:-}

if [ -n "$REF" ]; then
  echo "Dispatching $WORKFLOW on ref: $REF"
  gh workflow run "$WORKFLOW" --ref "$REF"
else
  echo "Dispatching $WORKFLOW on the default branch"
  gh workflow run "$WORKFLOW"
fi

# gh prints no run id; give the API a moment then show the run it created.
sleep 4
gh run list --workflow="$WORKFLOW" --limit 1
