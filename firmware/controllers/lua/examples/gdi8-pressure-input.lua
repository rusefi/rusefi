-- scriptname gdi8-pressure-input.lua

sensor = Sensor.new("FuelPressureHigh")
sensor : setTimeout(1000)

-- include utils-print-array.lua
-- endinclude

function getTwoBytesLSB(data, offset, factor)
	return (data[offset + 2] * 256 + data[offset + 1]) * factor
end

function onSensorData(bus, id, dlc, data)
  if data[8] ~= 0xDE then
    print ("invalid data: " ..arrayToString(data))
    return
  end
--  print ("valid data: " ..arrayToString(data))
  local pressureBar = getTwoBytesLSB(data, 0, 10)
  sensor : set(pressureBar)
  print ("valid pressureBar: " ..pressureBar)
end

-- bus index '1' on most rusEFI boards, '1' or '2' on Proteus
canRxAdd(2, 0xBB26, onSensorData)

function onTick()
end
