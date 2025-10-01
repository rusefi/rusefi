-- scriptname dash-sweep.lua

--
-- Lua script useful for dash integration development
--

-- include utils-dash-sweep.lua
-- endinclude

function onDashAlive(bus, id, dlc, data)
	print('CAN RX: dash says it is connected!')
end

canRxAdd(0x77000F, onDashAlive)

function onTick()
    onTickDashSweep()
end
