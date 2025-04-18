# Generate header to include all built modules
$(PROJECT_DIR)/controllers/modules/generated/engine_modules_generated.h.gen : .FORCE
	printf '$(MODULES_INCLUDE)' > $@

$(PROJECT_DIR)/controllers/modules/generated/engine_modules_generated.h : $(PROJECT_DIR)/controllers/modules/generated/engine_modules_generated.h.gen
	rsync --checksum $< $@

$(PROJECT_DIR)/controllers/modules/generated/modules_list_generated.h.gen : .FORCE
	printf '$(MODULES_LIST)' > $@

$(PROJECT_DIR)/controllers/modules/generated/modules_list_generated.h : $(PROJECT_DIR)/controllers/modules/generated/modules_list_generated.h.gen
	rsync --checksum $< $@

# All objects could depend on module list
$(OBJS) : $(PROJECT_DIR)/controllers/modules/generated/engine_modules_generated.h
$(OBJS) : $(PROJECT_DIR)/controllers/modules/generated/modules_list_generated.h
