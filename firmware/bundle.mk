ifneq (,$(findstring NT,$(UNAME_S)))
	H2D = ../misc/encedo_hex2dfu/hex2dfu.exe
else
	H2D = ../misc/encedo_hex2dfu/hex2dfu.bin
endif

DFU = $(FOLDER)/$(PROJECT).dfu

ifeq ($(LTS),true)
ifneq (,$(REF))
  FOLDER = rusefi.$(REF).$(BUNDLE_NAME)
else
  FOLDER = rusefi.snapshot.$(BUNDLE_NAME)
endif
else
  FOLDER = rusefi.snapshot.$(BUNDLE_NAME)
endif

INI_FILE = tunerstudio/generated/rusefi_$(SHORT_BOARD_NAME).ini

ARTIFACTS = ../artifacts

BUNDLE_FULL_NAME = rusefi_bundle_$(BUNDLE_NAME)

CONSOLE_FOLDER = $(FOLDER)/console
DRIVERS_FOLDER = $(FOLDER)/drivers
CACERTS_FOLDER = $(FOLDER)/update-ts-cacerts

UPDATE_FOLDER_SOURCES = \
  $(RUSEFI_CONSOLE_SETTINGS) \
  $(INI_FILE) \
  ../misc/console_launcher/rusefi_updater.exe \
  ../misc/console_launcher/readme.html

FOLDER_SOURCES = \
  ../java_console/bin

UPDATE_CONSOLE_FOLDER_SOURCES = \
  ../java_console_binary/rusefi_autoupdate.jar \
  ../java_console_binary/rusefi_console.jar

CONSOLE_FOLDER_SOURCES = \
  ../simulator/build/rusefi_simulator.exe \
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
  ../firmware/ext/openblt/Host/libopenblt_jni.dylib

CACERTS_FOLDER_SOURCES = $(wildcard ../misc/console_launcher/update-ts-cacerts/*)

ifeq ($(USE_OPENBLT),yes)
  BOOTLOADER_BIN = bootloader/blbuild/openblt_$(PROJECT_BOARD).bin
  BOOTLOADER_HEX = bootloader/blbuild/openblt_$(PROJECT_BOARD).hex
  BOUTS = $(FOLDER)/openblt.bin
  SREC_TARGET = $(FOLDER)/rusefi_update.srec
  OUTBIN = $(FOLDER)/$(PROJECT).bin
  OUTBIN_IN = .hex
else
  OUTBIN = $(FOLDER)/$(PROJECT).bin
  OUTBIN_IN = .bin
  OUTS = $(FOLDER)/$(PROJECT).hex
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

$(BOOTLOADER_HEX) $(BOOTLOADER_BIN) &:
	$(MAKE) -C bootloader -r

$(BUILDDIR)/$(PROJECT).map: $(BUILDDIR)/$(PROJECT).elf

$(SREC_TARGET): $(BUILDDIR)/rusefi.srec
	ln -rfs $< $@

$(OUTBIN): $(FOLDER)/%.bin: $(BUILDDIR)/%$(OUTBIN_IN) $(BOOTLOADER_HEX) | $(FOLDER)
ifeq ($(USE_OPENBLT),yes)
	$(H2D) -i $(BOOTLOADER_HEX) -i $< -C 0x1C -b $@
else
	ln -rfs $< $@
endif

$(OUTS): $(FOLDER)/%: $(BUILDDIR)/% | $(FOLDER)
	ln -rfs $< $@

$(BOUTS): $(FOLDER)/openblt%: bootloader/blbuild/openblt_$(PROJECT_BOARD)% | $(FOLDER)
	ln -rfs $< $@

$(DFU): $(FOLDER)/%.dfu: | $(BUILDDIR)/%.hex $(BOOTLOADER_HEX) $(FOLDER)
ifeq ($(USE_OPENBLT),yes)
	$(H2D) -i $(BOOTLOADER_HEX) -i $< -C 0x1C -o $@
else
	$(H2D) -i $< -C 0x1C -o $@
endif

$(ST_DRIVERS): | $(DRIVERS_FOLDER)
	wget https://rusefi.com/build_server/st_files/silent_st_drivers2.exe -P $(dir $@)

$(ARTIFACTS) $(FOLDER) $(CONSOLE_FOLDER) $(DRIVERS_FOLDER) $(CACERTS_FOLDER):
	mkdir -p $@

$(ARTIFACTS)/$(BUNDLE_FULL_NAME).zip: $(BUNDLE_FILES) | $(ARTIFACTS)
	zip -r $@ $(BUNDLE_FILES)

$(ARTIFACTS)/$(BUNDLE_FULL_NAME)_autoupdate.zip: $(UPDATE_BUNDLE_FILES) | $(ARTIFACTS)
	cd $(FOLDER) &&	zip -r ../$@ $(subst $(FOLDER)/,,$(UPDATE_BUNDLE_FILES))

.PHONY: bundle

bundle: $(ARTIFACTS)/$(BUNDLE_FULL_NAME)_autoupdate.zip $(ARTIFACTS)/$(BUNDLE_FULL_NAME).zip

CLEAN_BUNDLE_HOOK:
	$(MAKE) -C bootloader clean
	rm -rf $(FOLDER)

PERCENT = %

.SECONDEXPANSION:
$(FOLDER_TARGETS) $(UPDATE_FOLDER_TARGETS): $(FOLDER)/%: $$(filter $$(PERCENT)$$*,$(FOLDER_SOURCES) $(UPDATE_FOLDER_SOURCES)) | $(FOLDER)
	ln -rfs $< $@

$(CONSOLE_FOLDER_TARGETS) $(UPDATE_CONSOLE_FOLDER_TARGETS): $(CONSOLE_FOLDER)/%: $$(filter $$(PERCENT)$$*,$(CONSOLE_FOLDER_SOURCES) $(UPDATE_CONSOLE_FOLDER_SOURCES)) | $(CONSOLE_FOLDER)
	ln -rfs $< $@

$(CACERTS_FOLDER_TARGETS): $(CACERTS_FOLDER)/%: $$(filter $$(PERCENT)$$*,$(CACERTS_FOLDER_SOURCES)) | $(CACERTS_FOLDER)
	ln -rfs $< $@

