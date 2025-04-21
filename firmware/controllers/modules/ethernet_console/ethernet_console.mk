MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/ethernet_console/ethernet_console.cpp
MODULES_LIST += #if EFI_ETHERNET \n EthernetConsoleModule, \n #endif
