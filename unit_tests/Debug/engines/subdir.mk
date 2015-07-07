################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../engines/GY6_139QMB.cpp \
../engines/MiniCooperR50.cpp \
../engines/acura_rsx.cpp \
../engines/audi_aan.cpp \
../engines/bmw_e34.cpp \
../engines/citroenBerlingoTU3JP.cpp \
../engines/custom_engine.cpp \
../engines/dodge_neon.cpp \
../engines/dodge_ram.cpp \
../engines/ford_1995_inline_6.cpp \
../engines/ford_aspire.cpp \
../engines/ford_fiesta.cpp \
../engines/gm_2_2.cpp \
../engines/honda_accord.cpp \
../engines/mazda_323.cpp \
../engines/mazda_626.cpp \
../engines/mazda_miata.cpp \
../engines/mazda_miata_nb.cpp \
../engines/mitsubishi.cpp \
../engines/nissan_primera.cpp \
../engines/rover_v8.cpp \
../engines/sachs.cpp \
../engines/saturn_ion.cpp \
../engines/snow_blower.cpp \
../engines/subaru.cpp \
../engines/test_engine.cpp \
../engines/vw.cpp 

OBJS += \
./engines/GY6_139QMB.o \
./engines/MiniCooperR50.o \
./engines/acura_rsx.o \
./engines/audi_aan.o \
./engines/bmw_e34.o \
./engines/citroenBerlingoTU3JP.o \
./engines/custom_engine.o \
./engines/dodge_neon.o \
./engines/dodge_ram.o \
./engines/ford_1995_inline_6.o \
./engines/ford_aspire.o \
./engines/ford_fiesta.o \
./engines/gm_2_2.o \
./engines/honda_accord.o \
./engines/mazda_323.o \
./engines/mazda_626.o \
./engines/mazda_miata.o \
./engines/mazda_miata_nb.o \
./engines/mitsubishi.o \
./engines/nissan_primera.o \
./engines/rover_v8.o \
./engines/sachs.o \
./engines/saturn_ion.o \
./engines/snow_blower.o \
./engines/subaru.o \
./engines/test_engine.o \
./engines/vw.o 

CPP_DEPS += \
./engines/GY6_139QMB.d \
./engines/MiniCooperR50.d \
./engines/acura_rsx.d \
./engines/audi_aan.d \
./engines/bmw_e34.d \
./engines/citroenBerlingoTU3JP.d \
./engines/custom_engine.d \
./engines/dodge_neon.d \
./engines/dodge_ram.d \
./engines/ford_1995_inline_6.d \
./engines/ford_aspire.d \
./engines/ford_fiesta.d \
./engines/gm_2_2.d \
./engines/honda_accord.d \
./engines/mazda_323.d \
./engines/mazda_626.d \
./engines/mazda_miata.d \
./engines/mazda_miata_nb.d \
./engines/mitsubishi.d \
./engines/nissan_primera.d \
./engines/rover_v8.d \
./engines/sachs.d \
./engines/saturn_ion.d \
./engines/snow_blower.d \
./engines/subaru.d \
./engines/test_engine.d \
./engines/vw.d 


# Each subdirectory must supply rules for building sources it contributes
engines/%.o: ../engines/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_basic_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/test_data_structures" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/engines" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo/nmea" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/ext_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_math" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_sensors" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_algo" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_trigger" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_core" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/controllers_system" -I"/cygdrive/c/stuff/rusefi_sourceforge/unit_tests/util" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


