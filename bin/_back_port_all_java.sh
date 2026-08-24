#!/bin/bash

# this script helps to back-port complete master java into some very old branch by just dumping it there
# NOTE: build.gradle files are intentionally NOT synced — LTS uses different module aliases
#       (e.g. :core_io instead of :shared_io) and different rootDir-relative paths.
#       If the backported code requires new gradle dependencies, add them manually
#       following the pattern from commit 9bb90de.

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
find java_console java_tools -name "*.java" -not -path "*/build/*" | rsync -avR --files-from=- . "$TARGET_DIR/"

# copy resource files (images, test data, properties) that accompany the java code
echo "Copying resource files..."
find java_console java_tools \
  \( -path "*/src/main/resources/*" -o -path "*/src/test/resources/*" \) \
  -not -path "*/build/*" -type f | rsync -avR --files-from=- . "$TARGET_DIR/"

# copy non-java test data files (ini, msq, xml, etc. under src/test/java)
echo "Copying test data files..."
find java_console java_tools -path "*/src/test/java/*" \
  \( -name "*.ini" -o -name "*.msq" -o -name "*.xml" -o -name "*.json" \) \
  -not -path "*/build/*" -type f | rsync -avR --files-from=- . "$TARGET_DIR/"

echo "Done! Verify the build compiles with: cd $TARGET_DIR/java_tools && ./gradlew :ui:compileJava"
echo "If new gradle module dependencies are missing, see commit 9bb90de for the repair pattern."
