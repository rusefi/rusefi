################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../hw_layer/HIP9011.cpp \
../hw_layer/adc_inputs.cpp \
../hw_layer/board_test.cpp \
../hw_layer/can_hw.cpp \
../hw_layer/gpio_helper.cpp \
../hw_layer/hardware.cpp \
../hw_layer/io_pins.cpp \
../hw_layer/joystick.cpp \
../hw_layer/max31855.cpp \
../hw_layer/microsecond_timer.cpp \
../hw_layer/mmc_card.cpp \
../hw_layer/neo6m.cpp \
../hw_layer/pin_repository.cpp \
../hw_layer/pwm_generator.cpp \
../hw_layer/servo.cpp \
../hw_layer/stepper.cpp \
../hw_layer/trigger_input.cpp \
../hw_layer/vehicle_speed.cpp \
../hw_layer/wave_analyzer_hw.cpp 

C_SRCS += \
../hw_layer/flash.c \
../hw_layer/mcp3208.c \
../hw_layer/rtc_helper.c 

OBJS += \
./hw_layer/HIP9011.o \
./hw_layer/adc_inputs.o \
./hw_layer/board_test.o \
./hw_layer/can_hw.o \
./hw_layer/flash.o \
./hw_layer/gpio_helper.o \
./hw_layer/hardware.o \
./hw_layer/io_pins.o \
./hw_layer/joystick.o \
./hw_layer/max31855.o \
./hw_layer/mcp3208.o \
./hw_layer/microsecond_timer.o \
./hw_layer/mmc_card.o \
./hw_layer/neo6m.o \
./hw_layer/pin_repository.o \
./hw_layer/pwm_generator.o \
./hw_layer/rtc_helper.o \
./hw_layer/servo.o \
./hw_layer/stepper.o \
./hw_layer/trigger_input.o \
./hw_layer/vehicle_speed.o \
./hw_layer/wave_analyzer_hw.o 

C_DEPS += \
./hw_layer/flash.d \
./hw_layer/mcp3208.d \
./hw_layer/rtc_helper.d 

CPP_DEPS += \
./hw_layer/HIP9011.d \
./hw_layer/adc_inputs.d \
./hw_layer/board_test.d \
./hw_layer/can_hw.d \
./hw_layer/gpio_helper.d \
./hw_layer/hardware.d \
./hw_layer/io_pins.d \
./hw_layer/joystick.d \
./hw_layer/max31855.d \
./hw_layer/microsecond_timer.d \
./hw_layer/mmc_card.d \
./hw_layer/neo6m.d \
./hw_layer/pin_repository.d \
./hw_layer/pwm_generator.d \
./hw_layer/servo.d \
./hw_layer/stepper.d \
./hw_layer/trigger_input.d \
./hw_layer/vehicle_speed.d \
./hw_layer/wave_analyzer_hw.d 


# Each subdirectory must supply rules for building sources it contributes
hw_layer/%.o: ../hw_layer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -DCORTEX_USE_FPU=TRUE -I".." -I"../hw_layer/stm32f4" -I"../chibios/os/kernel/include" -I"../ext" -I"../ext_algo" -I"../chibios/os/various" -I"../hw_layer/algo" -I"../util" -I"../console" -I"../console/binary" -I"../config" -I"../config/engines" -I"../config/boards" -I"../controllers" -I"../controllers/algo" -I"../controllers/core" -I"../controllers/trigger" -I"../controllers/sensors" -I"../controllers/system" -I"../controllers/math" -I"../console_util" -I"../development" -I"../development/test" -I"../development/hw_layer" -I"../hw_layer" -I"../hw_layer/flash" -I"../hw_layer/lcd" -I"../hw_layer/serial_over_usb" -I"../russianefi" -I"../russianefi/algo" -I"../russianefi/ckp" -I"../russianefi/adc" -I"../config/system" -I"../config/stm32f4ems" -I"../splib" -I"../chibios/os/kernel/include" -I"../chibios/boards/ST_STM32F4_DISCOVERY" -I"../chibios/os/hal/src" -I"../chibios/os/hal/include" -I"../chibios/os/hal/platforms/STM32/TIMv1" -I"../chibios/os/hal/platforms/STM32/I2Cv1" -I"../chibios/os/hal/platforms/STM32" -I"../chibios/os/hal/platforms/STM32/RTCv2" -I"../chibios/os/hal/platforms/STM32/OTGv1" -I"../chibios/os/hal/platforms/STM32/GPIOv2" -I"../chibios/os/hal/platforms/STM32F4xx" -I"../chibios/os/hal/platforms/STM32/SPIv1" -I"../chibios/os/hal/platforms/STM32/USARTv1" -I"../chibios/os/ports/GCC/ARMCMx" -I"../chibios/os/ports/GCC/ARMCMx/STM32F4xx" -I"../chibios/os/ports/common/ARMCMx" -I../chibios/os/ports/common/ARMCMx/CMSIS/include -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fgnu89-inline -c -Werror=type-limits -Werror=uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

hw_layer/%.o: ../hw_layer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -DCORTEX_USE_FPU=TRUE -I".." -I"../hw_layer/stm32f4" -I../chibios/os/kernel/include -I"../ext" -I"../ext_algo" -I"../chibios/os/various" -I"../hw_layer/algo" -I"../util" -I"../console" -I"../console/binary" -I"../config" -I"../config/engines" -I"../config/boards" -I"../controllers" -I"../controllers/algo" -I"../controllers/core" -I"../controllers/trigger" -I"../controllers/sensors" -I"../controllers/system" -I"../controllers/math" -I"../console_util" -I"../development" -I"../development/test" -I"../development/hw_layer" -I"../hw_layer" -I"../hw_layer/flash" -I"../hw_layer/lcd" -I"../hw_layer/serial_over_usb" -I"../russianefi" -I"../russianefi/algo" -I"../russianefi/ckp" -I"../russianefi/adc" -I"../config/system" -I"../config/stm32f4ems" -I"../splib" -I"../chibios/os/kernel/include" -I"../chibios/boards/ST_STM32F4_DISCOVERY" -I"../chibios/os/hal/src" -I"../chibios/os/hal/include" -I"../chibios/os/hal/platforms/STM32/TIMv1" -I"../chibios/os/hal/platforms/STM32/I2Cv1" -I"../chibios/os/hal/platforms/STM32" -I"../chibios/os/hal/platforms/STM32/RTCv2" -I"../chibios/os/hal/platforms/STM32/OTGv1" -I"../chibios/os/hal/platforms/STM32/GPIOv2" -I"../chibios/os/hal/platforms/STM32F4xx" -I"../chibios/os/hal/platforms/STM32/SPIv1" -I"../chibios/os/hal/platforms/STM32/USARTv1" -I"../chibios/os/ports/GCC/ARMCMx" -I"../chibios/os/ports/GCC/ARMCMx/STM32F4xx" -I"../chibios/os/ports/common/ARMCMx" -I"../chibios/os/ports/common/ARMCMx/CMSIS/include" -std=gnu11 -fgnu89-inline -c -Werror=type-limits -Werror=uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


