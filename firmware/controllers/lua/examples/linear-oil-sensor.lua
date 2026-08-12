// Lua for copying AuxLinear1 into OilTempSensor
//
// Set the new sensor type at the top
//

OilTempSensor = Sensor.new("OilTemperature")
OilTempSensor:setTimeout(60000)

function onTick()
    OilTempValue = getSensor("AuxLinear1")  //-- Can multiply by scaling factor
    OilTempSensor:set(OilTempValue)
end
