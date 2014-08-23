/*
 * @file	tunerstudio_configuration.h
 * @brief	Tuner Studio connectivity configuration
 *
 * In this file the configuration of TunerStudio is defined
 *
 * @date Oct 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TUNERSTUDIO_CONFIGURATION_H_
#define TUNERSTUDIO_CONFIGURATION_H_

/**
 * please be aware that current "stable" version of TunerStudio does not
 * support 'float' (F32) type. You would need a beta version to handle floats
 */
typedef struct {
	// primary instrument cluster gauges
	int rpm; // size 4, offset 0
	float coolant_temperature; // size 4, offset 4
	float intake_air_temperature; // size 4, offset 8
	float throttle_positon; // size 4, offset 12
	float mass_air_flow; // size 4, offset 16
	float air_fuel_ratio; // size 4, offset 20
	float fuel_load; // size 4, offset 24
	float v_batt; // size 4, offset 28
	short int tpsADC; // size 2, offset 32
	short int alignment; // size 2, offset 34
	float atmospherePressure; // size 4, offset 36
	float manifold_air_pressure; // size 4, offset 40
	float crankingFuel;
	int tpsVolrage;
	float tCharge; // 52
	float inj_adv; // 56
	float sparkDwell; // 60
	float pulseWidth; // 64
	float warmUpEnrich;	// 68
	/**
	 * Yes, I do not really enjoy packing bits into integers but we simply have too many boolean flags and I cannot
	 * water 4 bytes per trafic - I want gauges to work as fast as possible
	 */
	unsigned int hasSdCard : 1; // bit 0
	unsigned int ignition_enabled : 1; // bit 1
	unsigned int injection_enabled : 1; // bit 2
	unsigned int cylinder_cleanup_enabled : 1; // bit 3
	unsigned int cylinder_cleanup : 1; // bit 4
	unsigned int isFuelPumpOn : 1; // bit 5
	unsigned int isFanOn : 1; // bit 6
	unsigned int isO2HeaterOn : 1; // bit 7
	unsigned int checkEngine : 1; // bit 8
	unsigned int needBurn : 1; // bit 9
	unsigned int secondTriggerChannelEnabled : 1; // bit 10
	int unused[10];
} TunerStudioOutputChannels;

#endif /* TUNERSTUDIO_CONFIGURATION_H_ */
