#!/bin/bash

echo "Testing matrix generation for evoTech targets only..."

MATRIX_OUTPUT=$(bash bin/generate_matrix.sh)
echo "Matrix output:"
echo "$MATRIX_OUTPUT"

echo ""
echo "Checking targets..."

if [[ "$MATRIX_OUTPUT" == *"evoTech12"* ]]; then
    echo "✅ evoTech12 target found"
else
    echo "❌ evoTech12 target NOT found"
fi

if [[ "$MATRIX_OUTPUT" == *"evoTech12plus"* ]]; then
    echo "✅ evoTech12plus target found"
else
    echo "❌ evoTech12plus target NOT found"
fi

# Check that no other targets are present
TARGET_COUNT=$(echo "$MATRIX_OUTPUT" | grep -o '"build-target"' | wc -l)
echo ""
echo "Total targets found: $TARGET_COUNT"

if [[ "$TARGET_COUNT" -eq 2 ]]; then
    echo "✅ Correct number of targets (2)"
else
    echo "❌ Wrong number of targets (expected 2, got $TARGET_COUNT)"
fi

echo ""
echo "Test completed!" 