#!/usr/bin/env bash

# stashing everything not-committed - no plans to un-stash, a hacky way to clean local clone
git stash

echo Fetching only current branch to make things quicker

#git rev-parse ... @{upstream}: This finds the name of the remote branch your current branch is tracking (e.g., origin/main).
#sed 's!/! !': This takes that result (origin/main) and replaces the slash with a space, turning it into origin main.
#The Result: It executes git fetch origin main.
#Why: Instead of running a full git fetch, which downloads updates for every branch on the server, this fetches only the specific branch you are currently working on. It’s a great time-saver on massive repositories.

git fetch $(git rev-parse --symbolic-full-name --abbrev-ref @{upstream} | sed 's!/! !')
git rebase
