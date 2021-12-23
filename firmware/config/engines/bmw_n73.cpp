/*
 * bmw_n73.cpp
 *
 * @date Oct 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "bmw_n73.h"

void setEngineProteusBMW_N73_GDI() {

}

void setEngineProteusGearboxManInTheMiddle() {
	strncpy(config->luaScript, R"(

CAN_BMW_E90_RPM_THROTTLE = 0x0AA
CAN_BMW_E90_TORQUE_DEMAND   =  0x0B6
CAN_BMW_E90_IGNITION_KEY = 0x130
CAN_BMW_E65_GEAR_SELECTOR = 0x192
CAN_BMW_E90_COOLANT = 0x1D0
CAN_BMW_E90_DASH_ON = 0x332

ECU_BUS = 1
GEAR_BUS = 2

canRxAdd(CAN_BMW_E90_RPM_THROTTLE)
canRxAdd(CAN_BMW_E90_TORQUE_DEMAND)
canRxAdd(CAN_BMW_E90_IGNITION_KEY)
canRxAdd(CAN_BMW_E65_GEAR_SELECTOR)
canRxAdd(CAN_BMW_E90_COOLANT)
canRxAdd(CAN_BMW_E90_DASH_ON)


txPayload = {}
function onTick()
end
)", efi::size(config->luaScript));


}
