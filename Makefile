# ─────────────────────────────────────────────────────────────
#  Master Makefile
#     • first word = sub-directory
#     • rest of the words are forwarded (i.e. special rules for subdir's Makefile)
# ─────────────────────────────────────────────────────────────
.NOTPARALLEL:
.DEFAULT_GOAL := help

.PHONY: help firmware simulator unit_tests clean all

SUBDIRS := firmware simulator unit_tests

# -------------------------------------------------------------------
# 0. Help
# -------------------------------------------------------------------
help:
	@echo ""
	@echo "Usage:"
	@echo "  make <subdir> [args...]"
	@echo ""
	@echo "SUBDIRS: $(SUBDIRS)"
	@echo ""
	@echo "Examples:"
	@echo ""
	@echo "# It is expected from user:"
	@echo "#   * to execute from root dir"
	@echo "#   * to set number of jobs via -j# or --jobs=#, this is not shown in examples below"
	@echo ""
	@echo "  make all                         # Calls make in firmware/unit_tests/simulator"
	@echo ""
	@echo "  make clean                       # Calls make clean in firmware/unit_tests/simulator"
	@echo ""
	@echo "  make firmware                    # Builds default target(s) in firmware dir"
	@echo "  make firmware clean              # Runs 'make clean' in firmware dir"
	@echo ""
	@echo "  make firmware %ARG_1%            # Builds board firmware in firmware dir setting vars:"
	@echo "                                   #   BOARD_DIR=./config/boards/%ARG_1%"
	@echo "                                   #   SHORT_BOARD_NAME=%ARG_1%"
	@echo "                                   # Used for boards with dedicated dir and same name as dir"
	@echo ""
	@echo "  make firmware f407-discovery     # Builds board firmware in firmware dir setting vars:"
	@echo "                                   #   BOARD_DIR=./config/boards/f407-discovery"
	@echo "                                   #   SHORT_BOARD_NAME=f407-discovery"
	@echo ""
	@echo "  make firmware %ARG_1% %ARG_2%    # Builds board firmware in firmware dir setting vars:"
	@echo "                                   #   BOARD_DIR=./config/boards/%ARG_1%"
	@echo "                                   #   SHORT_BOARD_NAME=%ARG_2%"
	@echo "                                   # Used for boards with complex dir structure"
	@echo ""
	@echo "  make firmware hellen/uaefi uaefi # Builds board firmware in firmware dir setting vars:"
	@echo "                                   #   BOARD_DIR=./config/boards/hellen/uaefi"
	@echo "                                   #   SHORT_BOARD_NAME=uaefi"
	@echo ""
	@echo "  make firmware hellen/super-uaefi super-uaefi # Builds board firmware in firmware dir setting vars:"
	@echo "                                               #   BOARD_DIR=./config/boards/hellen/super-uaefi"
	@echo "                                               #   SHORT_BOARD_NAME=super-uaefi"
	@echo ""
	@echo "  make simulator        # Builds default target in simulator dir"
	@echo "  make simulator clean  # Runs 'make clean' in simulator dir"
	@echo ""
	@echo "  make unit_tests       # Builds default target in unit_tests dir"
	@echo "  make unit_tests clean # Runs 'make clean' in unit_tests dir"
	@echo ""

# Extract MAKE flags (-j, etc) and targets
TARGETS := $(filter-out -%,$(MAKECMDGOALS))
MAKEFLAGS := $(filter -%,$(MAKECMDGOALS))

FIRMWARE_POTENTIAL_ARG1 := $(word 2,$(TARGETS))
FIRMWARE_POTENTIAL_ARG2 := $(word 3,$(TARGETS))
FIRMWARE_POTENTIAL_ARG3 := $(word 4,$(TARGETS))

# List of allowed top-level targets
EXCLUSIVE_TARGETS := help firmware simulator unit_tests all

# Count how many exclusive targets are present in the command
EXCLUSIVE_COUNT := $(words $(filter $(EXCLUSIVE_TARGETS),$(TARGETS)))

# Disallow more than one exclusive target at once
ifeq ($(EXCLUSIVE_COUNT),1)
# OK
else ifneq ($(EXCLUSIVE_COUNT),0)
$(error Only one of [$(EXCLUSIVE_TARGETS)] can be used at a time. Got: $(filter $(EXCLUSIVE_TARGETS),$(TARGETS)))
endif

clean:
	$(info Cleaning all subdirs: $(SUBDIRS))
	$(MAKE) -C firmware clean
	$(MAKE) -C simulator clean
	$(MAKE) -C unit_tests clean

all:
	$(info Running make with default target in all subdirs: $(SUBDIRS))
	$(MAKE) -C firmware
	$(MAKE) -C simulator
	$(MAKE) -C unit_tests

firmware:
	$(info Building firmware)
	$(info FW_ARGS="$(TARGETS)" FW_ARG1="$(FIRMWARE_POTENTIAL_ARG1)" FW_ARG2="$(FIRMWARE_POTENTIAL_ARG2)" FW_ARG3="$(FIRMWARE_POTENTIAL_ARG3)" FW_MAKEFLAGS="$(MAKEFLAGS)")
ifeq ($(FIRMWARE_POTENTIAL_ARG3),)
  ifeq ($(FIRMWARE_POTENTIAL_ARG1),)
	$(MAKE) -C firmware
  else ifeq ($(FIRMWARE_POTENTIAL_ARG1),clean)
	$(MAKE) -C firmware clean
  else ifeq ($(FIRMWARE_POTENTIAL_ARG2),)
	$(MAKE) -C firmware BOARD_DIR=./config/boards/$(FIRMWARE_POTENTIAL_ARG1) SHORT_BOARD_NAME=$(FIRMWARE_POTENTIAL_ARG1)
  else
	$(MAKE) -C firmware BOARD_DIR=./config/boards/$(FIRMWARE_POTENTIAL_ARG1) SHORT_BOARD_NAME=$(FIRMWARE_POTENTIAL_ARG2)
  endif
else
	$(error Too many arguments for 'firmware'. Max 2.)
endif

simulator:
	$(info Building simulator)
	$(MAKE) -C simulator $(wordlist 2, 99, $(TARGETS))

unit_tests:
	$(info Building unit_tests)
	$(MAKE) -C unit_tests $(wordlist 2, 99, $(TARGETS))

# Catch non-targets so they don't cause "No rule to make target"
%:
	@:
