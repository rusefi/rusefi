-- in the 29 bit protocol we hard-code magic areas based on data point location within MS outputs structure
-- http://www.megamanual.com/com/CAN.htm

--TPS MAP CLT RPM
packetRPM = {0x00, 0x00}
packetCLT = {0x00, 0x00} --BIT 3 AND 4 MAP(0.1/KPA) & BIT 7 AND 8 CLT (0.1/F)
packetTPS = {0x00, 0x00} -- BIT 1 AND 2 TPS (0.1/%)
packetMAP = {0x00, 0x00}

setTickRate(66)

canRxAdd(0x18B838)
canRxAdd(0x58B838)
canRxAdd(0x60B838)
canRxAdd(0x48B838)


function onCanRx(bus, id, dlc, data)
    id11 = id % 2048
    if id11 == 0x18B838 then -- RPM
        txCan(1, 0x903B8, 1, packetRPM) -- Reply with RPM
    end
    if id11 == 0x58B838 then -- CLT
        txCan(1, 0x5903B8, 1, packetCLT) -- Reply with RPM
    end
    if id11 == 0x60B838 then -- TPS
        txCan(1, 0x6103B8, 1, packetTPS) -- Reply with RPM
    end
    if id11 == 0x48B838 then -- MAP
        txCan(1, 0x4903B8, 1, packetMAP) -- Reply with MAP - HOW MANY BYTESS????
    end

end



function onTick()

    rpm = getSensor("RPM")
    rpm = (rpm == nil and 0 or rpm)
    packetRPM[1] = rpm / 256
    packetRPM[2] = rpm

    tps = getSensor("TPS")
    tps = (tps == nil and 0 or tps)
    tps = tps * 10
    packetTPS[1] = tps / 256
    packetTPS[2] = tps

    clt = getSensor("Clt")
    clt = (clt == nil and 0 or clt)
    clt = clt * 1.8 + 134.2
    packetCLT[1] = clt / 256
    packetCLT[2] = clt

    MAP = getSensor("MAP")
    MAP = (MAP == nil and 0 or MAP)
    MAP = MAP * 10
    packetMAP[1] = map / 256
    packetMAP[2] = map
end
