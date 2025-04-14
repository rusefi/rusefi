-- scriptname Dyno-Dynamics.lua

-- display data from dynamometer
-- details https://github.com/rusefi/rusefi/issues/7714

-- include utils-can-dbc.lua
-- endinclude

EngineSpeed = 0
VehicleSpeed = 0
Power = 0
TorqueATW = 0


function onCanData100(bus, id, dlc, data)
    EngineSpeed = getBitRangeMoto(data, 0, 16)
    VehicleSpeed = getBitRangeMoto(data, 16, 16)
    Power = getBitRangeMoto(data, 32, 16)
    TorqueATW = getBitRangeMoto(data, 48, 16)
end
canRxAdd(1, 0x100, onCanData100)


everySecondTimer = Timer.new()
function onTick()
    if everySecondTimer:getElapsedSeconds() > 1 then
        everySecondTimer:reset()

        print('EngineSpeed='..EngineSpeed..' VehicleSpeed='..VehicleSpeed..' Power='..Power..' TorqueATW='..TorqueATW)

    end
end
