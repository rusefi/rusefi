

# Add the PCH dir to source path
SRCPATHS += $(PCH_DIR)

PCHOBJ = $(addprefix $(PCH_DIR)/, $(notdir $(PCHSRC:.h=.h.gch)))

# Compile precompiled header file(s) as a cpp file, but output to .h.gch file
$(PCHOBJ) : $(PCH_DIR)/%.h.gch : %.h Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CPPC) -c $(CPPFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(CPPC) -c $(CPPFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
endif

# Make all cpp objects explicitly depend on the PCH
$(TCPPOBJS) : $(PCHOBJ)
$(ACPPOBJS) : $(PCHOBJ)

# Delete PCH output on clean
CLEAN_RULE_HOOK:
	rm -f $(PCHOBJ)
