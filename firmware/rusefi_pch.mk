# Add the PCH dir to source path
SRCPATHS += $(PCH_DIR)

PCHOBJ = $(addprefix $(PCH_DIR)/, $(notdir $(PCHSRC:.h=.h.gch)))/$(PCHSUB)

# Compile precompiled header file(s) as a cpp file, but output to .h.gch file
$(PCHOBJ) : $(PCH_DIR)/%.h.gch/$(PCHSUB) : %.h Makefile $(CONFIG_FILES)
	@mkdir -p $<.gch
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CPPC) -c $(CPPFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling PCH $(<F)
	@$(CPPC) -c $(CPPFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
endif

# Make all cpp objects explicitly depend on the PCH
# This is necessary because the ChibiOS makefile builds a .o file and generates
#  the deps for that .o file in the same GCC call, so if the .deps aren't already
#  in the correct state, things can fail to build because Make doesn't know it needs
#  to build the prerequisites (in this case PCHOBJ) for those files ahead of time.
$(TCPPOBJS): $(PCHOBJ)
$(ACPPOBJS): $(PCHOBJ)
$(CPPOBJS): $(PCHOBJ)

# Delete PCH output on clean
CLEAN_PCH_HOOK:
	@echo Cleaning PCH
	rm -f $(PCHOBJ)
