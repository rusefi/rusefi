################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../controllers_algo/accel_enrichment.cpp \
../controllers_algo/advance_map.cpp \
../controllers_algo/algo.cpp \
../controllers_algo/auto_generated_enums.cpp \
../controllers_algo/engine.cpp \
../controllers_algo/engine_configuration.cpp \
../controllers_algo/event_registry.cpp \
../controllers_algo/fuel_math.cpp \
../controllers_algo/idle_controller.cpp \
../controllers_algo/lcd_menu_tree.cpp \
../controllers_algo/signal_executor.cpp 

C_SRCS += \
../controllers_algo/malfunction_central.c \
../controllers_algo/map_adjuster.c \
../controllers_algo/nmea.c 

OBJS += \
./controllers_algo/accel_enrichment.o \
./controllers_algo/advance_map.o \
./controllers_algo/algo.o \
./controllers_algo/auto_generated_enums.o \
./controllers_algo/engine.o \
./controllers_algo/engine_configuration.o \
./controllers_algo/event_registry.o \
./controllers_algo/fuel_math.o \
./controllers_algo/idle_controller.o \
./controllers_algo/lcd_menu_tree.o \
./controllers_algo/malfunction_central.o \
./controllers_algo/map_adjuster.o \
./controllers_algo/nmea.o \
./controllers_algo/signal_executor.o 

C_DEPS += \
./controllers_algo/malfunction_central.d \
./controllers_algo/map_adjuster.d \
./controllers_algo/nmea.d 

CPP_DEPS += \
./controllers_algo/accel_enrichment.d \
./controllers_algo/advance_map.d \
./controllers_algo/algo.d \
./controllers_algo/auto_generated_enums.d \
./controllers_algo/engine.d \
./controllers_algo/engine_configuration.d \
./controllers_algo/event_registry.d \
./controllers_algo/fuel_math.d \
./controllers_algo/idle_controller.d \
./controllers_algo/lcd_menu_tree.d \
./controllers_algo/signal_executor.d 


# Each subdirectory must supply rules for building sources it contributes
controllers_algo/%.o: ../controllers_algo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

controllers_algo/%.o: ../controllers_algo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -std=gnu99 -fgnu89-inline -Werror-implicit-function-declaration -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


