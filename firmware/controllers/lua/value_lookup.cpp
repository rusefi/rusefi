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
	if (!param || !param->getter) return EFI_ERROR_CODE;
	return param->getter();
}

float getConfigValueByName(const char *name) {
	int hash = djb2lowerCase(name);
	return getConfigValueByHash(hash);
}

bool setConfigValueByName(const char *name, float value) {
	int hash = djb2lowerCase(name);
	const ConfigParameter* param = findParamByHash((uint32_t)hash);
	if (!param || !param->setter) return false;
	param->setter(value);
	return true;
}
