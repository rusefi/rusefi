package com.rusefi.io.stream;

import com.devexperts.logging.Logging;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import com.rusefi.io.can.IsoTpCanDecoder;
import com.rusefi.io.can.IsoTpConnector;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.serial.RateCounter;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.ui.StatusConsumer;
import org.jetbrains.annotations.Nullable;
import peak.can.basic.*;

import java.io.IOException;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.config.generated.Fields.CAN_ECU_SERIAL_TX_ID;
import static peak.can.basic.TPCANMessageType.PCAN_MESSAGE_STANDARD;

public class PCanIoStream extends AbstractIoStream {
    private static final int INFO_SKIP_RATE = 3-00;
    static Logging log = getLogging(PCanIoStream.class);

    public static final TPCANHandle CHANNEL = TPCANHandle.PCAN_USBBUS1;
    private final IncomingDataBuffer dataBuffer;
    private final PCANBasic can;
    private final StatusConsumer statusListener;

    private final RateCounter totalCounter = new RateCounter();
    private final RateCounter isoTpCounter = new RateCounter();
    private final IsoTpCanDecoder canDecoder = new IsoTpCanDecoder() {
        @Override
        protected void onTpFirstFrame() {
            sendCanPacket(FLOW_CONTROL);
        }
    };

    private final IsoTpConnector isoTpConnector = new IsoTpConnector() {
        @Override
        public void sendCanData(byte[] hdr, byte[] data, int dataOffset, int dataLength) {
            byte[] total = combineArrays(hdr, data, dataOffset, dataLength);

            sendCanPacket(total);
        }

        @Override
        public void receiveData() {
        }
    };
    private int logSkipRate;

    @Nullable
    public static PCanIoStream createStream() {
        return createStream(message -> log.info(message));
    }

    public static PCanIoStream createStream(StatusConsumer statusListener) {
        PCANBasic can = new PCANBasic();
        can.initializeAPI();
        TPCANStatus status = can.Initialize(CHANNEL, TPCANBaudrate.PCAN_BAUD_500K, TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            statusListener.append("Error initializing PCAN: " + status);
            return null;
        }
        statusListener.append("Creating PCAN stream...");
        return new PCanIoStream(can, statusListener);
    }

    private void sendCanPacket(byte[] payLoad) {
        if (log.debugEnabled())
            log.debug("-------sendIsoTp " + payLoad.length + " byte(s):");

        if (log.debugEnabled())
            log.debug("Sending " + IoStream.printHexBinary(payLoad));

        TPCANMsg msg = new TPCANMsg(Fields.CAN_ECU_SERIAL_RX_ID, PCAN_MESSAGE_STANDARD.getValue(),
                (byte) payLoad.length, payLoad);
        TPCANStatus status = can.Write(CHANNEL, msg);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            statusListener.append("Unable to write the CAN message: " + status);
            System.exit(0);
        }
//        log.info("Send OK! length=" + payLoad.length);
    }

    private PCanIoStream(PCANBasic can, StatusConsumer statusListener) {
        this.can = can;
        this.statusListener = statusListener;
        dataBuffer = createDataBuffer("[PCAN] ");
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
        // todo: should be? TPCANMsg rx = new TPCANMsg();
        // https://github.com/rusefi/rusefi/issues/4370 nasty work-around
        TPCANMsg rx = new TPCANMsg(Byte.MAX_VALUE);
        TPCANStatus status = can.Read(CHANNEL, rx, null);
        if (status == TPCANStatus.PCAN_ERROR_OK) {
            totalCounter.add();
            if (rx.getID() != CAN_ECU_SERIAL_TX_ID) {
//                if (log.debugEnabled())
                logSkipRate ++;
                if (logSkipRate % INFO_SKIP_RATE == 0) {
                    debugPacket(rx);
                    log.info("Skipping non " + String.format("%X", CAN_ECU_SERIAL_TX_ID) + " packet: " + String.format("%X", rx.getID()));
                    log.info("Total rate " + totalCounter.getCurrentRate() + ", isotp rate " + isoTpCounter.getCurrentRate());
                }
                return;
            }
            debugPacket(rx);
            isoTpCounter.add();
            byte[] decode = canDecoder.decodePacket(rx.getData());
            listener.onDataArrived(decode);

            //            log.info("Decoded " + IoStream.printByteArray(decode));
        } else {
//                   log.info("Receive " + status);
        }
    }

    private void debugPacket(TPCANMsg rx) {
        if (log.debugEnabled())
            log.debug("Got [" + rx + "] id=" + String.format("%X", rx.getID()) + " len=" + rx.getLength() + ": " + IoStream.printByteArray(rx.getData()));
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }
}
