
USE_PROCESS_STACKSIZE = 1536
USE_EXCEPTIONS_STACKSIZE = 4096

CYPRESS_CONTRIB = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)/OS

GENERATED_ENUMS_DIR = $(BOARD_DIR)/config/controllers/algo

EXTRA_PARAMS += -DFIRMWARE_ID=\"cypress\" -DSHORT_BOARD_NAME=hellen_cypress
# -nodefaultlibs -lc -lgcc -ltinyc

# used by USE_SMART_BUILD
CONFDIR = $(PROJECT_DIR)/config/boards/$(PROJECT_BOARD)
