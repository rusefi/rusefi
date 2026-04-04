#!/bin/bash
# Format firmware code with clang-format, excluding external code
# Usage: ./format.sh [check]
#   format.sh              - Format files in-place
#   format.sh check        - Check if formatting is needed (exit 1 if changes needed)

set -e

CHECK_ONLY=false
if [ "$1" = "check" ]; then
    CHECK_ONLY=true
fi

# Find all C/C++ files in firmware and unit_tests directories
# Exclude:
#   - firmware/ext (external submodules)
#   - unit_tests/googletest (external submodule)
#   - generated files
files=$(find firmware unit_tests \
    -path "firmware/ext" -prune -o \
    -path "unit_tests/googletest" -prune -o \
    -path "*_generated.h" -prune -o \
    -path "firmware/generated" -prune -o \
    -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cc" \) \
    -print)

if [ -z "$files" ]; then
    echo "No C/C++ files found"
    exit 0
fi

if [ "$CHECK_ONLY" = true ]; then
    # Check mode: show formatting diffs
    echo "Checking code formatting..."

    has_changes=false
    while IFS= read -r file; do
        echo "Checking $file..."

        # Get the formatted version
        formatted=$(./firmware/ext/clang-format "$file")

        # Compare with original
        if ! diff -u "$file" <(echo "$formatted") > /dev/null 2>&1; then
            # Changes needed - show the diff
            echo ""
            echo "=== Formatting changes needed in: $file ==="
            diff -u "$file" <(echo "$formatted") || true
            has_changes=true
        fi
    done <<< "$files"

    if [ "$has_changes" = true ]; then
        echo ""
        echo "Formatting check failed. Run './format.sh' to fix."
        exit 1
    else
        echo "Code formatting is correct."
        exit 0
    fi
else
    # Format mode: apply changes in-place
    echo "Formatting code..."
    echo "$files" | xargs -d '\n' ./firmware/ext/clang-format -i
    echo "Formatting complete"
    exit 0
fi
