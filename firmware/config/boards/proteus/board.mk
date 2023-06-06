include $(BOARDS_DIR)/proteus/proteus-base.mk

DDEFS += -DFIRMWARE_ID=\"proteus\"

ifeq ($(PROJECT_CPU),ARCH_STM32F7)
	DDEFS += -DSHORT_BOARD_NAME=proteus_f7
else ifeq ($(PROJECT_CPU),ARCH_STM32F4)
	DDEFS += -DSHORT_BOARD_NAME=proteus_f4
else ifeq ($(PROJECT_CPU),ARCH_STM32H7)
	DDEFS += -DSHORT_BOARD_NAME=proteus_h7
else
$(error Unsupported PROJECT_CPU for Proteus: [$(PROJECT_CPU)])
endif
