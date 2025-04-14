-- Set of utilites to get and set CAN data according to DBC rules

-- little-endian byte order
function getBitRangeLsb(data, bitIndex, bitWidth)
	local byteIndex = bitIndex >> 3
	local shift = bitIndex - byteIndex * 8
	value = data[1 + byteIndex]
	if (shift + bitWidth > 8) then
		value = value + data[2 + byteIndex] * 256
	end
	mask = (1 << bitWidth) - 1
	return (value >> shift) & mask
end

-- little-endian byte order
function setBitRangeLsb(data, totalBitIndex, bitWidth, value)
    local byteIndex = totalBitIndex >> 3
    local bitInByteIndex = totalBitIndex - byteIndex * 8
    if (bitInByteIndex + bitWidth > 8) then
        local bitsToHandleNow = 8 - bitInByteIndex
        setBitRangeLsb(data, totalBitIndex + bitsToHandleNow, bitWidth - bitsToHandleNow, value >> bitsToHandleNow)
        bitWidth = bitsToHandleNow
    end
    local mask = (1 << bitWidth) - 1
    data[1 + byteIndex] = data[1 + byteIndex] & (~(mask << bitInByteIndex))
    local maskedValue = value & mask
    local shiftedValue = maskedValue << bitInByteIndex
    data[1 + byteIndex] = data[1 + byteIndex] | shiftedValue
end

-- big endian byte order, but bitIndex differs from used in DBC
function getBitRangeMsb(data, bitIndex, bitWidth) 
	local byteIndex = bitIndex >> 3 
	local shift = bitIndex - byteIndex * 8 
	local value = data[1 + byteIndex] 
	if (shift + bitWidth > 8) then 
		value = value + data[0 + byteIndex] * 256
	end 
	local mask = (1 << bitWidth) - 1 
	return (value >> shift) & mask 
end 

-- big endian byte order, but bitIndex differs from used in DBC
function setBitRangeMsb(data, totalBitIndex, bitWidth, value)
	local byteIndex = totalBitIndex >> 3
	local bitInByteIndex = totalBitIndex - byteIndex * 8
	if (bitInByteIndex + bitWidth > 8) then
		local bitsToHandleNow = 8 - bitInByteIndex
		setBitRangeMsb(data, (byteIndex - 1) * 8, bitWidth - bitsToHandleNow, value >> bitsToHandleNow)
		bitWidth = bitsToHandleNow
	end
	local mask = (1 << bitWidth) - 1
	data[1 + byteIndex] = data[1 + byteIndex] & (~(mask << bitInByteIndex))
	local maskedValue = value & mask
	local shiftedValue = maskedValue << bitInByteIndex
	data[1 + byteIndex] = data[1 + byteIndex] | shiftedValue
end

local function motoMagic(b, length)
    b = b - (b % 8) + 7 - (b % 8)
    b = b + length - 1
    b = b - (b % 8) + 7 - (b % 8)
    return b
end

-- big endian byte order
function setBitRangeMoto(data, totalBitIndex, bitWidth, value)
    value = math.floor(value)
    local moto = motoMagic(totalBitIndex, bitWidth)
    setBitRangeMsb(data, moto, bitWidth, value)
end

-- big endian byte order
function getBitRangeMoto(data, bitIndex, bitWidth)
    local moto = motoMagic(bitIndex, bitWidth)
    return getBitRangeMsb(data, moto, bitWidth)
end
