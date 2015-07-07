################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../controllers_core/EfiWave.cpp \
../controllers_core/avg_values.cpp \
../controllers_core/fsio_core.cpp \
../controllers_core/fsio_impl.cpp \
../controllers_core/interpolation.cpp \
../controllers_core/table_helper.cpp 

C_SRCS += \
../controllers_core/signal_filtering.c 

OBJS += \
./controllers_core/EfiWave.o \
./controllers_core/avg_values.o \
./controllers_core/fsio_core.o \
./controllers_core/fsio_impl.o \
./controllers_core/interpolation.o \
./controllers_core/signal_filtering.o \
./controllers_core/table_helper.o 

C_DEPS += \
./controllers_core/signal_filtering.d 

CPP_DEPS += \
./controllers_core/EfiWave.d \
./controllers_core/avg_values.d \
./controllers_core/fsio_core.d \
./controllers_core/fsio_impl.d \
./controllers_core/interpolation.d \
./controllers_core/table_helper.d 


# Each subdirectory must supply rules for building sources it contributes
controllers_core/%.o: ../controllers_core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

controllers_core/%.o: ../controllers_core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -std=gnu99 -fgnu89-inline -Werror-implicit-function-declaration -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


