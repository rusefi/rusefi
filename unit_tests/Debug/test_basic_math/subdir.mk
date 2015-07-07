################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test_basic_math/test_find_index.cpp \
../test_basic_math/test_interpolation_3d.cpp 

OBJS += \
./test_basic_math/test_find_index.o \
./test_basic_math/test_interpolation_3d.o 

CPP_DEPS += \
./test_basic_math/test_find_index.d \
./test_basic_math/test_interpolation_3d.d 


# Each subdirectory must supply rules for building sources it contributes
test_basic_math/%.o: ../test_basic_math/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


