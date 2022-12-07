-- "alive" message from pump
canRxAdd(0x1B200002)

local pumpAlive = Timer.new()

function onCanRx(bus, id, dlc, data)
	if id == 0x1B200002 then
		pumpAlive:reset()
	end
end

local data_0x1ae0092c = { 0x00, 0x00, 0x22, 0xe0, 0x41, 0x90, 0x00, 0x00 }
local data_0x02104136 = { 0xbb, 0x00, 0x3f, 0xff, 0x06, 0xe0, 0x00, 0x00 }

local slowCounter = 0
local slowRoll = 0

local slowRollTable = { 0x00, 0x40, 0x80, 0xC0 }

-- car stopped is 0
-- car "moving fast" is 6000 (whatever that means?)
local speedVal = 1000

function onTick()
	if pumpAlive:getElapsedSeconds() < 1 then
		-- Ignition on frame: sends every 30th speed frame
		if slowCounter == 0 then
			-- cycle through 0, 40, 80, c0
			slowRoll = (slowRoll + 1) & 3
			data_0x1ae0092c[1] = slowRollTable[slowRoll + 1]

			txCan(1, 0x1ae0092c, 1, data_0x1ae0092c)

			slowCounter = 30
		end

		slowCounter = slowCounter - 1

		data_0x02104136[7] = (speedVal >> 8) & 0xFF
		data_0x02104136[8] = speedVal & 0xFF

		-- pump speed frame
		txCan(1, 0x02104136, 1, data_0x02104136)
	end
end

setTickRate(71)