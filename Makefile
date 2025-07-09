# ─────────────────────────────────────────────────────────────
#  Master Makefile
#     • first word = sub-directory
#     • rest of the words are forwarded (special rules for firmware)
#     • no “Nothing to be done …”, safe with  -j
# ─────────────────────────────────────────────────────────────

.DEFAULT_GOAL := help
.NOTPARALLEL:

# Sub-directories you allow from the top level
SUBDIRS := firmware simulator unit_tests

# -------------------------------------------------------------------
# 0. Help
# -------------------------------------------------------------------
help:
	@echo ""; \
	echo "USAGE  : make <subdir> [args …]"; \
	echo "SUBDIRS: $(SUBDIRS)"; \
	echo ""; \
	echo "Examples:"; \
	echo "  make firmware"; \
	echo "  make firmware clean"; \
	echo "  make firmware uaefi"; \
	echo "  make firmware hellen/uaefi uaefi"; \
	echo "  make simulator"; \
	echo "  make simulator clean"; \
	echo "  make -j16 unit_tests"; \
	echo "  make -j16 unit_tests clean"; \
	echo ""

# -------------------------------------------------------------------
# 1. The dispatcher — does all the work exactly once
# -------------------------------------------------------------------
.PHONY: dispatch
dispatch:
	@set -eu; \
	set -- $(MAKECMDGOALS); \
	subdir=$$1; [ "$${subdir:-}" ] || subdir=help; shift || true; \
	if [ "$$subdir" = help ]; then $(MAKE) --no-print-directory help; exit 0; fi; \
	\
	if [ "$$subdir" = firmware ]; then \
		case "$${1-}" in \
			"")     echo "Running firmware (no extra args)"; \
			        $(MAKE) --no-print-directory -C firmware; exit 0 ;; \
			clean)  if [ -n "$${2-}" ]; then \
			           echo "Error: 'firmware clean' takes no further words"; exit 1; fi; \
			        echo "Running firmware clean"; \
			        $(MAKE) --no-print-directory -C firmware clean; exit 0 ;; \
		esac; \
		board_dir=$$1; short=$${2-$$1}; proj=$${3-}; extra=$${4-}; \
		if [ -n "$$extra" ]; then \
			echo "Error: too many arguments for 'firmware' (max 3)"; exit 1; fi; \
		echo "Running firmware  BOARD_DIR=$$board_dir  SHORT_BOARD_NAME=$$short  PROJECT_BOARD=$$proj"; \
		opts="BOARD_DIR=./config/boards/$$board_dir SHORT_BOARD_NAME=$$short"; \
		[ -n "$$proj" ] && opts="$$opts PROJECT_BOARD=$$proj"; \
		$(MAKE) --no-print-directory -C firmware $$opts; exit 0; \
	fi; \
	\
	if echo "$(SUBDIRS)" | grep -qw "$$subdir"; then \
		echo "Running in subdirectory '$$subdir' with args: $$*"; \
		$(MAKE) --no-print-directory -C $$subdir $$*; \
	else \
		echo "Error: unknown top-level target '$$subdir'"; exit 1; \
	fi

# -------------------------------------------------------------------
# 2. Phony targets for *every* allowed sub-dir
#    Each depends on dispatcher; they have no recipe, so the dispatcher
#    runs and Make never prints “Nothing to be done …”.
# -------------------------------------------------------------------
.PHONY: $(SUBDIRS)
$(SUBDIRS): dispatch

# -------------------------------------------------------------------
# 3. Catch all other words (like ‘clean’, ‘build_bundle’ …)
#    They depend on dispatch too, so the dispatcher sees the *whole*
#    command line once; afterwards these phony targets are satisfied.
# -------------------------------------------------------------------
%: dispatch
	@:
