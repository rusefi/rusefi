-- sometimes we want to cut a CAN bus and install rusEFI into that cut
-- https://en.wikipedia.org/wiki/Man-in-the-middle_attack

-- this controls onCanRx rate as well!
setTickRate(300)

ECU_BUS = 1
-- really 'not ECU'
TCU_BUS = 2

hexstr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "A", "B", "C", "D", "E", "F" }

function toHexString(num)
	if num == 0 then
		return '0'
	end

	local result = ""
	while num > 0 do
		local n = num % 16
		result = hexstr[n + 1] ..result
		num = math.floor(num / 16)
	end
	return result
end

function arrayToString(arr)
	local str = ""
	local index = 1
	while arr[index] ~= nil do
		str = str.." "..toHexString(arr[index])
		index = index + 1
	end
	return str
end

totalEcuMessages = 0
totalTcuMessages = 0

function printAndDrop(bus, id, dlc, data)
    print('Dropping ' ..arrayToString(data))
end

function onAnythingFromECU(bus, id, dlc, data)
    totalEcuMessages = totalEcuMessages + 1
    txCan(TCU_BUS, id, 0, data) -- relay non-TCU message to TCU
end

function onAnythingFromTCU(bus, id, dlc, data)
    totalTcuMessages = totalTcuMessages + 1
    txCan(ECU_BUS, id, 0, data) -- relay non-ECU message to ECU
end

-- VAG Motor_1 just as example
-- canRxAdd(ECU_BUS, 0x280, printAndDrop)

-- last option: unconditional forward of all remaining messages
canRxAddMask(ECU_BUS, 0, 0, onAnythingFromECU)
canRxAddMask(TCU_BUS, 0, 0, onAnythingFromTCU)

everySecondTimer = Timer.new()

function onTick()
    if everySecondTimer:getElapsedSeconds() > 1 then
        everySecondTimer:reset()
        print("Total from ECU " .. totalEcuMessages .. " from TCU " .. totalTcuMessages)
    end
end
