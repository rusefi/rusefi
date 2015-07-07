################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../controllers_trigger/main_trigger_callback.cpp \
../controllers_trigger/rpm_calculator.cpp \
../controllers_trigger/trigger_bmw.cpp \
../controllers_trigger/trigger_central.cpp \
../controllers_trigger/trigger_chrysler.cpp \
../controllers_trigger/trigger_decoder.cpp \
../controllers_trigger/trigger_gm.cpp \
../controllers_trigger/trigger_mazda.cpp \
../controllers_trigger/trigger_mitsubishi.cpp \
../controllers_trigger/trigger_structure.cpp 

OBJS += \
./controllers_trigger/main_trigger_callback.o \
./controllers_trigger/rpm_calculator.o \
./controllers_trigger/trigger_bmw.o \
./controllers_trigger/trigger_central.o \
./controllers_trigger/trigger_chrysler.o \
./controllers_trigger/trigger_decoder.o \
./controllers_trigger/trigger_gm.o \
./controllers_trigger/trigger_mazda.o \
./controllers_trigger/trigger_mitsubishi.o \
./controllers_trigger/trigger_structure.o 

CPP_DEPS += \
./controllers_trigger/main_trigger_callback.d \
./controllers_trigger/rpm_calculator.d \
./controllers_trigger/trigger_bmw.d \
./controllers_trigger/trigger_central.d \
./controllers_trigger/trigger_chrysler.d \
./controllers_trigger/trigger_decoder.d \
./controllers_trigger/trigger_gm.d \
./controllers_trigger/trigger_mazda.d \
./controllers_trigger/trigger_mitsubishi.d \
./controllers_trigger/trigger_structure.d 


# Each subdirectory must supply rules for building sources it contributes
controllers_trigger/%.o: ../controllers_trigger/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


