#!/bin/bash

# this script is helps to back-port complete master java into some very old branch by just dumping it there

TARGET_DIR=${1:-../rusefi.2}

if [ ! -d "$TARGET_DIR" ]; then
  echo "Target directory $TARGET_DIR does not exist"
  exit 1
fi

echo "Back-porting Java to $TARGET_DIR"

# remove all java files in target dir
echo "Removing old Java files..."
find "$TARGET_DIR/java_console" "$TARGET_DIR/java_tools" -name "*.java" -type f -delete 2>/dev/null

# copy all java
echo "Copying new Java files..."
find java_console java_tools -name "*.java" | rsync -avR --files-from=- . "$TARGET_DIR/"

echo "Done!"
