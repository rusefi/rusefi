#!/usr/bin/env bash
# Trigger the "Update libfirmware Reference" GitHub Actions workflow
# (.github/workflows/update-libfirmware-reference.yaml), which advances the
# firmware/libfirmware submodule to the tip of its tracked branch and pushes the
# bump commit back to the branch the workflow runs on.
#
# This PUSHES a real commit to the chosen branch. By default the workflow runs
# on origin's default branch; pass a branch name to run it on another branch
# (which must exist on origin and contain the workflow file).
#
# Usage: bin/update_libfirmware_reference.sh [branch]

set -euo pipefail

WORKFLOW=update-libfirmware-reference.yaml
REF=${1:-}

# The workflow lives in this checkout's own repo (origin), not in the submodule
# repo named by .gitmodules. Pass it explicitly so gh needs no `gh repo set-default`.
ORIGIN_URL=$(git -C "$(dirname "$0")" remote get-url origin)
REPO=$(echo "$ORIGIN_URL" | sed -E 's#^(https://|ssh://git@|git@)github\.com[:/]##; s#/$##; s#\.git$##')
export GH_REPO=$REPO

if [ -n "$REF" ]; then
  echo "Dispatching $WORKFLOW in $REPO on ref: $REF"
  gh workflow run "$WORKFLOW" --ref "$REF"
else
  echo "Dispatching $WORKFLOW in $REPO on the default branch"
  gh workflow run "$WORKFLOW"
fi

# gh prints no run id; give the API a moment then show the run it created.
sleep 4
gh run list --workflow="$WORKFLOW" --limit 1
