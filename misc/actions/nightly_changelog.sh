#!/bin/bash
#
# Generates the changelog (GitHub release body, markdown) for a nightly release.
#
# Usage: nightly_changelog.sh <owner/repo> <today-tag> <head-sha> [output-file]
#
# The changelog lists commits between the previous nightly tag (the latest YYYY-MM-DD tag strictly
# older than <today-tag>) and <head-sha>. Commits produced by automation (auto-generated configs, date
# bumps, plain branch merges) are filtered out since they carry no information for release consumers.
#
# No local checkout is needed: tags are read with 'git ls-remote' and commits via the GitHub API ('gh api'),
# so this works from a job which has not checked out the (large) repository.
#
# Environment: GH_TOKEN is required by 'gh api' (GITHUB_TOKEN of the workflow is enough for reads).
#
set -euo pipefail

REPO="${1:?usage: $0 <owner/repo> <today-tag> <head-sha> [output-file]}"
TODAY_TAG="${2:?usage: $0 <owner/repo> <today-tag> <head-sha> [output-file]}"
HEAD_SHA="${3:?usage: $0 <owner/repo> <today-tag> <head-sha> [output-file]}"
OUTPUT="${4:-changelog.md}"

REPO_URL="https://github.com/${REPO}"

# Latest nightly tag strictly older than today. Nightly tags are plain ISO dates, so lexical sort is chronological.
PREV_TAG=$(git ls-remote --tags --refs "${REPO_URL}" \
  | sed -n 's#.*refs/tags/\([0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\}\)$#\1#p' \
  | sort \
  | awk -v today="${TODAY_TAG}" '$0 < today' \
  | tail -n 1)

if [ -z "${PREV_TAG}" ]; then
  echo "No previous nightly tag found, changelog will only link to the release commit"
  {
    echo "Nightly build of [${HEAD_SHA:0:10}](${REPO_URL}/commit/${HEAD_SHA})."
  } > "${OUTPUT}"
  exit 0
fi

echo "Previous nightly tag: ${PREV_TAG}, head: ${HEAD_SHA}"

# One '<sha>\t<author>\t<first line of message>' per commit, oldest first (compare API order).
# 'author.login' is null for commits whose e-mail is not linked to a GitHub account: fall back to the git name.
COMMITS=$(gh api --paginate "repos/${REPO}/compare/${PREV_TAG}...${HEAD_SHA}" \
  --jq '.commits[] | [.sha, (.author.login // .commit.author.name), (.commit.message | split("\n")[0])] | @tsv')

# Drop automation noise. Keep 'Merge pull request' since those reference the PR.
FILTERED=$(printf '%s\n' "${COMMITS}" | awk -F'\t' '
  $3 ~ /^Auto-generated/ { next }
  $3 ~ /^Update date$/ { next }
  $3 ~ /^Merge branch / { next }
  $3 ~ /^Merge remote-tracking branch / { next }
  NF >= 3 { print }
')

TOTAL=$(printf '%s\n' "${COMMITS}" | grep -c . || true)
KEPT=$(printf '%s\n' "${FILTERED}" | grep -c . || true)
echo "Commits since ${PREV_TAG}: ${TOTAL}, listed: ${KEPT}"

{
  echo "## Changes since [${PREV_TAG}](${REPO_URL}/releases/tag/${PREV_TAG})"
  echo
  if [ "${KEPT}" -eq 0 ]; then
    echo "No source changes, only automated commits."
  else
    # Newest first, so the most recent work is at the top of the release page.
    printf '%s\n' "${FILTERED}" | tac | awk -F'\t' -v repo="${REPO_URL}" '{
      sha = $1; author = $2; subject = $3
      # Escape markdown characters which would otherwise change formatting of the bullet.
      gsub(/[\\`*_<>\[\]]/, "\\\\&", subject)
      printf "- [%s](%s/commit/%s) %s (%s)\n", substr(sha, 1, 10), repo, sha, subject, author
    }'
  fi
  echo
  echo "Full diff: [${PREV_TAG}...${TODAY_TAG}](${REPO_URL}/compare/${PREV_TAG}...${TODAY_TAG})"
} > "${OUTPUT}"

echo "Changelog written to ${OUTPUT}"
