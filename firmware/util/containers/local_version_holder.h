/**
 * @file	local_version_holder.h
 *
 * @date Mar 19, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef LOCALVERSIONHOLDER_H_
#define LOCALVERSIONHOLDER_H_

#include "stdbool.h"

/**
 * this
 */
class LocalVersionHolder {
public:
	/**
	 * Calling this method changes the internal state of the class - each consumer interested in tracking
	 * changes should have it's own instance of this class
	 *
	 * @return true if 'globalVersion' has changed since last time we've invoked this method
	 */
	bool isOld(int globalVersion);
	int getVersion() const;

private:
	/**
	 * we want local version to be 'old' on instantiation
	 */
	int localVersion = -1;

};

#endif /* LOCALVERSIONHOLDER_H_ */
