################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../util/LocalVersionHolder.cpp \
../util/cli_registry.cpp \
../util/cyclic_buffer.cpp \
../util/datalogging.cpp \
../util/efilib.cpp \
../util/efilib2.cpp \
../util/listener_array.cpp \
../util/loggingcentral.cpp 

C_SRCS += \
../util/crc.c \
../util/data_buffer.c \
../util/histogram.c 

OBJS += \
./util/LocalVersionHolder.o \
./util/cli_registry.o \
./util/crc.o \
./util/cyclic_buffer.o \
./util/data_buffer.o \
./util/datalogging.o \
./util/efilib.o \
./util/efilib2.o \
./util/histogram.o \
./util/listener_array.o \
./util/loggingcentral.o 

C_DEPS += \
./util/crc.d \
./util/data_buffer.d \
./util/histogram.d 

CPP_DEPS += \
./util/LocalVersionHolder.d \
./util/cli_registry.d \
./util/cyclic_buffer.d \
./util/datalogging.d \
./util/efilib.d \
./util/efilib2.d \
./util/listener_array.d \
./util/loggingcentral.d 


# Each subdirectory must supply rules for building sources it contributes
util/%.o: ../util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

util/%.o: ../util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -std=gnu99 -fgnu89-inline -Werror-implicit-function-declaration -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


