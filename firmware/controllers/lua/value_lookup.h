/*
 * @file value_lookup.h
 *
 * @date Dec 13, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once
#include "pch.h"
#include "livedata_board_extra.h"

struct plain_get_integer_s {
	const char *token;
	int *value;
};

struct plain_get_short_s {
	const char *token;
	uint16_t *value;
};

struct plain_get_u8_s {
	const char *token;
	uint8_t *value;
};

struct plain_get_float_s {
	const char *token;
	float *value;
};

float getOutputValueByHash(const int hash);

float getConfigValueByHash(const int hash);
float getConfigValueByName(const char *name);
/**
 * @return true if name was recognized, false otherwise
 */
bool setConfigValueByName(const char *name, float value);
float getOutputValueByName(const char *name);

void * hackEngineConfigurationPointer(void *ptr);
