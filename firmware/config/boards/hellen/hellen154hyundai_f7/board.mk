# Combine the related files for a specific platform and MCU.

# Target ECU board design
BOARDCPPSRC = $(BOARD_DIR)/board_configuration.cpp
# Set this if you want a default engine type other than normal
ifeq ($(VAR_DEF_ENGINE_TYPE),)
  VAR_DEF_ENGINE_TYPE = -DDEFAULT_ENGINE_TYPE=engine_type_e::HELLEN_154_HYUNDAI_COUPE_BK2
endif

DDEFS += -DEFI_MAIN_RELAY_CONTROL=TRUE
DDEFS += -DBOARD_MC33810_COUNT=1


	DDEFS += -DCH_DBG_ENABLE_ASSERTS=FALSE
	DDEFS += -DENABLE_PERF_TRACE=FALSE



# Add them all together
DDEFS += $(VAR_DEF_ENGINE_TYPE)
DDEFS += -DSTATIC_BOARD_ID=STATIC_BOARD_ID_HELLEN_154_HYUNDAI

include $(BOARDS_DIR)/hellen/hellen-common-mega144.mk
DDEFS += -DHW_HELLEN_HYUNDAI=1
