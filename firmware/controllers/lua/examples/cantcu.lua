-- CanTCU integration example
--
-- A CanTCU transmission controller broadcasts shift-assist requests in CAN
-- frame 0x301. This script translates those requests into engine actions:
--  - a torque cut during shifts, implemented as ignition timing retard
--  - a throttle blip for rev-matching on downshifts, implemented via
--    electronic throttle position add
--
-- Note: in rusEFI Lua the CAN payload 'data' array is 1-based, so data[1]
-- is the first byte of the frame.

-- raw throttle blip request from the TCU (byte 8 of frame 0x301)
blipRequest = 0
-- throttle position adjustment sent to the electronic throttle, percent
etbBlipAdd = 0
-- raw torque cut request from the TCU (byte 7 of frame 0x301)
cutRequest = 0
-- cut request mapped through curve 1 into degrees of timing change
cutRequestMapped = 0
-- ignition timing adjustment actually applied, degrees (negative = retard)
timingRetardAdd = 0

canRxAdd(0x301)

setTickRate(100)

function onCanRx(bus, id, dlc, data)

	if id == 0x301 then
		-- intervention state reported by the TCU, currently unused
		interventionState = data[6]

		-- torque cut: map the request through curve 1 (configured in the tune)
		-- and apply it as negative timing offset, i.e. ignition retard
		cutRequest = data[7]
		cutRequestMapped = curve(1, cutRequest)
		timingRetardAdd = -cutRequestMapped
		setTimingAdd(timingRetardAdd)

		-- throttle blip: scale the request into a throttle position add
		blipRequest = data[8]
		etbBlipAdd = 0.3 * blipRequest
		setEtbAdd(etbBlipAdd)
	end

end

function onTick()

	print('timing retard: ' ..timingRetardAdd)
	print('blip request: ' ..blipRequest)
	print('etb blip add: ' ..etbBlipAdd)
end
