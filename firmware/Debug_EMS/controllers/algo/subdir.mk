################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../controllers/algo/accel_enrichment.cpp \
../controllers/algo/advance_map.cpp \
../controllers/algo/algo.cpp \
../controllers/algo/auto_generated_enums.cpp \
../controllers/algo/engine.cpp \
../controllers/algo/engine_configuration.cpp \
../controllers/algo/event_registry.cpp \
../controllers/algo/fuel_math.cpp \
../controllers/algo/idle_controller.cpp \
../controllers/algo/lcd_menu_tree.cpp \
../controllers/algo/signal_executor.cpp 

C_SRCS += \
../controllers/algo/malfunction_central.c \
../controllers/algo/map_adjuster.c \
../controllers/algo/nmea.c 

OBJS += \
./controllers/algo/accel_enrichment.o \
./controllers/algo/advance_map.o \
./controllers/algo/algo.o \
./controllers/algo/auto_generated_enums.o \
./controllers/algo/engine.o \
./controllers/algo/engine_configuration.o \
./controllers/algo/event_registry.o \
./controllers/algo/fuel_math.o \
./controllers/algo/idle_controller.o \
./controllers/algo/lcd_menu_tree.o \
./controllers/algo/malfunction_central.o \
./controllers/algo/map_adjuster.o \
./controllers/algo/nmea.o \
./controllers/algo/signal_executor.o 

C_DEPS += \
./controllers/algo/malfunction_central.d \
./controllers/algo/map_adjuster.d \
./controllers/algo/nmea.d 

CPP_DEPS += \
./controllers/algo/accel_enrichment.d \
./controllers/algo/advance_map.d \
./controllers/algo/algo.d \
./controllers/algo/auto_generated_enums.d \
./controllers/algo/engine.d \
./controllers/algo/engine_configuration.d \
./controllers/algo/event_registry.d \
./controllers/algo/fuel_math.d \
./controllers/algo/idle_controller.d \
./controllers/algo/lcd_menu_tree.d \
./controllers/algo/signal_executor.d 


# Each subdirectory must supply rules for building sources it contributes
controllers/algo/%.o: ../controllers/algo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DCORTEX_USE_FPU=TRUE -I".." -I"../hw_layer/stm32f4" -I"../chibios/os/kernel/include" -I"../ext" -I"../ext_algo" -I"../chibios/os/various" -I"../hw_layer/algo" -I"../util" -I"../console" -I"../console/binary" -I"../config" -I"../config/engines" -I"../config/boards" -I"../controllers" -I"../controllers/algo" -I"../controllers/core" -I"../controllers/trigger" -I"../controllers/sensors" -I"../controllers/system" -I"../controllers/math" -I"../console_util" -I"../development" -I"../development/test" -I"../development/hw_layer" -I"../hw_layer" -I"../hw_layer/flash" -I"../hw_layer/lcd" -I"../hw_layer/serial_over_usb" -I"../russianefi" -I"../russianefi/algo" -I"../russianefi/ckp" -I"../russianefi/adc" -I"../config/system" -I"../config/stm32f4ems" -I"../splib" -I"../chibios/os/kernel/include" -I"../chibios/boards/ST_STM32F4_DISCOVERY" -I"../chibios/os/hal/src" -I"../chibios/os/hal/include" -I"../chibios/os/hal/platforms/STM32/TIMv1" -I"../chibios/os/hal/platforms/STM32/I2Cv1" -I"../chibios/os/hal/platforms/STM32" -I"../chibios/os/hal/platforms/STM32/RTCv2" -I"../chibios/os/hal/platforms/STM32/OTGv1" -I"../chibios/os/hal/platforms/STM32/GPIOv2" -I"../chibios/os/hal/platforms/STM32F4xx" -I"../chibios/os/hal/platforms/STM32/SPIv1" -I"../chibios/os/hal/platforms/STM32/USARTv1" -I"../chibios/os/ports/GCC/ARMCMx" -I"../chibios/os/ports/GCC/ARMCMx/STM32F4xx" -I"../chibios/os/ports/common/ARMCMx" -I../chibios/os/ports/common/ARMCMx/CMSIS/include -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fgnu89-inline -c -Werror=type-limits -Werror=uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

controllers/algo/%.o: ../controllers/algo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DCORTEX_USE_FPU=TRUE -I".." -I"../hw_layer/stm32f4" -I../chibios/os/kernel/include -I"../ext" -I"../ext_algo" -I"../chibios/os/various" -I"../hw_layer/algo" -I"../util" -I"../console" -I"../console/binary" -I"../config" -I"../config/engines" -I"../config/boards" -I"../controllers" -I"../controllers/algo" -I"../controllers/core" -I"../controllers/trigger" -I"../controllers/sensors" -I"../controllers/system" -I"../controllers/math" -I"../console_util" -I"../development" -I"../development/test" -I"../development/hw_layer" -I"../hw_layer" -I"../hw_layer/flash" -I"../hw_layer/lcd" -I"../hw_layer/serial_over_usb" -I"../russianefi" -I"../russianefi/algo" -I"../russianefi/ckp" -I"../russianefi/adc" -I"../config/system" -I"../config/stm32f4ems" -I"../splib" -I"../chibios/os/kernel/include" -I"../chibios/boards/ST_STM32F4_DISCOVERY" -I"../chibios/os/hal/src" -I"../chibios/os/hal/include" -I"../chibios/os/hal/platforms/STM32/TIMv1" -I"../chibios/os/hal/platforms/STM32/I2Cv1" -I"../chibios/os/hal/platforms/STM32" -I"../chibios/os/hal/platforms/STM32/RTCv2" -I"../chibios/os/hal/platforms/STM32/OTGv1" -I"../chibios/os/hal/platforms/STM32/GPIOv2" -I"../chibios/os/hal/platforms/STM32F4xx" -I"../chibios/os/hal/platforms/STM32/SPIv1" -I"../chibios/os/hal/platforms/STM32/USARTv1" -I"../chibios/os/ports/GCC/ARMCMx" -I"../chibios/os/ports/GCC/ARMCMx/STM32F4xx" -I"../chibios/os/ports/common/ARMCMx" -I"../chibios/os/ports/common/ARMCMx/CMSIS/include" -std=gnu11 -fgnu89-inline -c -Werror=type-limits -Werror=uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


