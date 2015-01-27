/**
 * @file	LocalVersionHolder.cpp
 *
 * @date Mar 19, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "LocalVersionHolder.h"

int getGlobalConfigurationVersion(void);

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
