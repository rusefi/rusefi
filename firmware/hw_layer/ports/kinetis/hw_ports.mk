ifeq ($(KINETIS_CONTRIB),)
  KINETIS_CONTRIB = $(CHIBIOS_CONTRIB)
endif

HW_LAYER_EMS += 	$(PROJECT_DIR)/hw_layer/ports/kinetis/flash.c \
					$(KINETIS_CONTRIB)/os/hal/ports/KINETIS/KE1xF/fsl/fsl_ftfx_flexnvm.c \
					$(KINETIS_CONTRIB)/os/hal/ports/KINETIS/KE1xF/fsl/fsl_ftfx_controller.c

HW_LAYER_EMS_CPP += $(PROJECT_DIR)/hw_layer/ports/kinetis/mpu_util.cpp \
	$(PROJECT_DIR)/hw_layer/ports/kinetis/kinetis_pins.cpp \
	$(PROJECT_DIR)/hw_layer/ports/kinetis/kinetis_common.cpp
