################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../boards.cpp \
../engine_test_helper.cpp \
../main.cpp \
../map_resize.cpp \
../test_accel_enrichment.cpp \
../test_engine_math.cpp \
../test_fuel_map.cpp \
../test_idle_controller.cpp \
../test_logic_expression.cpp \
../test_sensors.cpp \
../test_signal_executor.cpp \
../test_speed_density.cpp \
../test_trigger_decoder.cpp \
../test_util.cpp 

OBJS += \
./boards.o \
./engine_test_helper.o \
./main.o \
./map_resize.o \
./test_accel_enrichment.o \
./test_engine_math.o \
./test_fuel_map.o \
./test_idle_controller.o \
./test_logic_expression.o \
./test_sensors.o \
./test_signal_executor.o \
./test_speed_density.o \
./test_trigger_decoder.o \
./test_util.o 

CPP_DEPS += \
./boards.d \
./engine_test_helper.d \
./main.d \
./map_resize.d \
./test_accel_enrichment.d \
./test_engine_math.d \
./test_fuel_map.d \
./test_idle_controller.d \
./test_logic_expression.d \
./test_sensors.d \
./test_signal_executor.d \
./test_speed_density.d \
./test_trigger_decoder.d \
./test_util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


