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
# Why a custom script instead of an off-the-shelf solution?
#
# Alternatives considered:
#   1. GitHub built-in release notes: 'generate_release_notes: true' on the create-release API, exposed as
#      'generateReleaseNotes: true' by ncipollo/release-action (which we already use). This is what
#      lvgl-linux-dash does in bin/create-combo10-release.py.
#   2. Third-party actions: mikepenz/release-changelog-builder-action (mode: COMMIT), metcalfc/changelog-generator,
#      orhun/git-cliff-action (needs conventional-commit prefixes).
#
# Pros of this script:
#   - Lists commits, not pull requests. Most rusEFI work lands as direct commits to master; GitHub's built-in
#     notes only list merged PRs (plus "New Contributors" and a compare link), so a typical nightly would be
#     nearly empty. Same problem in lvgl-linux-dash, where commits are also direct.
#   - Filters repo-specific bot noise (Auto-generated configs/tune/stack usage, 'Update date', branch merges),
#     which would otherwise be half of every list. The generic actions can do this too but only via regex
#     configuration that lives in the workflow YAML.
#   - Previous-tag detection is explicit and deterministic (latest YYYY-MM-DD tag older than today). GitHub's
#     built-in notes pick "the previous release" by their own rules (which for date tags happen to match, but
#     nothing pins that down).
#   - No clone: ~60 lines of bash + jq over 'gh api', runs in seconds in a job that has no checkout.
#   - Fully testable from a laptop: same command, same output, no workflow run needed.
#   - Zero new third-party action dependencies (supply chain, version pinning, deprecation).
#
# Cons of this script:
#   - It is one more piece of home-grown CI to maintain; the generic actions come with templates,
#     categories by label, contributor lists and are maintained by someone else.
#   - No PR-level grouping: a squash-merged PR shows as one commit, a rebase-merged PR shows as N commits
#     with no link to the PR. The built-in notes would give the PR title + author in both cases.
#   - No "New Contributors" section and no automatic categorization by label.
#   - Commit subjects are shown verbatim, so quality of the changelog is exactly the quality of commit messages.
#   - The compare API pages commits 250 at a time; 'gh api --paginate' follows the pages, but a gap of many
#     weeks between nightlies would make the list very long, with no summarization.
#
# The two are not exclusive: ncipollo/release-action prepends 'bodyFile' to the generated notes when
# 'generateReleaseNotes: true' is also set, so PR-based notes can be added on top of this list later.
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
