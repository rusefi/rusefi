/**
 * @file	honda_accord.h
 *
 * @date Jan 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "engine_configuration.h"

void setHondaAccordConfigurationTwoWires();

/**
 * https://technet.ff-squad.com/wiring.obd1.htm
 *
 * as you move inward from the outside of the connector, tooth count decreases
 * outermost CKP = 12/24
 * middle = 4 total, 1/cyl
 * inner CID cylinder ID  = 1 total, 1/cam
 */
void setHondaAccordConfiguration1_24();
