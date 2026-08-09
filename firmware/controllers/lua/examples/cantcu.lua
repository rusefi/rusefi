blip = 0
blipout = 0
cut = 0
cut2 = 0
cutout = 0

canRxAdd(0x301)

setTickRate(100)

function onCanRx(bus, id, dlc, data)

	if id == 0x301 then
		inter = data [6]

		cut = data [7]
		cut2 = curve(1, cut)
		cutout = - cut2
		setTimingAdd(cutout)

		blip = data [8]
		blipout = 0.3 * blip
		setEtbAdd(blipout)
	end

end

function onTick()

	print('cut: ' ..cutout)
	print('blip: ' ..blip)
	print('blipout:' ..blipout)
end
