ifeq ($(KINETIS_CONTRIB),)
  KINETIS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

HW_LAYER_EMS += 	$(KINETIS_CONTRIB)/os/hal/ports/KINETIS/KE1xF/fsl/fsl_ftfx_flexnvm.c \
					$(KINETIS_CONTRIB)/os/hal/ports/KINETIS/KE1xF/fsl/fsl_ftfx_controller.c

HW_LAYER_EMS_CPP += $(PROJECT_DIR)/hw_layer/ports/kinetis/mpu_util.cpp \
	$(PROJECT_DIR)/hw_layer/ports/kinetis/kinetis_pins.cpp \
	$(PROJECT_DIR)/hw_layer/ports/kinetis/backup_ram.cpp \
	$(PROJECT_DIR)/hw_layer/ports/kinetis/kinetis_common.cpp \
	$(PROJECT_DIR)/hw_layer/trigger_input_comp.cpp \
	$(PROJECT_DIR)/hw_layer/microsecond_timer/microsecond_timer_gpt.cpp \
	$(PROJECT_DIR)/hw_layer/ports/kinetis/flash_int.c \

MCU = cortex-m4
