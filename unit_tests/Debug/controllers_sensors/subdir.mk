################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../controllers_sensors/allsensors.cpp \
../controllers_sensors/ego.cpp \
../controllers_sensors/hip9011_lookup.cpp \
../controllers_sensors/maf.cpp \
../controllers_sensors/map.cpp \
../controllers_sensors/thermistors.cpp \
../controllers_sensors/tps.cpp \
../controllers_sensors/voltage.cpp 

OBJS += \
./controllers_sensors/allsensors.o \
./controllers_sensors/ego.o \
./controllers_sensors/hip9011_lookup.o \
./controllers_sensors/maf.o \
./controllers_sensors/map.o \
./controllers_sensors/thermistors.o \
./controllers_sensors/tps.o \
./controllers_sensors/voltage.o 

CPP_DEPS += \
./controllers_sensors/allsensors.d \
./controllers_sensors/ego.d \
./controllers_sensors/hip9011_lookup.d \
./controllers_sensors/maf.d \
./controllers_sensors/map.d \
./controllers_sensors/thermistors.d \
./controllers_sensors/tps.d \
./controllers_sensors/voltage.d 


# Each subdirectory must supply rules for building sources it contributes
controllers_sensors/%.o: ../controllers_sensors/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


