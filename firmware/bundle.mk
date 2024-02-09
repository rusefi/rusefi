ifeq (,$(BUNDLE_NAME))
  BUNDLE_NAME = $(SHORT_BOARD_NAME)
endif

ifneq (,$(findstring NT,$(UNAME_S)))
	H2D = ../misc/encedo_hex2dfu/hex2dfu.exe
else
	H2D = ../misc/encedo_hex2dfu/hex2dfu.bin
endif

DFU = $(DELIVER)/$(PROJECT).dfu
DBIN = $(DELIVER)/$(PROJECT).bin
OUTBIN = $(FOLDER)/$(PROJECT).bin

ifeq ($(LTS),true)
ifneq (,$(REF))
  FOLDER = rusefi.$(REF).$(BUNDLE_NAME)
else
  FOLDER = rusefi.snapshot.$(BUNDLE_NAME)
endif
else
  FOLDER = rusefi.snapshot.$(BUNDLE_NAME)
endif

DELIVER = deliver
ARTIFACTS = ../artifacts

BUNDLE_FULL_NAME = rusefi_bundle_$(BUNDLE_NAME)

CONSOLE_FOLDER = $(FOLDER)/console
DRIVERS_FOLDER = $(FOLDER)/drivers
CACERTS_FOLDER = $(FOLDER)/update-ts-cacerts

UPDATE_FOLDER_SOURCES = \
  $(RUSEFI_CONSOLE_SETTINGS) \
  $(INI_FILE) \
  ../misc/console_launcher/readme.html
#  ../misc/console_launcher/rusefi_updater.exe

FOLDER_SOURCES = \
  ../java_console/bin

ifneq ($(BUNDLE_SIMULATOR),no)
  SIMULATOR_OUT = ../simulator/build/rusefi_simulator.exe
endif

UPDATE_CONSOLE_FOLDER_SOURCES = \
  $(CONSOLE_OUT) \
  $(AUTOUPDATE_OUT)

