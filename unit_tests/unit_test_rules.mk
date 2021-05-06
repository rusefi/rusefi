# Define project name here
PROJECT = rusefi_test

ifeq ("$(wildcard $(UNIT_TESTS_DIR)/googletest/LICENSE)","")
$(info Invoking "git submodule update --init")
$(shell git submodule update --init)
$(info Invoked "git submodule update --init")
# make is not happy about newly checked out module for some reason but next invocation would work
$(error Please run 'make' again. Please make sure you have 'git' command in PATH)
endif

