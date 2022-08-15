#define ARRAY_EQUALS "function equals(data1, data2) \
 \
  local index = 1 \
  while data1[index] ~= nil do \
	if data1[index] ~= data1[index] then \
       return -1 - index \
    end \
	index = index + 1 \
  end \
	if nil ~= data2[index] then \
       return -1 - index \
    end \
  return 0 \
end \
	"

// LSB (Least Significant Byte comes first) "Intel"
#define TWO_BYTES_LSB "function getTwoBytesLSB(data, offset, factor)        \
		return (data[offset + 2] * 256 + data[offset + 1]) * factor   \
	end"

// Little-endian System, "Intel"
#define SET_TWO_BYTES "	function setTwoBytes(data, offset, value) \
		value = math.floor(value) \
		data[offset + 2] = value >> 8 \
		data[offset + 1] = value & 0xff \
	end \
	"

// MOTOROLA order, MSB (Most Significant Byte/Big Endian) comes first.
#define TWO_BYTES_MSB "function getTwoBytesMSB(data, offset, factor)        \
		return (data[offset + 1] * 256 + data[offset + 2]) * factor   \
	end"

#define SET_TWO_BYTES_MSB "	function setTwoBytesMsb(data, offset, value) \
		value = math.floor(value) \
		data[offset + 1] = value >> 8 \
		data[offset + 2] = value & 0xff \
	end \
	"
