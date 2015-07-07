################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../controllers/core/EfiWave.cpp \
../controllers/core/avg_values.cpp \
../controllers/core/fsio_core.cpp \
../controllers/core/fsio_impl.cpp \
../controllers/core/interpolation.cpp \
../controllers/core/table_helper.cpp 

C_SRCS += \
../controllers/core/signal_filtering.c 

OBJS += \
./controllers/core/EfiWave.o \
./controllers/core/avg_values.o \
./controllers/core/fsio_core.o \
./controllers/core/fsio_impl.o \
./controllers/core/interpolation.o \
./controllers/core/signal_filtering.o \
./controllers/core/table_helper.o 

C_DEPS += \
./controllers/core/signal_filtering.d 

CPP_DEPS += \
./controllers/core/EfiWave.d \
./controllers/core/avg_values.d \
./controllers/core/fsio_core.d \
./controllers/core/fsio_impl.d \
./controllers/core/interpolation.d \
./controllers/core/table_helper.d 


# Each subdirectory must supply rules for building sources it contributes
controllers/core/%.o: ../controllers/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -DCORTEX_USE_FPU=TRUE -I".." -I"../hw_layer/stm32f4" -I"../chibios/os/kernel/include" -I"../ext" -I"../ext_algo" -I"../chibios/os/various" -I"../hw_layer/algo" -I"../util" -I"../console" -I"../console/binary" -I"../config" -I"../config/engines" -I"../config/boards" -I"../controllers" -I"../controllers/algo" -I"../controllers/core" -I"../controllers/trigger" -I"../controllers/sensors" -I"../controllers/system" -I"../controllers/math" -I"../console_util" -I"../development" -I"../development/test" -I"../development/hw_layer" -I"../hw_layer" -I"../hw_layer/flash" -I"../hw_layer/lcd" -I"../hw_layer/serial_over_usb" -I"../russianefi" -I"../russianefi/algo" -I"../russianefi/ckp" -I"../russianefi/adc" -I"../config/system" -I"../config/stm32f4ems" -I"../splib" -I"../chibios/os/kernel/include" -I"../chibios/boards/ST_STM32F4_DISCOVERY" -I"../chibios/os/hal/src" -I"../chibios/os/hal/include" -I"../chibios/os/hal/platforms/STM32/TIMv1" -I"../chibios/os/hal/platforms/STM32/I2Cv1" -I"../chibios/os/hal/platforms/STM32" -I"../chibios/os/hal/platforms/STM32/RTCv2" -I"../chibios/os/hal/platforms/STM32/OTGv1" -I"../chibios/os/hal/platforms/STM32/GPIOv2" -I"../chibios/os/hal/platforms/STM32F4xx" -I"../chibios/os/hal/platforms/STM32/SPIv1" -I"../chibios/os/hal/platforms/STM32/USARTv1" -I"../chibios/os/ports/GCC/ARMCMx" -I"../chibios/os/ports/GCC/ARMCMx/STM32F4xx" -I"../chibios/os/ports/common/ARMCMx" -I../chibios/os/ports/common/ARMCMx/CMSIS/include -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fgnu89-inline -c -Werror=type-limits -Werror=uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

controllers/core/%.o: ../controllers/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -DCORTEX_USE_FPU=TRUE -I".." -I"../hw_layer/stm32f4" -I../chibios/os/kernel/include -I"../ext" -I"../ext_algo" -I"../chibios/os/various" -I"../hw_layer/algo" -I"../util" -I"../console" -I"../console/binary" -I"../config" -I"../config/engines" -I"../config/boards" -I"../controllers" -I"../controllers/algo" -I"../controllers/core" -I"../controllers/trigger" -I"../controllers/sensors" -I"../controllers/system" -I"../controllers/math" -I"../console_util" -I"../development" -I"../development/test" -I"../development/hw_layer" -I"../hw_layer" -I"../hw_layer/flash" -I"../hw_layer/lcd" -I"../hw_layer/serial_over_usb" -I"../russianefi" -I"../russianefi/algo" -I"../russianefi/ckp" -I"../russianefi/adc" -I"../config/system" -I"../config/stm32f4ems" -I"../splib" -I"../chibios/os/kernel/include" -I"../chibios/boards/ST_STM32F4_DISCOVERY" -I"../chibios/os/hal/src" -I"../chibios/os/hal/include" -I"../chibios/os/hal/platforms/STM32/TIMv1" -I"../chibios/os/hal/platforms/STM32/I2Cv1" -I"../chibios/os/hal/platforms/STM32" -I"../chibios/os/hal/platforms/STM32/RTCv2" -I"../chibios/os/hal/platforms/STM32/OTGv1" -I"../chibios/os/hal/platforms/STM32/GPIOv2" -I"../chibios/os/hal/platforms/STM32F4xx" -I"../chibios/os/hal/platforms/STM32/SPIv1" -I"../chibios/os/hal/platforms/STM32/USARTv1" -I"../chibios/os/ports/GCC/ARMCMx" -I"../chibios/os/ports/GCC/ARMCMx/STM32F4xx" -I"../chibios/os/ports/common/ARMCMx" -I"../chibios/os/ports/common/ARMCMx/CMSIS/include" -std=gnu11 -fgnu89-inline -c -Werror=type-limits -Werror=uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


