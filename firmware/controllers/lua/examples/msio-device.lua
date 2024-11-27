-- this script turns a rusEFI into an MSIO box

setTickRate(300)
every50msTimer = Timer.new()

baseId = 0x240
isExtId = 0

-- MOTOROLA order, MSB (Most Significant Byte/Big Endian) comes first
function setTwoBytesMsb(data, offset, value)
	value = math.floor(value)
	data[offset + 1] = value >> 8
	data[offset + 2] = value & 0xff
end

local config_packet = { 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
local tach_packet = { 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

setTwoBytesMsb(config_packet, 4, 5000)
setTwoBytesMsb(config_packet, 6, 5000)

function onPing(bus, id, dlc, data)
	print('Got ping')
  txCan(1, baseId + 8, isExtId, config_packet)
end

currentState1 = 0
currentState2 = 0

local frequency = 100
startPwm(0, frequency, 0)
startPwm(1, frequency, 0)

function onIoState(bus, id, dlc, data)
  local state = data[5]

  local newState1 = ((state & 1) ~= 0) and 1 or 0
  if currentState1 ~= newState1 then
    currentState1 = newState1
    print("newState1 " .. newState1)
    setPwmDuty(0, currentState1)
  end

  local newState2 = ((state & 2) ~= 0) and 1 or 0
  if currentState2 ~= newState2 then
    currentState2 = newState2
    print("newState2 " .. newState2)
    setPwmDuty(1, currentState1)
  end
end

canRxAdd(1, baseId, onPing)
canRxAdd(1, baseId + 5, onIoState)

function onTick()

    if every50msTimer:getElapsedSeconds() > 0.05 then
        every50msTimer:reset()
        -- random tach packet just to show that we are alive
        txCan(1, baseId + 0xB, isExtId, tach_packet)
    end
end
