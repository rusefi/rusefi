
-- https://sourceforge.net/p/ecu/wiki/canbus/
-- todo: CAN ID 430h, period 50ms, produced by SJB. Byte 7: Bit2 A/C request (button pressed).

canRxAdd(0x201)
setTickRate(20)

ppsSensor = Sensor.new("AcceleratorPedal")

function getTwoBytesMSB(data, offset, factor)
	return (data[offset + 1] * 256 + data[offset + 2]) * factor
end

function onCanRx(bus, id, dlc, data)
	packet = getTwoBytesMSB(data, 6, 1)
	if packet < 0x7f or packet > 0xC8FF then
		ppsSensor : invalidate()
	else
		ppsSensor : set(packet / 512)
	end
end

function onTick()
	ppsValue = getSensor("AcceleratorPedal")
	if ppsValue == nil then
		print ("PPS not defined")
	else
		print ("PPS " ..ppsValue)
	end
end
