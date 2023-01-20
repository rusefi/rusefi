-- this controls onCanRx rate as well!
setTickRate(300)

timeout = 3000

cltSensor = Sensor.new("clt")
cltSensor : setTimeout(timeout)

iatSensor = Sensor.new("iat")
iatSensor : setTimeout(timeout)

rpmSensor = Sensor.new("rpm")
rpmSensor : setTimeout(timeout)

mapSensor = Sensor.new("map")
mapSensor : setTimeout(timeout)

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


VWTP_OUT = 0x200
cuType = 0x01
-- todo: VWTP_IN should be 0x200 + cuType maybe?
VWTP_IN = 0x201
VWTP_TESTER = 0x300

local cuId = 0

function onCanHello(bus, id, dlc, data)
	-- here we handle 201 packets
	print('Got Hello Response ' ..arrayToString(data))
	cuId = data[6] * 256 + data[5]
	print('From ECU ' ..cuId)

    out = { 0xA0, 0x0F, 0x8A, 0xFF, 0x32, 0xFF }
	txCan(1, cuId, 0, out)
end

local sendCounter = 0
local packetCounter = 1
local payLoadIndex = 0

local groups = { 2, 4, 31 }
-- todo: smarter array size calculation?
local groupsSize = 3

local groupIndex = 1

function nextReq()
    local result = 0x10 + sendCounter
    sendCounter = sendCounter + 1
	if sendCounter == 16 then
		sendCounter = 0
	end
    return result
end

function onCanTester(bus, id, dlc, data)
	-- here we handle 300 packets

	-- 	print('Got from tester ' ..arrayToString(data))

	if data[1] == 0xA3 then
		-- 		print ("Keep-alive")
		txCan(1, cuId, 0, { 0xA1, 0x0F, 0x8A, 0xFF, 0x4A, 0xFF })

		groupIndex = groupIndex + 1
		if groupIndex > groupsSize then
			groupIndex = 1
		end
		groupId = groups[groupIndex]
		print ("KA " .. groupIndex .." " ..groupId)


		reqFirst = nextReq()
		print("Requesting next group " ..groupId .." with counter " ..reqFirst)
		txCan(1, cuId, 0, { reqFirst, 0x00, 0x02, 0x21, groupId })

		return
	end


	if data[1] == 0xA1 then
		print ("Happy 300 packet, follow-up...")
		txCan(1, cuId, 0, { nextReq(), 0x00, 0x02, 0x10, 0x89 })
		return
	end

	if data[1] == 0xA8 then
		print ("They said Bye-Bye")
		return
	end


	if data[1] == 0x10 and dlc == 5 then
		ackPacket = 0xB0 + packetCounter
		print ("Sending ACK B1 " ..ackPacket)
		txCan(1, cuId, 0, { ackPacket })
		-- request first group from array
		txCan(1, cuId, 0, { nextReq(), 0x00, 0x02, 0x21, groups[1] })
		return
	end

	top4 = math.floor(data[1] / 16)

	if top4 == 0xB then
		-- 		print("Got ACK")
		return
	end

	if top4 == 2 or top4 == 1 then
		print ("Looks like payload index " ..payLoadIndex ..": " ..arrayToString(data))

		if groupId == 2 and payLoadIndex == 0 then
			rpmL = data[7]
			rpmH = data[8]
			rpm = rpmH * rpmL / 5
			print(rpmL .." " ..rpmH .." rpm " ..rpm)
			rpmSensor : set(rpm)
		end

		if groupId == 2 and payLoadIndex == 1 then
			elL = data[3]
			elH = data[4]
			el = 100 * elH / elL
			print(elL .." " ..elH .." EL " ..el)
		end

		if groupId == 2 and payLoadIndex == 2 then
			mapL = data[2]
			mapH = data[3]
			map = 1000.0 * mapH / mapL
			print(mapL .." " ..mapH .." MAP " ..map)
			mapSensor : set(map / 10)
		end

		if groupId == 4 and payLoadIndex == 1 then
			cltL = data[6]
			cltH = data[7]
			clt = cltL * (cltH - 100) / 10
			print(cltL .." " ..cltH .." CLT " ..clt)
			cltSensor : set(clt)
		end

		if groupId == 4 and payLoadIndex == 2 then
			iatL = data[2]
			iatH = data[3]
			iat = iatL * (iatH - 100) / 10
			print(iatL .." " ..iatH .." IAT " ..iat)
			iatSensor : set(iat)
		end

		payLoadIndex = payLoadIndex + 1


		packetCounter = packetCounter + 1
		if packetCounter > 15 then
			packetCounter = 0
		end

		if top4 == 1 then
            -- todo: should we ACK top4 == 2?
			ackPacket = 0xB0 + packetCounter
			print ("Sending payload ACK " ..ackPacket)
			txCan(1, cuId, 0, { ackPacket })
			payLoadIndex = 0
		end

		return
	end

	print('Got unexpected ' ..arrayToString(data))
end

canRxAdd(VWTP_IN, onCanHello)
canRxAdd(VWTP_TESTER, onCanTester)

txCan(1, VWTP_OUT, 0, { cuType, 0xC0, 0x00, 0x10, 0x00, 0x03, 0x01 })

function onTick()
end