CONSOLE_FOLDER_SOURCES = \
  ../misc/console_launcher/rusefi_autoupdate.exe \
  ../misc/console_launcher/rusefi_console.exe \
  ../misc/install/openocd \
  ../misc/install/STM32_Programmer_CLI \
  $(wildcard ../java_console/*.dll) \
  ../firmware/ext/openblt/Host/libopenblt.dll \
  ../firmware/ext/openblt/Host/libopenblt.so \
  ../firmware/ext/openblt/Host/libopenblt.dylib \
  ../firmware/ext/openblt/Host/openblt_jni.dll \
  ../firmware/ext/openblt/Host/libopenblt_jni.so \
  ../firmware/ext/openblt/Host/libopenblt_jni.dylib \
  $(SIMULATOR_OUT)

CACERTS_FOLDER_SOURCES = $(wildcard ../misc/console_launcher/update-ts-cacerts/*)

BOOTLOADER_BIN = bootloader/blbuild/openblt_$(PROJECT_BOARD).bin
BOOTLOADER_HEX = bootloader/blbuild/openblt_$(PROJECT_BOARD).hex

ifeq ($(USE_OPENBLT),yes)
  BOOTLOADER_OUT = $(BOOTLOADER_HEX)
  BOUTS = $(FOLDER)/openblt.bin
  SREC_TARGET = $(FOLDER)/rusefi_update.srec
else
  OUTS = $(FOLDER)/$(PROJECT).hex
  BINSRC = $(BUILDDIR)/$(PROJECT).bin
ifeq ($(INCLUDE_ELF),yes)
  OUTS += $(FOLDER)/$(PROJECT).elf $(FOLDER)/$(PROJECT).map $(FOLDER)/$(PROJECT).list
endif
endif

ST_DRIVERS = $(DRIVERS_FOLDER)/silent_st_drivers2.exe

UPDATE_FOLDER_TARGETS = $(addprefix $(FOLDER)/,$(notdir $(UPDATE_FOLDER_SOURCES)))
UPDATE_CONSOLE_FOLDER_TARGETS = $(addprefix $(CONSOLE_FOLDER)/,$(notdir $(UPDATE_CONSOLE_FOLDER_SOURCES)))
CACERTS_FOLDER_TARGETS = $(addprefix $(CACERTS_FOLDER)/,$(notdir $(CACERTS_FOLDER_SOURCES)))

UPDATE_BUNDLE_FILES = \
  $(OUTS) \
  $(BOUTS) \
  $(OUTBIN) \
  $(SREC_TARGET) \
  $(UPDATE_FOLDER_TARGETS) \
  $(UPDATE_CONSOLE_FOLDER_TARGETS) \
  $(CACERTS_FOLDER_TARGETS)

FOLDER_TARGETS = $(addprefix $(FOLDER)/,$(notdir $(FOLDER_SOURCES)))
CONSOLE_FOLDER_TARGETS = $(addprefix $(CONSOLE_FOLDER)/,$(notdir $(CONSOLE_FOLDER_SOURCES)))

BUNDLE_FILES = \
  $(UPDATE_BUNDLE_FILES) \
  $(ST_DRIVERS) \
  $(FOLDER_TARGETS) \
  $(CONSOLE_FOLDER_TARGETS)

$(SIMULATOR_OUT):
	$(MAKE) -C ../simulator -r SIMULATOR_DEBUG_LEVEL_OPT="-O2" OS="Windows_NT"

$(BOOTLOADER_HEX) $(BOOTLOADER_BIN): .bootloader-sentinel ;

.bootloader-sentinel:
	BOARD_DIR=../$(BOARD_DIR) BOARD_META_PATH=../$(BOARD_META_PATH) $(MAKE) -C bootloader -r
	@touch $@

$(BUILDDIR)/$(PROJECT).map: $(BUILDDIR)/$(PROJECT).elf

$(SREC_TARGET): $(BUILDDIR)/rusefi.srec
	ln -rfs $< $@

$(OUTS): $(FOLDER)/%: $(BUILDDIR)/% | $(FOLDER)
	ln -rfs $< $@

$(BOUTS): $(FOLDER)/openblt%: bootloader/blbuild/openblt_$(PROJECT_BOARD)% | $(FOLDER)
	ln -rfs $< $@

$(OUTBIN) $(FOLDER)/$(PROJECT).dfu: $(FOLDER)/%: $(DELIVER)/% | $(FOLDER)
	ln -rfs $< $@

$(DFU) $(DBIN): .h2d-sentinel ;

.h2d-sentinel: $(BUILDDIR)/$(PROJECT).hex $(BOOTLOADER_OUT) $(BINSRC) | $(DELIVER)
ifeq ($(USE_OPENBLT),yes)
	$(H2D) -i $(BOOTLOADER_HEX) -i $(BUILDDIR)/$(PROJECT).hex -C 0x1C -o $(DFU) -b $(DBIN)
else
	$(H2D) -i $(BUILDDIR)/$(PROJECT).hex -C 0x1C -o $(DFU)
	cp $(BUILDDIR)/$(PROJECT).bin $(DBIN)
endif
	@touch $@

$(ST_DRIVERS): | $(DRIVERS_FOLDER)
	wget https://rusefi.com/build_server/st_files/silent_st_drivers2.exe -P $(dir $@)

$(DELIVER) $(ARTIFACTS) $(FOLDER) $(CONSOLE_FOLDER) $(DRIVERS_FOLDER) $(CACERTS_FOLDER):
	mkdir -p $@

$(ARTIFACTS)/$(BUNDLE_FULL_NAME).zip: $(BUNDLE_FILES) | $(ARTIFACTS)
	zip -r $@ $(BUNDLE_FILES)

$(ARTIFACTS)/$(BUNDLE_FULL_NAME)_autoupdate.zip: $(UPDATE_BUNDLE_FILES) | $(ARTIFACTS)
	cd $(FOLDER) &&	zip -r ../$@ $(subst $(FOLDER)/,,$(UPDATE_BUNDLE_FILES))

.PHONY: bundle

bundle: $(ARTIFACTS)/$(BUNDLE_FULL_NAME)_autoupdate.zip $(ARTIFACTS)/$(BUNDLE_FULL_NAME).zip all

CLEAN_BUNDLE_HOOK:
	@echo Cleaning Bundle
	$(MAKE) -C ../simulator clean
	BOARD_DIR=../$(BOARD_DIR) BOARD_META_PATH=../$(BOARD_META_PATH) $(MAKE) -C bootloader clean
	rm -rf $(FOLDER)
	@echo Done Cleaning Bundle

PERCENT = %

.SECONDEXPANSION:
$(FOLDER_TARGETS) $(UPDATE_FOLDER_TARGETS): $(FOLDER)/%: $$(filter $$(PERCENT)$$*,$(FOLDER_SOURCES) $(UPDATE_FOLDER_SOURCES)) | $(FOLDER)
	ln -rfs $< $@

$(CONSOLE_FOLDER_TARGETS) $(UPDATE_CONSOLE_FOLDER_TARGETS): $(CONSOLE_FOLDER)/%: $$(filter $$(PERCENT)$$*,$(CONSOLE_FOLDER_SOURCES) $(UPDATE_CONSOLE_FOLDER_SOURCES)) | $(CONSOLE_FOLDER)
	ln -rfs $< $@

$(CACERTS_FOLDER_TARGETS): $(CACERTS_FOLDER)/%: $$(filter $$(PERCENT)$$*,$(CACERTS_FOLDER_SOURCES)) | $(CACERTS_FOLDER)
	ln -rfs $< $@
