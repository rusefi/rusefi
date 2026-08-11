#include "pch.h"
#include "value_lookup.h"

const ConfigParameter* findParamByHash(uint32_t hash) {
	int low = 0;
	int high = (int)allParametersCount - 1;
	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (allParameters[mid].hash == hash) {
			return &allParameters[mid];
		} else if (allParameters[mid].hash < hash) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}
	return nullptr;
}

float getConfigValueByHash(const int hash) {
	const ConfigParameter* param = findParamByHash((uint32_t)hash);
	if (!param) return EFI_ERROR_CODE;
	const uint8_t* engineConfigurationPtr = (const uint8_t*)hackEngineConfigurationPointer(nullptr);
	void* addr = (void*)(engineConfigurationPtr + param->offset);
	switch (param->type) {
		case 0: return *(float*)addr;
		case 1: return (*(const uint32_t*)addr & (1U << param->bitOffset)) ? 1.0f : 0.0f;
		case 2: return (float)*(int*)addr;
		case 3: return (float)*(int8_t*)addr;
		case 4: return (float)*(uint8_t*)addr;
		case 5: return (float)*(int16_t*)addr;
		case 6: return (float)*(uint16_t*)addr;
		default: return EFI_ERROR_CODE;
	}
}

float getConfigValueByName(const char *name) {
	int hash = djb2lowerCase(name);
	return getConfigValueByHash(hash);
}

bool setConfigValueByName(const char *name, float value) {
	int hash = djb2lowerCase(name);
	const ConfigParameter* param = findParamByHash((uint32_t)hash);
	if (!param) return false;
	uint8_t* engineConfigurationPtr = (uint8_t*)hackEngineConfigurationPointer(nullptr);
	void* addr = (void*)(engineConfigurationPtr + param->offset);
	switch (param->type) {
		case 0: *(float*)addr = value; break;
		case 1:
			if (value != 0) {
				*(uint32_t*)addr |= (1U << param->bitOffset);
			} else {
				*(uint32_t*)addr &= ~(1U << param->bitOffset);
			}
			break;
		case 2: *(int*)addr = (int)value; break;
		case 3: *(int8_t*)addr = (int8_t)value; break;
		case 4: *(uint8_t*)addr = (uint8_t)value; break;
		case 5: *(int16_t*)addr = (int16_t)value; break;
		case 6: *(uint16_t*)addr = (uint16_t)value; break;
		default: return false;
	}
	return true;
}
