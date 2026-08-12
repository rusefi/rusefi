-- greyhill 3K default Baud Rate 250Kbps

-- When the device is first powered, it transmits the Boot message

NodeID = 0xA
bootCANID = 0x700 + NodeID

statusID = 0x180

NMTMasterID = 0
StartRemoteNodeCommand = 1
startPayLoad = {StartRemoteNodeCommand, NodeID}
busIndex = 1

txCan(busIndex, NMTMasterID, 0, startPayLoad)

function onCanOpenKeyPad(bus, id, dlc, data)
    print("onCanOpenKeyPad " + data[1])
end

canRxAdd(busIndex, statusID + NodeID, onCanOpenKeyPad)
