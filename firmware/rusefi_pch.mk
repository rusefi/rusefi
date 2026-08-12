# Each delivery unit (firmware, bootloader, simulator, unit_tests) compiles its PCH
# into its own private directory under its own BUILDDIR, so concurrent builds never
# read or write each other's PCH.
PCH_OUT_DIR = $(BUILDDIR)/pch
PCHOBJ = $(PCH_OUT_DIR)/pch.h.gch

# GCC looks for pch.h.gch in each include directory just before it looks for pch.h
# itself, so putting PCH_OUT_DIR ahead of every other include directory makes this
# build pick up exactly its own PCH (PCH_OUT_DIR holds nothing but the .gch).
# Prepend to IINCDIR rather than INCDIR: the ARM rules.mk computes IINCDIR with ':='
# before this file is included, so an INCDIR change would not propagate there.
IINCDIR := -I$(PCH_OUT_DIR) $(IINCDIR)

# Compile the precompiled header; compiling a .h with the C++ compiler produces a PCH.
# pch.h itself is copied next to the .gch: sources that include pch.h somewhere other
# than as their very first include cannot legally use a PCH, and GCC then wants the
# real header in the directory where it found the .gch.
$(PCHOBJ) : $(PCHSRC) Makefile $(CONFIG_FILES)
	@mkdir -p $(PCH_OUT_DIR)
	@cp $(PCHSRC) $(PCH_OUT_DIR)/pch.h
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CPPC) -c $(CPPFLAGS) $(AOPT) -I. $(IINCDIR) $(PCH_OUT_DIR)/pch.h -o $@
else
	@echo Compiling PCH $(<F)
	@$(CPPC) -c $(CPPFLAGS) $(AOPT) -I. $(IINCDIR) $(PCH_OUT_DIR)/pch.h -o $@
endif

# Make all cpp objects explicitly depend on the PCH
# This is necessary because the ChibiOS makefile builds a .o file and generates
#  the deps for that .o file in the same GCC call, so if the .deps aren't already
#  in the correct state, things can fail to build because Make doesn't know it needs
#  to build the prerequisites (in this case PCHOBJ) for those files ahead of time.
$(TCPPOBJS): $(PCHOBJ)
$(ACPPOBJS): $(PCHOBJ)
$(CPPOBJS): $(PCHOBJ)

# Delete PCH output on clean. Also purge the legacy shared pch.h.gch/ directory that
# used to live next to pch.h, so a stale old-layout PCH can never be picked up.
CLEAN_PCH_HOOK:
	@echo Cleaning PCH
	rm -f $(PCHOBJ) $(PCH_OUT_DIR)/pch.h
	rm -rf $(PCH_DIR)/pch.h.gch
