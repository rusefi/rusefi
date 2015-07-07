################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := cs-rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include util/subdir.mk
-include iar/Debug/Obj/subdir.mk
-include hw_layer/stm32f4/subdir.mk
-include hw_layer/serial_over_usb/subdir.mk
-include hw_layer/lcd/subdir.mk
-include hw_layer/subdir.mk
-include development/test/subdir.mk
-include development/hw_layer/subdir.mk
-include development/subdir.mk
-include deliver/subdir.mk
-include controllers/trigger/subdir.mk
-include controllers/system/subdir.mk
-include controllers/sensors/subdir.mk
-include controllers/math/subdir.mk
-include controllers/core/subdir.mk
-include controllers/algo/subdir.mk
-include controllers/subdir.mk
-include console_util/subdir.mk
-include console/binary/subdir.mk
-include console/subdir.mk
-include config/engines/subdir.mk
-include chibios/os/various/fatfs_bindings/subdir.mk
-include chibios/os/various/cpp_wrappers/subdir.mk
-include chibios/os/various/subdir.mk
-include chibios/os/ports/common/ARMCMx/subdir.mk
-include chibios/os/ports/GCC/ARMCMx/STM32F4xx/subdir.mk
-include chibios/os/ports/GCC/ARMCMx/subdir.mk
-include chibios/os/kernel/src/subdir.mk
-include chibios/os/hal/src/subdir.mk
-include chibios/os/hal/platforms/STM32F4xx/subdir.mk
-include chibios/os/hal/platforms/STM32/USARTv1/subdir.mk
-include chibios/os/hal/platforms/STM32/TIMv1/subdir.mk
-include chibios/os/hal/platforms/STM32/SPIv1/subdir.mk
-include chibios/os/hal/platforms/STM32/RTCv2/subdir.mk
-include chibios/os/hal/platforms/STM32/OTGv1/subdir.mk
-include chibios/os/hal/platforms/STM32/I2Cv1/subdir.mk
-include chibios/os/hal/platforms/STM32/GPIOv2/subdir.mk
-include chibios/os/hal/platforms/STM32/subdir.mk
-include chibios/ext/fatfs/src/option/subdir.mk
-include chibios/ext/fatfs/src/subdir.mk
-include chibios/boards/ST_STM32F4_DISCOVERY/subdir.mk
-include build/obj/subdir.mk
-include build/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
rusefi.hex \

SECONDARY_SIZE += \
rusefi.siz \


# All Target
all: rusefi.elf secondary-outputs

# Tool invocations
rusefi.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -T "..\config\stm32f4ems\STM32F407xG_CCM.ld" -Xlinker --gc-sections -Wl,-Map,"rusefi.map" -Wl,-Map,"rusefi.map" -mcpu=cortex-m4 -mthumb -g3 -o "rusefi.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

rusefi.hex: rusefi.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "rusefi.elf"  "rusefi.hex"
	@echo 'Finished building: $@'
	@echo ' '

rusefi.siz: rusefi.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "rusefi.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(SECONDARY_FLASH)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)$(S_UPPER_DEPS) rusefi.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
