/**
 * @file	engine_configuration.h
 * @brief	Main engine configuration data structure.
 *
 * @date Oct 30, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef ENGINE_CONFIGURATION_H_
#define ENGINE_CONFIGURATION_H_

#include "efifeatures.h"
#include "crc.h"
#include "can_header.h"
#include "rusefi_enums.h"
#include "global.h"
#include "rusefi_types.h"
#include "engine_configuration_generated_structures.h"

#define MOCK_UNDEFINED -1

// WARNING: by default, our small enums are ONE BYTE. this one is made 4-byte with the 'ENUM_32_BITS' hack

// 19010105 decimal
#define HEADER_MAGIC_NUMBER 0x1221239

/**
 * @brief	Engine configuration.
 * 		Values in this data structure are adjustable and persisted in on-board flash RAM.
 *
 *  The offsets are tracked using
 *  https://docs.google.com/spreadsheet/ccc?key=0AiAmAn6tn3L_dGJXZDZOcVVhaG9SaHZKU1dyMjhEV0E
 *
 *  todo: currently the fields here are simply in the order in which they were implemented
 *  todo: re-arrange this structure one we have a stable code version
 */

void setOperationMode(engine_configuration_s *engineConfiguration, operation_mode_e mode);
operation_mode_e getOperationMode(engine_configuration_s const *engineConfiguration);

typedef struct {
	engine_configuration_s engineConfiguration;
} persistent_config_s;

typedef struct {
	int version;
	int size;
	persistent_config_s persistentConfiguration;
	crc_t value;
} persistent_config_container_s;

void setDefaultConfiguration(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration);
void setMap(fuel_table_t table, float value);
void setWholeFuelMap(engine_configuration_s *engineConfiguration, float value);
void setWholeTimingTable(engine_configuration_s *engineConfiguration, float value);
void setConstantDwell(engine_configuration_s *engineConfiguration, float dwellMs);
void printFloatArray(const char *prefix, float array[], int size);

void incrementGlobalConfigurationVersion(void);
int getGlobalConfigurationVersion(void);

void commonFrankensoAnalogInputs(engine_configuration_s *engineConfiguration);
void setFrankenso0_1_joystick(engine_configuration_s *engineConfiguration);

void copyFuelTable(fuel_table_t const source, fuel_table_t destination);
void copyTimingTable(ignition_table_t const source, ignition_table_t destination);

#endif /* ENGINE_CONFIGURATION_H_ */
