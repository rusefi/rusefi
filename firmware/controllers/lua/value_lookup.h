/*
 * @file value_lookup.h
 *
 * @date Dec 13, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#pragma once

struct plain_get_integer_s {
	const char *token;
	int *value;
};

struct plain_get_short_s {
	const char *token;
	uint16_t *value;
};

struct plain_get_float_s {
	const char *token;
	float *value;
};

plain_get_float_s * findFloat(const char *name);

float getConfigValueByName(const char *name);
void setConfigValueByName(const char *name, float value);
float getOutputValueByName(const char *name);

void * hackEngineConfigurationPointer(void *ptr);
