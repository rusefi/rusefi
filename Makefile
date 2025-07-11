# ─────────────────────────────────────────────────────────────
#  Master Makefile
#     • first word = sub-directory
#     • rest of the words are forwarded (i.e. special rules for subdir's Makefile)
# ─────────────────────────────────────────────────────────────

.DEFAULT_GOAL := help
.NOTPARALLEL:

.PHONY: help firmware simulator unit_tests

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

# Extract MAKE flags (-j, etc) and goals
GOALS := $(filter-out -%,$(MAKECMDGOALS))
MAKEFLAGS_ONLY := $(filter -%,$(MAKECMDGOALS))

# Firmware dispatcher
firmware:
ifeq ($(ARG3),)
  ifeq ($(ARG1),)
	@echo "→ make -C firmware $(MAKEFLAGS_ONLY)"
	$(MAKE) -C firmware $(MAKEFLAGS_ONLY)
  else ifeq ($(ARG1),clean)
	@echo "→ make -C firmware $(MAKEFLAGS_ONLY) clean"
	$(MAKE) -C firmware $(MAKEFLAGS_ONLY) clean
  else ifeq ($(ARG2),)
	@echo "→ make -C firmware $(MAKEFLAGS_ONLY) BOARD_DIR=./config/boards/$(ARG1) SHORT_BOARD_NAME=$(ARG1)"
	$(MAKE) -C firmware $(MAKEFLAGS_ONLY) BOARD_DIR=./config/boards/$(ARG1) SHORT_BOARD_NAME=$(ARG1)
  else
	@echo "→ make -C firmware $(MAKEFLAGS_ONLY) BOARD_DIR=./config/boards/$(ARG1) SHORT_BOARD_NAME=$(ARG2)"
	$(MAKE) -C firmware $(MAKEFLAGS_ONLY) BOARD_DIR=./config/boards/$(ARG1) SHORT_BOARD_NAME=$(ARG2)
  endif
else
	$(error Too many arguments for 'firmware'. Max 2.)
endif

# -------------------------------------------------------------------
# Other subdirs — forward everything after the first word
# -------------------------------------------------------------------
simulator:
	$(MAKE) -C simulator $(MAKEFLAGS_ONLY) $(wordlist 2, 99, $(GOALS))

unit_tests:
	$(MAKE) -C unit_tests $(MAKEFLAGS_ONLY) $(wordlist 2, 99, $(GOALS))

# Catch non-targets so they don't cause "No rule to make target"
%:
	@:
