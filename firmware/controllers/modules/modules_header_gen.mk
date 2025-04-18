# Generate header to include all built modules
engine_modules_generated.h.gen : .FORCE
	printf '$(MODULES_INCLUDE)' > $@

engine_modules_generated.h : engine_modules_generated.h.gen
	rsync --checksum $< $@

modules_list_generated.h.gen : .FORCE
	printf '$(MODULES_LIST)' > $@

modules_list_generated.h : modules_list_generated.h.gen
	rsync --checksum $< $@

# All objects could depend on module list
$(OBJS) : engine_modules_generated.h
$(OBJS) : modules_list_generated.h
