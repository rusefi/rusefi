ifeq ($(TRGT),)
	UNAME_SM := $(shell uname -sm)
$(info UNAME_SM:          $(UNAME_SM))

	ifeq ($(firstword $(UNAME_SM)),Darwin)
		COMPILER_PLATFORM = arm-gnu-toolchain-11.3.rel1-darwin-x86_64-arm-none-eabi
	else ifeq ($(UNAME_SM),Linux x86_64)
		COMPILER_PLATFORM = arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-eabi
	else
$(error Looks like your platform, $(UNAME_SM), doesn't have a supported compiler!)
	endif

	BUILD_TOOLS_DIR = $(PROJECT_DIR)/ext/build-tools/
	TRGT = $(BUILD_TOOLS_DIR)$(COMPILER_PLATFORM)/bin/arm-none-eabi-

	# If the compiler doesn't exist, try to update the build tools submodule
	ifeq ("$(wildcard $(TRGT)g++)","")
$(info Compiler at $(TRGT)g++ not found, trying to update build-tools submodule...")
$(shell git submodule update --init --depth=1 $(BUILD_TOOLS_DIR))
$(error Please re-run make to execute build!)
	endif
else
	# If the compiler doesn't exist, fault now
	ifeq ("$(wildcard $(TRGT)g++)","")
$(error Compiler not found at $(TRGT)g++!)
	endif
endif

$(info COMPILER_PLATFORM: $(COMPILER_PLATFORM))
$(info TRGT:              $(TRGT))
