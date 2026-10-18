# Rolling launch shares EFI_LAUNCH_CONTROL with the spark limiter it uses.
MODULES_INC += $(PROJECT_DIR)/controllers/modules/rolling_launch
MODULES_CPPSRC += $(PROJECT_DIR)/controllers/modules/rolling_launch/rolling_launch.cpp
MODULES_INCLUDE += \#include "rolling_launch.h"\n
