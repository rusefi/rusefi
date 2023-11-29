package com.rusefi.uds;

import com.rusefi.io.can.PCanHelper;
import org.jetbrains.annotations.NotNull;
import peak.can.basic.PCANBasic;
import peak.can.basic.TPCANMode;
import peak.can.basic.TPCANMsg;
import peak.can.basic.TPCANStatus;

public class PCanConnector {
    @NotNull
    static CanConnector createPCanConnector() {
        PCANBasic can = PCanHelper.create();
        CanConnector connector = new CanConnector() {
            @Override
            public void send(int id, byte[] payLoad) {
                PCanHelper.send(can, id, payLoad);
            }

            @Override
            public CanPacket read() {
                TPCANMsg rx = new TPCANMsg(Byte.MAX_VALUE);
                TPCANStatus status = can.Read(PCanHelper.CHANNEL, rx, null);
                if (status != TPCANStatus.PCAN_ERROR_OK) {
                    return null;
                }
                return new CanPacket() {
                    @Override
                    public int id() {
                        return rx.getID();
                    }

                    //
                    @Override
                    public byte[] payload() {
                        return rx.getData();
                    }
                };
            }
        };
        // does not seem to be filtering anything?
        can.FilterMessages(PCanHelper.CHANNEL, CanConnector.UDS_IN, CanConnector.UDS_IN, TPCANMode.PCAN_MODE_STANDARD);
        TPCANStatus status = PCanHelper.init(can);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            throw new IllegalStateException("Unhappy PCAN " + status);
        }
        return connector;
    }
}
