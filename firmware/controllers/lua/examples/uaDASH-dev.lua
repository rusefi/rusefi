-- https://github.com/Light-r4y/uaDASH

canRxAdd(0x77000E)

function onCanRx(bus, id, dlc, data)
if id == 0x77000E then
print('got CAN id=' ..id ..' data=' .. data[1] .. ' ' .. data[2] .. ' ' .. data[3] .. ' ' .. data[4] .. ' ' .. data[5])
end
end

function onTick()

txPayload = {}
txPayload[1] = 0x66
txPayload[2] = 0x00
txPayload[3] = 0x00
txPayload[4] = 0x00
txPayload[5] = 0x00
txCan(1, 0x77000D, 1, txPayload)
end
