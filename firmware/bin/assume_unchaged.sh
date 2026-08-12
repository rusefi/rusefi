#!/usr/bin/env bash

# Makefile invokes 'detect_github.sh'. If $GITHUB_ACTIONS context is NOT detected, this bash script is invoked.

# problem: this seem to not help actually?
# a way to undo: '--no-assume-unchanged'

# todo: proper fix would be to split file so that board-specific stuff is not even here?
git update-index --assume-unchanged ../java_console/models/src/main/java/com/rusefi/config/generated/VariableRegistryValues.java
git update-index --assume-unchanged hw_layer/mass_storage/ramdisk_image_compressed.h

# problem: looks like pathspec patterns *.txt are not supported, unclear why bash expansion is not happening?
# todo: try 'git ls-files "*.env" | xargs git update-index --assume-unchanged'?
git update-index --assume-unchanged controllers/algo/auto*.*
git update-index --assume-unchanged controllers/lua/generated/*
git update-index --assume-unchanged live_data_generated/*
git update-index --assume-unchanged console/binary/generated/*
git update-index --assume-unchanged tunerstudio/generated/*
git update-index --assume-unchanged controllers/generated/*
