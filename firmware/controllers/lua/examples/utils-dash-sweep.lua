dashSweepCounter = 1

timeout = 3000

rpmSensor = Sensor.new("rpm")
rpmSensor : setTimeout(timeout)

ppsSensor = Sensor.new("AcceleratorPedal")
ppsSensor : setTimeout(timeout)

tpsSensor = Sensor.new("tps1")
tpsSensor : setTimeout(timeout)

cltSensor = Sensor.new("clt")
cltSensor : setTimeout(timeout)

iatSensor = Sensor.new("iat")
iatSensor : setTimeout(timeout)

mapSensor = Sensor.new("map")
mapSensor : setTimeout(timeout)

speedSensor = Sensor.new("VehicleSpeed")
speedSensor : setTimeout(timeout)

gearSensor = Sensor.new("DetectedGear")
gearSensor : setTimeout(timeout)

function onTickDashSweep()

  speedSensor : set(    3 * dashSweepCounter)
  ppsSensor : set(    dashSweepCounter)
  tpsSensor : set(    dashSweepCounter)
  cltSensor : set(    dashSweepCounter)
  iatSensor : set( 100 -   dashSweepCounter)
  mapSensor : set( dashSweepCounter / 2)
  --print()

  gearSensor: set( dashSweepCounter / 20)
  rpmSensor:set(dashSweepCounter * 80)

  if dashSweepCounter > 99 then
      dashSweepCounter = 0
  end
  dashSweepCounter = dashSweepCounter + 1

end

