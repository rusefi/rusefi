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

function byteToHex(byte)
    local hi = math.floor(byte / 16)
    local lo = byte % 16
    return hexstr[hi + 1] .. hexstr[lo + 1]
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
