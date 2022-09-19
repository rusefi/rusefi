-- just first few lines of XCP protocol

-- this controls onCanRx rate as well!
setTickRate(100)

XCP_ID = 0x7EF

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

STD_CONNECT = 0x01

canRxAdd(XCP_ID)

function onCanRx(bus, id, dlc, data)
	print('got CAN id=' ..id ..' dlc=' ..dlc .. arrayToString(data))

   if data[1] == STD_CONNECT then
     masterCounter = data[2]
     print ('STD_CONNECT ' .. masterCounter)

     resp = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
     resp[1] = 0xFF
     resp[3] = masterCounter
     txCan(1, 0x7EE, 0, resp)
   end

end

function onTick()
end
