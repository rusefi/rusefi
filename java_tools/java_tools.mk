JAVA_TOOLS = ../java_tools

#
# problem statement: 'make -j4' could easily attempt to run parallel gradle processes. gradle does not seem to like it
# see notes at https://github.com/rusefi/rusefi/pull/6031
#
FLOCK = flock /tmp/java.lock

CONFIG_DEFINITION = $(JAVA_TOOLS)/configuration_definition/build/libs/config_definition-all.jar
CONFIG_DEFINITION_BASE = $(JAVA_TOOLS)/configuration_definition_base/build/libs/config_definition_base-all.jar
ENUM_TO_STRING = $(JAVA_TOOLS)/enum_to_string/build/libs/enum_to_string-all.jar
CONSOLE_OUT = ../java_console_binary/rusefi_console.jar
AUTOUPDATE_OUT = ../java_console_binary/rusefi_autoupdate.jar
TPL_OUT = ../java_tools/ts_plugin_launcher/build/jar/rusefi_ts_plugin_launcher.jar

$(CONFIG_DEFINITION): .FORCE
	cd $(JAVA_TOOLS) && $(FLOCK) ./gradlew :config_definition:shadowJar

$(CONFIG_DEFINITION_BASE): .FORCE
	cd $(JAVA_TOOLS) && $(FLOCK) ./gradlew :config_definition_base:shadowJar

$(ENUM_TO_STRING): .FORCE
	cd $(JAVA_TOOLS) && $(FLOCK) ./gradlew :enum_to_string:shadowJar

$(TPL_OUT): .FORCE
	cd ../java_tools && $(FLOCK) ./gradlew :ts_plugin_launcher:shadowJar

$(CONSOLE_OUT): .FORCE
	cd ../java_tools && $(FLOCK) ./gradlew :ui:shadowJar

$(AUTOUPDATE_OUT): .FORCE
	cd ../java_tools && $(FLOCK) ./gradlew :autoupdate:jar

.FORCE:

