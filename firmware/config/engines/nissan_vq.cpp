/*
 * @file nissan_vq.cpp
 *
 *  Created on: Jul 2, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#include "nissan_vq.h"

void setHellen121nissanQR() {
	engineConfiguration->trigger.type = TT_NISSAN_QR25;

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->specs.displacement = 2;
	strcpy(engineConfiguration->engineCode, "QR");

	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = Gpio::Unassigned;

	engineConfiguration->vvtMode[0] = VVT_NISSAN_VQ;

	// we have this here and not in board_configuration.cpp so that unit test would get this value
	engineConfiguration->invertCamVVTSignal = true;

	engineConfiguration->vvtOffsets[0] = NISSAN_VQ_VVT_OFFSET;

}

void setHellen121nissanVQ() {
	// by definition this should be a copy-paste of https://github.com/rusefi/rusefi/blob/master/firmware/controllers/lua/examples/350z-ac.txt
	strncpy(config->luaScript, R"(
canRxAdd(0x35d)

setTickRate(100)
t = Timer.new()
t : reset()

globalAcOut = 0

function onTick()
	local RPMread = math.floor(getSensor("RPM") + 0.5) / 3.15
	local RPMhi = RPMread / 256
	local RPMlo = RPMread
	local CLTread = math.floor(getSensor("CLT") + 0.5)

	--print('ac out = ' ..globalAcOut)
	if globalAcOut == 1 and RPMread >80 then
		fanPayloadOff = { 0x88, 0x00 }
		fanPayloadLo = { 0x88, 0x00 }
		fanPayloadHi = { 0x88, 0x00 }
	else
		--print('ac off payload')
		fanPayloadOff = { 0x00, 0x00 }
		fanPayloadLo = { 0x40, 0x00 }
		fanPayloadHi = { 0x80, 0x00 }
	end

	cltGauge = 0x00
	-- acOut = {0x32, 0x80, 0x00, 0x10, 0x00, 0x00}
	-- txCan(1, 0x625, 0,acOut)

	-- clt gauge stuff
	if CLTread < 115 then
		-- txCan(1, 0x608, 0, canCLTpayloadNo)
		cltGauge = math.floor(CLTread * 1.5 + 0.5)
	elseif CLTread >= 115 then
		-- txCan(1, 0x608, 0, canCLTpayloadHi)
		cltGauge = 0xF0
	end
	-- print('clt gauge = '..cltGauge)
	-- rpm fun stuff
	if t : getElapsedSeconds() < 2 then
		CLTandRPM = { 0x00, 0x18, 0x0C, 0x01, 0x0A, 0x87, 0xFF, 0xFF }
	else
		CLTandRPM = { 0x00, 0x18, 0x0c, RPMlo, RPMhi, 0x87, 0xFF, cltGauge }
	end

	txCan(1, 0x23D, 0, CLTandRPM) -- transmit CLT and RPM



	if RPMread > 80 then
		if CLTread <= 80 then
			txCan(1, 0x1F9, 0, fanPayloadOff)
		elseif CLTread >= 85 and CLTread < 90 then
			txCan(1, 0x1F9, 0, fanPayloadLo)
		elseif CLTread >= 90 then
			txCan(1, 0x1F9, 0, fanPayloadHi)
		end
	else
		txCan(1, 0x1F9, 0, fanPayloadOff)
	end
	-- print('CLT temp' ..CLTread)
end


function onCanRx(bus, id, dlc, data)
	--print('got CAN id=' ..id ..' dlc=' ..dlc)
	--print('ac value is= '..data[1])
	if data[1] == 193 then
		setAcRequestState(true)
		globalAcOut = 1
		--print('ac is on')
	else
		setAcRequestState(false)
		globalAcOut = 0
		--print('ac is off')
	end

end
)", efi::size(config->luaScript));


	engineConfiguration->trigger.type = TT_NISSAN_VQ35;

	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.firingOrder = FO_1_2_3_4_5_6;
	engineConfiguration->specs.displacement = 4;
	strcpy(engineConfiguration->engineCode, "VQ");

	engineConfiguration->vvtMode[0] = VVT_NISSAN_VQ;

	// we have this here and not in board_configuration.cpp so that unit test would get this value
	engineConfiguration->invertCamVVTSignal = true;

	engineConfiguration->vvtOffsets[0] = NISSAN_VQ_VVT_OFFSET;
	engineConfiguration->vvtOffsets[1 * CAMS_PER_BANK] = NISSAN_VQ_VVT_OFFSET - NISSAN_VQ_CAM_OFFSET;

	engineConfiguration->cranking.baseFuel = 35;
}

void setHellen121nissanAltimaN16() {
	// https://en.wikipedia.org/wiki/Nissan_QG_engine
	// https://en.wikipedia.org/wiki/N-VCT
	setHellen121nissanQR();
	engineConfiguration->specs.displacement = 1.8;
	strcpy(engineConfiguration->engineCode, "N16");

}
