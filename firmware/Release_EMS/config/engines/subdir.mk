################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../config/engines/GY6_139QMB.cpp \
../config/engines/MiniCooperR50.cpp \
../config/engines/acura_rsx.cpp \
../config/engines/audi_aan.cpp \
../config/engines/bmw_e34.cpp \
../config/engines/citroenBerlingoTU3JP.cpp \
../config/engines/custom_engine.cpp \
../config/engines/dodge_neon.cpp \
../config/engines/dodge_ram.cpp \
../config/engines/ford_1995_inline_6.cpp \
../config/engines/ford_aspire.cpp \
../config/engines/ford_fiesta.cpp \
../config/engines/gm_2_2.cpp \
../config/engines/honda_accord.cpp \
../config/engines/mazda_323.cpp \
../config/engines/mazda_626.cpp \
../config/engines/mazda_miata.cpp \
../config/engines/mazda_miata_nb.cpp \
../config/engines/mitsubishi.cpp \
../config/engines/nissan_primera.cpp \
../config/engines/rover_v8.cpp \
../config/engines/sachs.cpp \
../config/engines/saturn_ion.cpp \
../config/engines/snow_blower.cpp \
../config/engines/subaru.cpp \
../config/engines/test_engine.cpp 

OBJS += \
./config/engines/GY6_139QMB.o \
./config/engines/MiniCooperR50.o \
./config/engines/acura_rsx.o \
./config/engines/audi_aan.o \
./config/engines/bmw_e34.o \
./config/engines/citroenBerlingoTU3JP.o \
./config/engines/custom_engine.o \
./config/engines/dodge_neon.o \
./config/engines/dodge_ram.o \
./config/engines/ford_1995_inline_6.o \
./config/engines/ford_aspire.o \
./config/engines/ford_fiesta.o \
./config/engines/gm_2_2.o \
./config/engines/honda_accord.o \
./config/engines/mazda_323.o \
./config/engines/mazda_626.o \
./config/engines/mazda_miata.o \
./config/engines/mazda_miata_nb.o \
./config/engines/mitsubishi.o \
./config/engines/nissan_primera.o \
./config/engines/rover_v8.o \
./config/engines/sachs.o \
./config/engines/saturn_ion.o \
./config/engines/snow_blower.o \
./config/engines/subaru.o \
./config/engines/test_engine.o 

CPP_DEPS += \
./config/engines/GY6_139QMB.d \
./config/engines/MiniCooperR50.d \
./config/engines/acura_rsx.d \
./config/engines/audi_aan.d \
./config/engines/bmw_e34.d \
./config/engines/citroenBerlingoTU3JP.d \
./config/engines/custom_engine.d \
./config/engines/dodge_neon.d \
./config/engines/dodge_ram.d \
./config/engines/ford_1995_inline_6.d \
./config/engines/ford_aspire.d \
./config/engines/ford_fiesta.d \
./config/engines/gm_2_2.d \
./config/engines/honda_accord.d \
./config/engines/mazda_323.d \
./config/engines/mazda_626.d \
./config/engines/mazda_miata.d \
./config/engines/mazda_miata_nb.d \
./config/engines/mitsubishi.d \
./config/engines/nissan_primera.d \
./config/engines/rover_v8.d \
./config/engines/sachs.d \
./config/engines/saturn_ion.d \
./config/engines/snow_blower.d \
./config/engines/subaru.d \
./config/engines/test_engine.d 


# Each subdirectory must supply rules for building sources it contributes
config/engines/%.o: ../config/engines/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto -DCORTEX_USE_FPU=TRUE -I".." -I"../hw_layer/stm32f4" -I"../chibios/os/kernel/include" -I"../ext" -I"../ext_algo" -I"../chibios/os/various" -I"../hw_layer/algo" -I"../util" -I"../console" -I"../console/binary" -I"../config" -I"../config/engines" -I"../config/boards" -I"../controllers" -I"../controllers/algo" -I"../controllers/core" -I"../controllers/trigger" -I"../controllers/sensors" -I"../controllers/system" -I"../controllers/math" -I"../console_util" -I"../development" -I"../development/test" -I"../development/hw_layer" -I"../hw_layer" -I"../hw_layer/flash" -I"../hw_layer/lcd" -I"../hw_layer/serial_over_usb" -I"../russianefi" -I"../russianefi/algo" -I"../russianefi/ckp" -I"../russianefi/adc" -I"../config/system" -I"../config/stm32f4ems" -I"../splib" -I"../chibios/os/kernel/include" -I"../chibios/boards/ST_STM32F4_DISCOVERY" -I"../chibios/os/hal/src" -I"../chibios/os/hal/include" -I"../chibios/os/hal/platforms/STM32/TIMv1" -I"../chibios/os/hal/platforms/STM32/I2Cv1" -I"../chibios/os/hal/platforms/STM32" -I"../chibios/os/hal/platforms/STM32/RTCv2" -I"../chibios/os/hal/platforms/STM32/OTGv1" -I"../chibios/os/hal/platforms/STM32/GPIOv2" -I"../chibios/os/hal/platforms/STM32F4xx" -I"../chibios/os/hal/platforms/STM32/SPIv1" -I"../chibios/os/hal/platforms/STM32/USARTv1" -I"../chibios/os/ports/GCC/ARMCMx" -I"../chibios/os/ports/GCC/ARMCMx/STM32F4xx" -I"../chibios/os/ports/common/ARMCMx" -I../chibios/os/ports/common/ARMCMx/CMSIS/include -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fgnu89-inline -c -Werror=type-limits -Werror=uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


