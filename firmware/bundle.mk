ifneq (,$(findstring NT,$(UNAME_S)))
	H2D = ../misc/encedo_hex2dfu/hex2dfu.exe
else
	H2D = ../misc/encedo_hex2dfu/hex2dfu.bin
endif

DFUS = deliver/$(PROJECT).dfu

BUNDLEFILES = \
  $(DFUS) \
  $(OUTS) \
  $(BOUTS)

ifeq ($(USE_OPENBLT),yes)
  BOOTLOADER_BIN = bootloader/blbuild/openblt_$(PROJECT_BOARD).bin
  BOOTLOADER_HEX = bootloader/blbuild/openblt_$(PROJECT_BOARD).hex
  BOUTS = deliver/openblt.bin
else
  OUTS = deliver/$(PROJECT).bin deliver/$(PROJECT).hex
ifeq ($(INCLUDE_ELF),yes)
  OUTS += deliver/$(PROJECT).elf deliver/$(PROJECT).map deliver/$(PROJECT).list
endif
endif

$(BOOTLOADER_HEX) $(BOOTLOADER_BIN) &:
	$(MAKE) -C bootloader -r

$(BUILDDIR)/$(PROJECT).map: $(BUILDDIR)/$(PROJECT).elf

$(OUTS): deliver/%: $(BUILDDIR)/%
	cp $< $@

$(BOUTS): deliver/openblt%: bootloader/blbuild/openblt_$(PROJECT_BOARD)%
	cp $< $@

$(DFUS): deliver/%.dfu: $(BUILDDIR)/%.hex $(BOOTLOADER_HEX) deliver
ifeq ($(USE_OPENBLT),yes)
	$(H2D) -i $(BOOTLOADER_HEX) -i $< -C 0x1C -o $@ -b deliver/$(PROJECT).bin
else
	$(H2D) -i $< -C 0x1C -o $@
endif

deliver:
	mkdir -p deliver

bundle: $(BUNDLEFILES) all

CLEAN_BUNDLE_HOOK:
	@echo Cleaning bundle
	$(MAKE) -C bootloader clean
	rm -rf $(FOLDER)
