#!/usr/bin/env bash

# problem: this seem to not help actually?

# todo: proper fix would be to split file so that board-specific stuff is not even here?
git update-index --assume-unchanged ../java_console/models/src/main/java/com/rusefi/config/generated/VariableRegistryValues.java

git update-index --assume-unchanged controllers/algo/auto*.*
git update-index --assume-unchanged controllers/lua/generated/*
git update-index --assume-unchanged live_data_generated/*
git update-index --assume-unchanged console/binary/generated/*
git update-index --assume-unchanged tunerstudio/generated/*
git update-index --assume-unchanged controllers/generated/*
git update-index --assume-unchanged hw_layer/mass_storage/ramdisk_image_compressed.h
