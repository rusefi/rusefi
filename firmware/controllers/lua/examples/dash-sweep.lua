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

t = Timer.new()
t : reset()

function onTick()
  -- timer allows below 1hz rate
   if t : getElapsedSeconds() > 2 then
     t : reset()
   else
     return
   end

    onTickDashSweep()
end
