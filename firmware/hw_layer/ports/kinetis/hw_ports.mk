ifeq ($(KINETIS_CONTRIB),)
	KINETIS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

HW_PORTS_SRC = \
	$(HW_PORT_DIR)/flash_int.c \
	$(KINETIS_CONTRIB)/os/hal/ports/KINETIS/KE1xF/fsl/fsl_ftfx_flexnvm.c \
	$(KINETIS_CONTRIB)/os/hal/ports/KINETIS/KE1xF/fsl/fsl_ftfx_controller.c

HW_PORTS_CPP = \
	$(HW_PORT_DIR)/mpu_util.cpp \
	$(HW_PORT_DIR)/kinetis_pins.cpp \
	$(HW_PORT_DIR)/backup_ram.cpp \
	$(HW_PORT_DIR)/kinetis_common.cpp \
	$(HW_LAYER_DIR)/trigger_input_comp.cpp \
	$(HW_LAYER_DIR)/microsecond_timer/microsecond_timer_gpt.cpp

HW_PORTS_INC =
