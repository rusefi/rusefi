-- https://rusefi.com/forum/viewtopic.php?f=4&t=2325
-- tested on 9286699

-- CAN Rx Channels
IDRIVE_BUTTONS = 0x267
IDRIVE_ROTARY = 0x264

-- CAN Tx Channels
IDRIVE_BRIGHTNESS = 0x202
IDRIVE_HEARBEAT = 0x560
IDRIVE_ROTARY_INIT = 0x273

brighness = 0

canRxAdd(IDRIVE_ROTARY) -- rotation
canRxAdd(IDRIVE_BUTTONS) -- buttons

setTickRate(5) -- set tick rate to 5hz

-- One time rotary initialisation

txCan(1, IDRIVE_ROTARY_INIT, 0, { 0x1D, 0xE1, 0x00, 0xF0, 0xFF, 0x7F, 0xDE, 0x04 })

function onCanRx(bus, id, dlc, data)
	-- 	print('got CAN id=' ..id ..' dlc=' ..dlc)

	if id == IDRIVE_BUTTONS then
		-- 		print('got buttons @4=' ..data[4] ..' @6=' ..data[6])

		if data[4] == 1 or data[4] == 2 then
			print('got hold or release @4=' ..data[4] ..' @6=' ..data[6])

			if data[6] == 2 and brighness > 0 then
				brighness = brighness - 1
				print('brighness ' ..brighness)
			end

			if data[6] == 4 and brighness < 15 then
				brighness = brighness + 1
				print('brighness ' ..brighness)
			end
		end
	end

	if id == IDRIVE_ROTARY then
		if data[5] == 0x80 then
			print('rotary turned right ' ..data[4] ..' clicks')
		end

		if data[5] == 0x7f then
			print('rotary turned left ' ..255 - data[4] ..' clicks')
		end
	end

end

function onTick()
	-- wake up, need to send once every 1.5 second but we can send more often because why not
	txCan(1, IDRIVE_HEARBEAT, 0, { 3 })
	txCan(1, IDRIVE_BRIGHTNESS, 0, { brighness * 16 + 0xd })
end

