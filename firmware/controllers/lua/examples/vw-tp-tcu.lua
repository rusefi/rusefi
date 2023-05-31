-- J2819 TP2.0 vehicle diagnostics protocol
-- very limited implementation
-- this controls onCanRx rate as well!
tickRate = 100

setTickRate(tickRate)

timeout = 3000

cuType = 0x02 -- TCU

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

VWTP_IN = 0x200 + cuType
VWTP_TESTER = 0x300

local cuId = 0

function onCanHello(bus, id, dlc, data)
	-- here we handle 201 packets
	print('Got Hello Response ' ..arrayToString(data))
	cuId = data[6] * 256 + data[5]

	print('From *CU ' ..cuId)
    out = { 0xA0, 0x0F, 0x8A, 0xFF, 0x32, 0xFF }
	print("Saying hello " .. arrayToString(out))
	txCan(1, cuId, 0, out)
end

local sendCounter = 0
local packetCounter = 1
local payLoadIndex = 0

local groupIndex = 1

withErrorCodes = 0

byeByeStateCounter = 2

function nextReq()
    local result = 0x10 + sendCounter
    sendCounter = sendCounter + 1
	if sendCounter == 16 then
		sendCounter = 0
	end
    return result
end

function startTp()
    sendCounter = 0
    packetCounter = 1
    payLoadIndex = 0
    print("Starting TP with TCU")

    txCan(1, VWTP_OUT, 0, { cuType, 0xC0, 0x00, 0x10, 0x00, 0x03, 0x01 })
end

function requestErrorCodes()
    out = { nextReq(), 0x00, 0x04, 0x18, 0x02, 0xFF, 0x00 }
    txCan(1, cuId, 0, out)
    print("Requesting error codes " .. arrayToString(out))
end

function requestEraseCodes()
    out = { nextReq(), 00, 0x03, 0x14, 0xFF, 0x00 }
    txCan(1, cuId, 0, out)
    print("Requesting to Erase Code(s)")
end

function scheduleTpRestart()
		byeByeStateCounter = 3 * tickRate
end

keepAliveCounter = 0

function onCanTester(bus, id, dlc, data)
	-- here we handle 300 packets
    scheduleTpRestart()
	print('Got from tester ' ..arrayToString(data))

	if data[1] == 0xA3 then
		print ("A3 Keep-alive")
		txCan(1, cuId, 0, { 0xA1, 0x0F, 0x8A, 0xFF, 0x4A, 0xFF })

		print ("Keep-alive withErrorCodes=" .. withErrorCodes)

        if withErrorCodes > 0 then
            requestEraseCodes()
        else
            keepAliveCounter = keepAliveCounter + 1
            if keepAliveCounter > 4 then
                requestErrorCodes()

                keepAliveCounter = 0
            end

        end

		return
	end


	if data[1] == 0xA1 then
		print ("Happy 300 packet " .. arrayToString(data))
		txCan(1, cuId, 0, { nextReq(), 0x00, 0x02, 0x10, 0x89 })
		return
	end

	if data[1] == 0xA8 then
		print ("They said Bye-Bye")
		return
	end

	if data[1] == 0x10 and dlc == 5 then
		ackPacket = 0xB0 + packetCounter
		print ("Sending ACK Bx " ..ackPacket)
		txCan(1, cuId, 0, { ackPacket })

        if data[3] == 2 and data[4] == 0x50 then
        	print('Got Hello2 Response ' ..arrayToString(data))

            requestErrorCodes()
        end
	end

	top4 = math.floor(data[1] / 16)

	if top4 == 0xB then
		print("Got 0xBx ACK")
		return
	end

	if top4 == 2 or top4 == 1 then
		print ("Looks like payload index " ..payLoadIndex ..": " ..arrayToString(data))

		if payLoadIndex == 0 and data[4] == 0x58 then
		    len = data[3]
		    print("Looks like CODES_REQ response of length " .. len)

		    if len ~= 2 then
		        print("HAVE CODES " .. len)
		        withErrorCodes = 1
            else
                withErrorCodes = 0
		    end

		end

		payLoadIndex = payLoadIndex + 1

		packetCounter = packetCounter + 1
		if packetCounter > 15 then
			packetCounter = 0
		end

		ackPacket = 0xB0 + packetCounter
		print ("Sending payload ACK " ..ackPacket)
    	txCan(1, cuId, 0, { ackPacket })

		if top4 == 1 then
			payLoadIndex = 0

            if data[2] == 0 and data[3] == 2 and data[4] == 0x58 then
                print("NO CODES")
            end

		end

		return
	end

	print('Got unexpected ' ..arrayToString(data))
end

canRxAdd(VWTP_IN, onCanHello)
canRxAdd(VWTP_TESTER, onCanTester)

startTp()

function onTick()

    if byeByeStateCounter > 0 then
        byeByeStateCounter = byeByeStateCounter - 1
    end

    if byeByeStateCounter == 1 then
        scheduleTpRestart()
        startTp()
    end

end

