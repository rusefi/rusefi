# The ChibiOS rules makefile at ChibiOS/os/common/startup/ARMCMx/compilers/GCC/mk/rules.mk
#  builds a .o file and generates the deps for that .o file in the same GCC call,
#  so if the .deps aren't already in the correct state,
#  things can fail to build because Make doesn't know it needs
#  to generate the prerequisites for those files ahead of time.
# This makefile is the answer to that problem.
# In this file, we do the following:
# - try to include a .d file for every .o file
# - define recipes to build a .d file if it doesn't exist
# - declare that every .o file has its .d file as a prerequisite, so that if the .d file changes,
#   the .o file will be rebuilt.
# - declare TGT_SENTINEL (see rusefi_config.mk) as a prerequisite of all .d files, so that if we switch
#   to building a different board, all .d files (and consequently all .o files) will be rebuilt.
#   There are a few .d and .o files that wouldn't necessarily need to be rebuilt, but there's no good
#   way to selectively not rebuild them without checking if they need to be rebuilt every time make is run.

TCDEP = $(addprefix $(DEPDIR)/, $(notdir $(TCSRC:.c=.o.d)))
TCPPDEP = $(addprefix $(DEPDIR)/, $(notdir $(patsubst %.cpp, %.o.d, $(filter %.cpp, $(TCPPSRC)))))
TCCDEP = $(addprefix $(DEPDIR)/, $(notdir $(patsubst %.cc, %.o.d, $(filter %.cc, $(TCPPSRC)))))

# Override CFLAGS and CPPFLAGS with their original values before the .dep creation variables were appended.
# This is to prevent to compilation rule in ChibiOS/os/common/startup/ARMCMx/compilers/GCC/mk/rules.mk from
#  also generating .deps
CFLAGS = $(MCFLAGS) $(OPT) $(COPT) $(CWARN) -Wa,-alms=$(LSTDIR)/$(notdir $(<:.c=.lst)) $(DEFS)
CPPFLAGS = $(MCFLAGS) $(OPT) $(CPPOPT) $(CPPWARN) -Wa,-alms=$(LSTDIR)/$(notdir $(<:.cpp=.lst)) $(DEFS)

$(TCDEP): $(DEPDIR)/%.o.d: %.c $(TGT_SENTINEL) | $(CONFIG_FILES) $(DEPDIR)
	@echo Gen deps for $(<F)
	@$(CC) -M -MP -MF $@ $(CFLAGS) -I. $(IINCDIR) $<

$(TCPPDEP): $(DEPDIR)/%.o.d: %.cpp $(TGT_SENTINEL) | $(CONFIG_FILES) $(DEPDIR)
	@echo Gen deps for $(<F)
	@$(CPPC) -M -MP -MF $@ $(CPPFLAGS) -I. $(IINCDIR) $<

$(TCCDEP): $(DEPDIR)/%.o.d: %.cc $(TGT_SENTINEL) | $(CONFIG_FILES) $(DEPDIR)
	@echo Gen deps for $(<F)
	@$(CPPC) -M -MP -MF $@ $(CPPFLAGS) -I. $(IINCDIR) $<

# Make each .o object depend on its .dep file
$(TCOBJS) $(TCPPOBJS) $(TCCOBJS): $(OBJDIR)/%.o: $(DEPDIR)/%.o.d

include $(TCDEP)
include $(TCPPDEP)
include $(TCCDEP)
