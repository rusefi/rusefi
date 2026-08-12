setTickRate(20)
enableCanTx(true)
yourTimer = Timer.new();
yourTimer:reset();
txPayload = {}

function startup()

txPayload[1] = 0 
txPayload[2] = 140
txPayload[3] = 160
txPayload[4] = 21
txPayload[5] = 224
txCan(1, 0x124, 0, txPayload)

txPayload[1] = 0 
txPayload[2] = 100
txPayload[3] = 0
txPayload[4] = 0
txPayload[5] = 0
txCan(1, 0x310, 0, txPayload)

end

function running()
surpm = getSensor("RPM")*4

txPayload[1] = readPin("PB1") 
txPayload[2] = math.floor(surpm / 256)
txPayload[3] = math.floor(((surpm/256) - math.floor(surpm/256))*256)
txPayload[4] = 21
txPayload[5] = 224

txCan(1, 0x124, 0, txPayload)

txPayload[1] = 0 

if getSensor("VehicleSpeed") == nil then 
txPayload[2] = 0
else txPayload[2] = getSensor("VehicleSpeed")/2
end

txPayload[3] = getSensor("IAT")+10
txPayload[4] =  getSensor("CLT")+35
txPayload[5] = 0

txCan(1, 0x310, 0, txPayload)

end

function onTick()

if yourTimer:getElapsedSeconds() < 2 then startup() 
else running() end

end
