#!/usr/bin/env bash

# Check if an argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <pattern>"
    exit 1
fi

pattern=$1

# Expand the pattern and iterate over files
# We use a loop to handle potential multiple files matching the pattern
# and to print the required message.
# Note: we should handle cases where the pattern might not match anything
# or where it's a directory.

# Using find to handle potential globbing issues if needed, 
# but simple expansion might be enough if passed with quotes or if handled by shell.
# However, the requirement says "going over individual files matching pattern".

# If we pass a pattern like 'firmware/*/*generated*.cpp', we want to expand it.

shopt -s globstar nullglob

# Function to get changed lines
get_changed_lines() {
    local file=$1
    local stats
    local added
    local deleted

    # git diff --numstat will give "added deleted path"
    # We want to check both unstaged AND staged changes because if it's already staged,
    # git diff will be empty but we still want to report the total change being added.
    # We'll use HEAD as reference to see total change from last commit.
    if git rev-parse --verify HEAD >/dev/null 2>&1; then
        stats=$(git diff --numstat HEAD "$file")
    else
        # Initial commit case: all lines are new
        added=$(wc -l < "$file")
        deleted=0
        echo $((added + deleted))
        return
    fi

    if [ -z "$stats" ]; then
        # Check if it's a new file (not yet in index at all)
        if ! git ls-files "$file" > /dev/null 2>&1; then
            added=$(wc -l < "$file")
            deleted=0
        else
            # File is already in index and no changes since HEAD
            added=0
            deleted=0
        fi
    else
        added=$(echo "$stats" | awk '{print $1}')
        deleted=$(echo "$stats" | awk '{print $2}')
        # Handle the case where awk might get non-numeric values (e.g., binary files)
        if [[ ! "$added" =~ ^[0-9]+$ ]]; then added=0; fi
        if [[ ! "$deleted" =~ ^[0-9]+$ ]]; then deleted=0; fi
    fi
    echo $((added + deleted))
}

# We want to support both directory names and globs
for file in $pattern; do
    if [ -f "$file" ]; then
        total_changed=$(get_changed_lines "$file")
        echo "adding $file file into commit: $total_changed lines changed"
        git add "$file"
    elif [ -d "$file" ]; then
        # If it's a directory, find all files and process them
        find "$file" -type f | while read -r subfile; do
            total_changed=$(get_changed_lines "$subfile")
            echo "adding $subfile file into commit: $total_changed lines changed"
            git add "$subfile"
        done
    fi
done