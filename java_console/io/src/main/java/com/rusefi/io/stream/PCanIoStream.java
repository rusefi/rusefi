package com.rusefi.io.stream;

import com.devexperts.logging.Logging;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.can.IsoTpCanDecoder;
import com.rusefi.io.can.IsoTpConnector;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.tcp.BinaryProtocolServer;
import org.jetbrains.annotations.Nullable;
import peak.can.basic.*;

import java.io.IOException;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.config.generated.Fields.CAN_ECU_SERIAL_TX_ID;
import static peak.can.basic.TPCANMessageType.PCAN_MESSAGE_STANDARD;

public class PCanIoStream extends AbstractIoStream {
    static Logging log = getLogging(PCanIoStream.class);

    public static final TPCANHandle CHANNEL = TPCANHandle.PCAN_USBBUS1;
    private final IncomingDataBuffer dataBuffer = createDataBuffer("[PCAN] ");
    private final PCANBasic can;
    private final IsoTpCanDecoder canDecoder = new IsoTpCanDecoder() {
        @Override
        protected void onTpFirstFrame() {
            sendCanPacket(new byte[]{0x30, 0, 0, 0, 0, 0, 0, 0});
        }
    };

    private final IsoTpConnector isoTpConnector = new IsoTpConnector() {
        @Override
        public void sendCanData(byte[] hdr, byte[] data, int dataOffset, int dataLength) {
            byte[] total = combineArrays(hdr, data, dataOffset, dataLength);

            log.info("-------sendIsoTp " + total.length + " byte(s):");

            log.info("Sending " + IoStream.printHexBinary(total));

            sendCanPacket(total);
        }

        @Override
        public void receiveData() {
        }
    };

    @Nullable
    public static PCanIoStream getPCANIoStream() {
        PCANBasic can = new PCANBasic();
        can.initializeAPI();
        TPCANStatus status = can.Initialize(CHANNEL, TPCANBaudrate.PCAN_BAUD_500K, TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            log.info("Error initializing PCAN: " + status);
            return null;
        }
        log.info("Hello PCAN!");
        return new PCanIoStream(can);
    }

    private void sendCanPacket(byte[] payLoad) {
        TPCANMsg msg = new TPCANMsg(Fields.CAN_ECU_SERIAL_RX_ID, PCAN_MESSAGE_STANDARD.getValue(),
                (byte) payLoad.length, payLoad);
        TPCANStatus status = can.Write(CHANNEL, msg);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            log.info("Unable to write the CAN message: " + status);
            System.exit(0);
        }
//        log.info("Send OK! length=" + payLoad.length);
    }

    public PCanIoStream(PCANBasic can) {
        this.can = can;
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        IsoTpConnector.sendStrategy(bytes, isoTpConnector);
    }

    @Override
    public void setInputListener(DataListener listener) {
        Executor threadExecutor = Executors.newSingleThreadExecutor(BinaryProtocolServer.getThreadFactory("PCAN reader"));
        threadExecutor.execute(() -> {
            while (!isClosed()) {
                readOnePacket(listener);
            }
        });
    }

    private void readOnePacket(DataListener listener) {
        // todo: can we reuse instance?
        TPCANMsg rx = new TPCANMsg();
        TPCANStatus status = can.Read(CHANNEL, rx, null);
        if (status == TPCANStatus.PCAN_ERROR_OK) {
            log.info("Got [" + rx + "] id=" + rx.getID() + " len=" + rx.getLength() + ": " + IoStream.printByteArray(rx.getData()));
            if (rx.getID() != CAN_ECU_SERIAL_TX_ID) {
                log.info("Skipping non " + CAN_ECU_SERIAL_TX_ID + " packet");
                return;
            }
            byte[] decode = canDecoder.decodePacket(rx.getData());
            listener.onDataArrived(decode);

            //            log.info("Decoded " + IoStream.printByteArray(decode));
        } else {
//                   log.info("Receive " + status);
        }
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }
}
