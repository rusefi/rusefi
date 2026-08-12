#include "pch.h"
#include "custom_engine.h"

// set engine_type 57
void setHondaCivicBcm() {
    setBodyControlUnit();
    engineConfiguration->totalGearsCount = 6;
    engineConfiguration->gearRatio[0] = 12;
    engineConfiguration->gearRatio[1] = 9;
    engineConfiguration->gearRatio[2] = 6.2;
    engineConfiguration->gearRatio[3] = 4.8;
    engineConfiguration->gearRatio[4] = 3.7;
    engineConfiguration->gearRatio[5] = 2.8;

    engineConfiguration->isBoostControlEnabled = true;

#ifdef HW_SMALL_CAN_BOARD
setLuaScript( R"(
-- this controls onCanRx rate as well!
setTickRate(300)

timeout = 3000

rpmSensor = Sensor.new("rpm")
rpmSensor : setTimeout(timeout)

ppsSensor = Sensor.new("AcceleratorPedal")
ppsSensor : setTimeout(timeout)

speedSensor = Sensor.new("VehicleSpeed")
speedSensor : setTimeout(timeout)

canTimer = Timer.new()
canTimer : reset()

hexstr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "A", "B", "C", "D", "E", "F" }

function getTwoBytesMSB(data, offset, factor)
	return (data[offset + 1] * 256 + data[offset + 2]) * factor
end

function getTwoBytesLSB(data, offset, factor)
	return (data[offset + 2] * 256 + data[offset + 1]) * factor
end

function onPOWERTRAIN_DATA(bus, id, dlc, data)
    canTimer : reset()
--    print('onPOWERTRAIN_DATA ' ..arrayToString(data))

    ppsValue = data[1] * 100.0 / 255
    ppsSensor : set(ppsValue)

    rpmValue = getTwoBytesMSB(data, 2, 1)
    rpmSensor : set(rpmValue)
--     print('onPOWERTRAIN_DATA ' .. rpmValue .. ' pedal ' .. ppsValue)
end

function onCAR_SPEED(bus, id, dlc, data)
    canTimer : reset()
    speedKph = getTwoBytesLSB(data, 1, 0.01)
    print('onCAR_SPEED ' .. speedKph)
    speedSensor : set(speedKph)

    print('onPOWERTRAIN_DATA speed' .. speedKph .. ' ratio ' .. (speedKph / rpmValue))

end

canRxAdd(1, 0x17C, onPOWERTRAIN_DATA)
canRxAdd(1, 0x309, onCAR_SPEED)

commTimer = Timer.new()
commTimer : reset()

canDelay = 1
commDelay = 20

function onTick()
    if getOutput("isUsbConnected") == 1 then
		commTimer : reset()
	end
	if (canTimer : getElapsedSeconds() > canDelay) and (commTimer : getElapsedSeconds() > commDelay) then
		mcu_standby()
	end
end
)");
#endif // HW_SMALL_CAN_BOARD
}
