
#default=1536
USE_PROCESS_STACKSIZE = 850
#default=4096
USE_EXCEPTIONS_STACKSIZE = 2400

KINETIS_CONTRIB = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/OS

GENERATED_ENUMS_DIR = $(BOARD_DIR)/config/controllers/algo

EXTRA_PARAMS += -DFIRMWARE_ID=\"kinetis\" -nodefaultlibs -L$(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/libc -lgcc -ltinyc
