/**
 * @file	LocalVersionHolder.cpp
 *
 * @date Mar 19, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "LocalVersionHolder.h"

extern "C" {
int getGlobalConfigurationVersion(void);
}

//ctor
LocalVersionHolder::LocalVersionHolder() {
	localVersion = 0;
}

int LocalVersionHolder::getVersion() {
	return localVersion;
}

bool LocalVersionHolder::isOld() {
	int global = getGlobalConfigurationVersion();
	if (global > localVersion) {
		localVersion = global;
		return true;
	}
	return false;
}
