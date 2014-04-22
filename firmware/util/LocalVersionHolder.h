/**
 * @file	LocalVersionHolder.h
 *
 * @date Mar 19, 2014
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#ifndef LOCALVERSIONHOLDER_H_
#define LOCALVERSIONHOLDER_H_

#include "stdbool.h"

class LocalVersionHolder {
public:
	LocalVersionHolder();

	bool isOld();

private:
	bool localVersion;

};

#endif /* LOCALVERSIONHOLDER_H_ */
