# Trigger Decoders

This folder is and should not be aware of engine.h or engine_configuration.h

# Hints on adding new trigger

Step 1: add into rusefi_config.txt

Step 2: add into rusefi_enums.h, update TT_UNUSED, invoke gen_enum script

Step 3: get it working.

It's useful to un-comments *AllTriggersFixture* line in unit_tests/main.cpp

It's useful to add setVerboseTrigger(true) into unit tests while troubleshooting fresh trigger code