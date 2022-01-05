/*
 * @file hyundai.cpp
 *
 * @date Oct 11, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "hyundai.h"
#include "map.h"

static void commonGenesisCoupe() {
	strncpy(config->luaScript, R"(

-- listen to CAN ID 0x4f0
canRxAdd(0x4f0)

-- todo: add payload logic
packet546 = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 }

function onCanRx(bus, id, dlc, data)
	id11 = id % 2048
	print('got CAN id=' .. id .. ' dlc='  .. dlc)
	-- todo: add filtering logic if needed

	rpm = getSensor("RPM")
	-- handle nil RPM, todo: change firmware to avoid nil RPM
	rpm = (rpm == nil and 0 or rpm)

	if rpm > 800 then
		-- at the moment we simply
		txCan(1, 0x546, 0, packet546)
	end


end

function onTick()
end


)", efi::size(config->luaScript));
}

void setGenesisCoupeBK1() {
	commonGenesisCoupe();
	// 2010-2012 315cc at 43.5psi
	// fuel system is fixed pressure 55psi
	engineConfiguration->injector.flow = 354.19; // https://www.google.com/search?q=315*sqrt%2855%2F43.5%29
	engineConfiguration->fuelReferencePressure = PSI2KPA(55);

}

void setGenesisCoupeBK2() {
	commonGenesisCoupe();
	// 2013+ 450cc at 43.5
	// fuel system is fixed pressure 85psi
	// flow rate P2 = flow rate P1 * sqrt(P2/P1)
	engineConfiguration->injector.flow = 629.03; // https://www.google.com/search?q=450*sqrt%2885%2F43.5%29
	engineConfiguration->fuelReferencePressure = PSI2KPA(85);

}
