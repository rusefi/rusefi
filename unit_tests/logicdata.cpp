/*
 * @file logicdata.cpp
 *
 *  Created on: Jul 19, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "logicdata.h"
#include <stdio.h>
#include <stdlib.h>

void writeFile() {

	FILE *ptr = fopen("test.logicdata", "wb");


	fclose(ptr);

}
