-- todo: add documentation
-- todo: code formatting and comments?

inter = 0
cut = 0
blip = 0
blipout = 0
cutout = 0

canRxAdd(0x301) -- cantcu cut blip intervention

function onCanRx(bus, id, dlc, data)
if id == 0x301 then
		inter = data [6]
		cut = data [7]
		blip = data [8]
	end
end

function Trans()
	rpm = getSensor("RPM")
	if inter == 1 then
		blipout = table3d(1, rpm, blip)
	else
		blipout = 0
	end
	if inter == 2 then
		cut2 = curve(1, cut)
		cutout = - cut2
	else
		cutout = 0
	end
	if inter == 0 then
		blipout = 0
		cutout = 0
	end

end

setTickRate(50)

function onTick()

	Trans()
        setEtbAdd(blipout)
	setTimingAdd(cutout)

end
