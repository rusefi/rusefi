#!/bin/bash

set -euo pipefail

# this script helps to back-port complete master java into some very old branch by just dumping it there
# NOTE: build.gradle files are intentionally NOT synced — LTS uses different module aliases
#       (e.g. :core_io instead of :shared_io) and different rootDir-relative paths.
#       If the backported code requires new gradle dependencies, add them manually
#       following the pattern from commit 9bb90de.

TARGET_DIR=${1:-../rusefi.3}
PEAK_CAN_PATH=java_console/peak-can-basic

if [ ! -d "$TARGET_DIR" ]; then
  echo "Target directory $TARGET_DIR does not exist"
  exit 1
fi

SOURCE_DIR=$(pwd -P)
TARGET_DIR=$(cd "$TARGET_DIR" && pwd -P)
if [ "$SOURCE_DIR" = "$TARGET_DIR" ]; then
  echo "Source and target must be different directories" >&2
  exit 1
fi

# This directory belongs to a separate Git repository. Use the source index's
# pinned revision, never local edits or untracked files in the submodule.
for directory in "$SOURCE_DIR" "$TARGET_DIR"; do
  if [[ $(git -C "$directory" ls-files --stage -- "$PEAK_CAN_PATH") != 160000\ * ]]; then
    echo "$directory must track $PEAK_CAN_PATH as a Git submodule" >&2
    exit 1
  fi
done
if [ ! -e "$SOURCE_DIR/$PEAK_CAN_PATH/.git" ]; then
  echo "Initialize the source submodule first: git submodule update --init -- $PEAK_CAN_PATH" >&2
  exit 1
fi
PEAK_CAN_REVISION=$(git rev-parse ":$PEAK_CAN_PATH")
git -C "$SOURCE_DIR/$PEAK_CAN_PATH" cat-file -e "$PEAK_CAN_REVISION^{commit}"

# Check before copying/deleting anything, and never discard target-local work.
if [ -e "$TARGET_DIR/$PEAK_CAN_PATH/.git" ]; then
  if [ -n "$(git -C "$TARGET_DIR/$PEAK_CAN_PATH" status --porcelain --untracked-files=all)" ]; then
    echo "Target submodule $PEAK_CAN_PATH has local changes; back-port aborted" >&2
    exit 1
  fi
else
  git -C "$TARGET_DIR" submodule update --init -- "$PEAK_CAN_PATH"
fi
echo "Updating $PEAK_CAN_PATH to $PEAK_CAN_REVISION"
git -C "$TARGET_DIR/$PEAK_CAN_PATH" fetch "$SOURCE_DIR/$PEAK_CAN_PATH" "$PEAK_CAN_REVISION"
git -C "$TARGET_DIR/$PEAK_CAN_PATH" checkout --detach "$PEAK_CAN_REVISION"

echo "Back-porting Java to $TARGET_DIR"

# remove all java files in target dir
echo "Removing old Java files..."
# -delete implies -depth, which defeats -prune; use -exec instead.
find "$TARGET_DIR/java_console" "$TARGET_DIR/java_tools" \
  -path "$TARGET_DIR/$PEAK_CAN_PATH" -prune -o \
  -name "*.java" -type f -exec rm -- {} +

# copy all java
echo "Copying new Java files..."
find java_console java_tools -path "$PEAK_CAN_PATH" -prune -o \
  -name "*.java" -not -path "*/build/*" -print | rsync -avR --files-from=- . "$TARGET_DIR/"

# copy resource files (images, test data, properties) that accompany the java code
echo "Copying resource files..."
find java_console java_tools \
  -path "$PEAK_CAN_PATH" -prune -o \
  \( -path "*/src/main/resources/*" -o -path "*/src/test/resources/*" \) \
  -not -path "*/build/*" -type f -print | rsync -avR --files-from=- . "$TARGET_DIR/"

# copy non-java test data files (ini, msq, xml, etc. under src/test/java)
echo "Copying test data files..."
find java_console java_tools -path "$PEAK_CAN_PATH" -prune -o \
  -path "*/src/test/java/*" \
  \( -name "*.ini" -o -name "*.msq" -o -name "*.xml" -o -name "*.json" \) \
  -not -path "*/build/*" -type f -print | rsync -avR --files-from=- . "$TARGET_DIR/"

echo "Done! Verify the build compiles with: cd $TARGET_DIR/java_tools && ./gradlew :ui:compileJava"
echo "If new gradle module dependencies are missing, see commit 9bb90de for the repair pattern."
