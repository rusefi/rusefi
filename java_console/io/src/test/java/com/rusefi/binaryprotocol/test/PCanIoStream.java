package com.rusefi.binaryprotocol.test;

import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.can.IsoTpCanDecoder;
import com.rusefi.io.can.IsoTpConnector;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.tcp.BinaryProtocolServer;
import peak.can.basic.PCANBasic;
import peak.can.basic.TPCANMsg;
import peak.can.basic.TPCANStatus;

import java.io.IOException;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import static peak.can.basic.TPCANMessageType.PCAN_MESSAGE_STANDARD;

public class PCanIoStream extends AbstractIoStream {
    private final IncomingDataBuffer dataBuffer;
    private final PCANBasic can;
    private final IsoTpCanDecoder canDecoder = new IsoTpCanDecoder() {
        @Override
        protected void onTpFirstFrame() {
            sendCanPacket(new byte[]{0x30, 0, 0, 0, 0, 0, 0, 0});
        }
    };

    private final IsoTpConnector isoTpConnector = new IsoTpConnector() {
        @Override
        public void sendCanData(byte[] hdr, byte[] data, int offset, int len) {
            byte[] total = new byte[hdr.length + len];
            System.arraycopy(hdr, 0, total, 0, hdr.length);
            System.arraycopy(data, offset, total, hdr.length, len);

            log.info("-------sendIsoTp " + total.length + " byte(s):");

            System.out.println(IoStream.printHexBinary(total));

            sendCanPacket(total);
        }

        @Override
        public void receiveData() {
            System.out.println("empty receiveData");
        }
    };

    private void sendCanPacket(byte[] payLoad) {
        TPCANMsg msg = new TPCANMsg(Fields.CAN_ECU_SERIAL_RX_ID, PCAN_MESSAGE_STANDARD.getValue(),
                (byte) payLoad.length, payLoad);
        TPCANStatus status = can.Write(PCanSandbox.CHANNEL, msg);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            System.out.println("Unable to write the CAN message: " + status);
            System.exit(0);
        }
//        System.out.println("Send OK! length=" + payLoad.length);
    }

    private DataListener listener;

    public PCanIoStream(PCANBasic can) {
        this.can = can;
        this.dataBuffer = IncomingDataBuffer.createDataBuffer("", this);
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        IsoTpConnector.sendStrategy(bytes, isoTpConnector);
    }

    @Override
    public void setInputListener(DataListener listener) {
        this.listener = listener;
        Executor threadExecutor = Executors.newSingleThreadExecutor(BinaryProtocolServer.getThreadFactory("PCAN reader"));
        threadExecutor.execute(() -> {
            while (!isClosed()) {
                readOnePacket();
            }
        });
    }

    public void readOnePacket() {
        // todo: can we reuse instance?
        TPCANMsg rx = new TPCANMsg();
        TPCANStatus status = can.Read(PCanSandbox.CHANNEL, rx, null);
        if (status == TPCANStatus.PCAN_ERROR_OK) {
            System.out.println(rx + " id=" + rx.getID() + " len=" + rx.getLength() + ": " + IoStream.printByteArray(rx.getData()));
            byte[] decode = canDecoder.decodePacket(rx.getData());
            listener.onDataArrived(decode);

            System.out.println("Decoded " + IoStream.printByteArray(decode));
        } else {
//                   System.out.println(new Date() + ": Receive " + status);
        }
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }
}
