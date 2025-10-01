dashSweepCounter = 1

timeout = 3000

rpmSensor = Sensor.new("rpm")
rpmSensor : setTimeout(timeout)

ppsSensor = Sensor.new("AcceleratorPedalUnfiltered")
ppsSensor : setTimeout(timeout)

tpsSensor = Sensor.new("tps1")
tpsSensor : setTimeout(timeout)

cltSensor = Sensor.new("clt")
cltSensor : setTimeout(timeout)

FuelLevel = Sensor.new("FuelLevel")
FuelLevel : setTimeout(timeout)

iatSensor = Sensor.new("iat")
iatSensor : setTimeout(timeout)

mapSensor = Sensor.new("map")
mapSensor : setTimeout(timeout)

speedSensor = Sensor.new("VehicleSpeed")
speedSensor : setTimeout(timeout)

gearSensor = Sensor.new("DetectedGear")
gearSensor : setTimeout(timeout)

oilP = Sensor.new("OilPressure")
oilP : setTimeout(timeout)

function onTickDashSweep()

-- uncomment line below if want static state
-- dashSweepCounter = 20

  speedSensor : set(    3 * dashSweepCounter)
  ppsSensor : set( 10 +   dashSweepCounter)
  tpsSensor : set( 5 +   dashSweepCounter)
  cltSensor : set(    dashSweepCounter)
  iatSensor : set( 100 -   dashSweepCounter)
  mapSensor : set( dashSweepCounter / 2)
  oilP      : set( dashSweepCounter / 3)
  FuelLevel : set( dashSweepCounter)
  --print()

  gearSensor: set( dashSweepCounter / 20)
  rpmSensor:set(dashSweepCounter * 80)

  if dashSweepCounter > 79 then
      dashSweepCounter = 0
  end
  dashSweepCounter = dashSweepCounter + 1

end

