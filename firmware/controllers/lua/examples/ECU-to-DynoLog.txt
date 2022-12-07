
DYNOLOG_BASE = 0x680

setTickRate(5) -- set tick rate to 5hz

protocolPacket = { 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
protocolBase1 = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

function setTwoBytesMsb(data, offset, value)
	value = math.floor(value)
	data[offset + 1] = value >> 8
	data[offset + 2] = value & 0xff
end

function onTick()
    local RPM = getSensor("RPM")
    local MAP = getSensor("MAP")
    local TPS = getSensor("TPS1")
    local IAT = getSensor("IAT")

    setTwoBytesMsb(protocolBase1, 0, RPM)
    setTwoBytesMsb(protocolBase1, 2, MAP * 10)
    setTwoBytesMsb(protocolBase1, 4, TPS * 10)
    setTwoBytesMsb(protocolBase1, 6, IAT * 100)

    txCan(1, DYNOLOG_BASE + 0, 0, protocolPacket)
    txCan(1, DYNOLOG_BASE + 1, 0, protocolBase1)
end
