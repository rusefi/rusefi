################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../chibios/os/various/cpp_wrappers/ch.cpp 

OBJS += \
./chibios/os/various/cpp_wrappers/ch.o 

CPP_DEPS += \
./chibios/os/various/cpp_wrappers/ch.d 


# Each subdirectory must supply rules for building sources it contributes
chibios/os/various/cpp_wrappers/%.o: ../chibios/os/various/cpp_wrappers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -DCORTEX_USE_FPU=TRUE -I".." -I"../hw_layer/stm32f4" -I"../chibios/os/kernel/include" -I"../ext" -I"../ext_algo" -I"../chibios/os/various" -I"../hw_layer/algo" -I"../util" -I"../console" -I"../console/binary" -I"../config" -I"../config/engines" -I"../config/boards" -I"../controllers" -I"../controllers/algo" -I"../controllers/core" -I"../controllers/trigger" -I"../controllers/sensors" -I"../controllers/system" -I"../controllers/math" -I"../console_util" -I"../development" -I"../development/test" -I"../development/hw_layer" -I"../hw_layer" -I"../hw_layer/flash" -I"../hw_layer/lcd" -I"../hw_layer/serial_over_usb" -I"../russianefi" -I"../russianefi/algo" -I"../russianefi/ckp" -I"../russianefi/adc" -I"../config/system" -I"../config/stm32f4ems" -I"../splib" -I"../chibios/os/kernel/include" -I"../chibios/boards/ST_STM32F4_DISCOVERY" -I"../chibios/os/hal/src" -I"../chibios/os/hal/include" -I"../chibios/os/hal/platforms/STM32/TIMv1" -I"../chibios/os/hal/platforms/STM32/I2Cv1" -I"../chibios/os/hal/platforms/STM32" -I"../chibios/os/hal/platforms/STM32/RTCv2" -I"../chibios/os/hal/platforms/STM32/OTGv1" -I"../chibios/os/hal/platforms/STM32/GPIOv2" -I"../chibios/os/hal/platforms/STM32F4xx" -I"../chibios/os/hal/platforms/STM32/SPIv1" -I"../chibios/os/hal/platforms/STM32/USARTv1" -I"../chibios/os/ports/GCC/ARMCMx" -I"../chibios/os/ports/GCC/ARMCMx/STM32F4xx" -I"../chibios/os/ports/common/ARMCMx" -I../chibios/os/ports/common/ARMCMx/CMSIS/include -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fgnu89-inline -c -Werror=type-limits -Werror=uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


