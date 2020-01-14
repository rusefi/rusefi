/**
 * @file	local_version_holder.cpp
 *
 * @date Mar 19, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "local_version_holder.h"

int LocalVersionHolder::getVersion() const {
	return localVersion;
}

bool LocalVersionHolder::isOld(int globalVersion) {
	if (globalVersion > localVersion) {
		localVersion = globalVersion;
		return true;
	}
	return false;
}
