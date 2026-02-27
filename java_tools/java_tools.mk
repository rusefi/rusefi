GRADLE_ROOT = $(PROJECT_DIR)/../java_tools
JAVA_TOOLS = $(PROJECT_DIR)/../java_tools

# We run multiple processes in parallel by passing the -j option to Make.
# If multiple recipes call Gradle, it is likely that those recipes will be executed in parallel.
# Gradle jobs do not like being run in parallel; it causes all kinds of nonsense errors.
# To solve this, we use flock. Flock checks if a file is locked. If it is, it waits until it isn't.
# Once it isn't locked, it locks it, runs the command, then unlocks it once the command is finished.
# Note that flock doesn't ship on macOS. You can install it with `brew install flock`
# On Windows, flock comes with Cygwin.
FLOCK = flock -o /tmp/java.lock

CONFIG_DEFINITION_JAR = $(JAVA_TOOLS)/configuration_definition/build/libs/config_definition-all.jar
CONFIG_DEFINITION_BASE_JAR = $(JAVA_TOOLS)/configuration_definition_base/build/libs/config_definition_base-all.jar
ENUM_TO_STRING_JAR = $(JAVA_TOOLS)/enum_to_string/build/libs/enum_to_string-all.jar
# TUNE_TOOLS_JAR = $(JAVA_TOOLS)/tune-tools/build/libs/tune-tools-all.jar
TS_PLUGIN_LAUNCHER_JAR = $(JAVA_TOOLS)/ts_plugin_launcher/build/jar/rusefi_ts_plugin_launcher.jar
CONSOLE_JAR = $(PROJECT_DIR)/../console/rusefi_console.jar
AUTOUPDATE_JAR = $(PROJECT_DIR)/../console/rusefi_autoupdate.jar

# We use .FORCE to always rebuild these tools. Gradle won't actually touch the jars if it doesn't need to,
# so we don't have to worry about triggering rebuilds of things that have these tools as a prerequisite.

$(CONFIG_DEFINITION_JAR): .docsenums-sentinel .FORCE
	cd $(GRADLE_ROOT) && $(FLOCK) ./gradlew :config_definition:shadowJar

$(CONFIG_DEFINITION_BASE_JAR): .FORCE
	cd $(GRADLE_ROOT) && $(FLOCK) ./gradlew :config_definition_base:shadowJar

$(ENUM_TO_STRING_JAR): .FORCE
	cd $(GRADLE_ROOT) && $(FLOCK) ./gradlew :enum_to_string:shadowJar

#$(TUNE_TOOLS_JAR):
	#cd $(GRADLE_ROOT) && $(FLOCK) ./gradlew :tune-tools:shadowJar

$(TS_PLUGIN_LAUNCHER_JAR): .FORCE
	cd $(GRADLE_ROOT) && $(FLOCK) ./gradlew :ts_plugin_launcher:shadowJar

$(CONSOLE_JAR): .docsenums-sentinel .config-sentinel .FORCE
	cd $(GRADLE_ROOT) && $(FLOCK) ./gradlew :ui:shadowJar

$(AUTOUPDATE_JAR): .FORCE
	cd $(GRADLE_ROOT) && $(FLOCK) ./gradlew :autoupdate:shadowJar

.FORCE:

