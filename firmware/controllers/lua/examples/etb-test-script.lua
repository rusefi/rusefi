setTickRate(20)

ppsSensor = Sensor.new("AcceleratorPedal")

function onTick()
    ppsSensor : set(25)
end
