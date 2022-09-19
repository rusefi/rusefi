-- 11 bit MS protocol for TCM integration please select "listen to CAN broadcast" in "Use CAN for TPS/RPM/CLT"
-- TPS MAP CLT RPM
-- http://www.msextra.com/doc/pdf/Megasquirt_CAN_Broadcast.pdf

packetALL = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
setTickRate(66)
every100msTimer = Timer.new()
function onTick()
    rpm = getSensor("RPM")
    rpm = (rpm == nil and 0 or rpm)
    packetALL[3] = rpm / 256
    packetALL[4] = rpm
    tps = getSensor("Tps1")
    tps = (tps == nil and 0 or tps)
    tps = tps * 10
    packetALL[7] = tps / 256
    packetALL[8] = tps
    clt = getSensor("Clt")
    clt = (clt == nil and 0 or clt)
    clt = clt * 1.8
    clt = clt + 32
    clt = clt * 10
    packetALL[5] = clt / 256
    packetALL[6] = clt
    MAP = getSensor("Map")
    MAP = (MAP == nil and 0 or MAP)
    MAP = MAP * 10
    packetALL[1] = MAP / 256
    packetALL[2] = MAP
    if every100msTimer:getElapsedSeconds() > 0.1 then
        every100msTimer:reset()
        txCan(1, 0x5E8, 0, packetALL)
    end
end