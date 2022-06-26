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


#define SET_TWO_BYTES "	function setTwoBytes(data, offset, value) \
		value = math.floor(value) \
		data[offset + 2] = value >> 8 \
		data[offset + 1] = value & 0xff \
	end \
	"
