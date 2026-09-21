package com.rusefi.io.can;

import com.devexperts.logging.Logging;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.io.can.isotp.DefaultFlowControl;
import com.rusefi.util.HexBinary;
import com.rusefi.io.can.isotp.IsoTpCanDecoder;
import com.rusefi.io.can.isotp.IsoTpConnector;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.serial.RateCounter;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.ui.StatusConsumer;
import org.jetbrains.annotations.Nullable;
import peak.can.basic.*;

import java.io.IOException;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.config.generated.VariableRegistryValues.CAN_ECU_SERIAL_TX_ID;

public class PCanIoStream extends AbstractIoStream {
    private static final int INFO_SKIP_RATE = 3-00;
    static Logging log = getLogging(PCanIoStream.class);

    private final IncomingDataBuffer dataBuffer;
    interface Driver {
        TPCANStatus read(TPCANMsg message);
        TPCANStatus write(int id, byte[] payload);
        TPCANStatus uninitialize();
    }

    interface Sleeper {
        void sleep(long milliseconds) throws InterruptedException;
    }

    private final Driver can;
    private final StatusConsumer statusListener;
    private final Supplier<Executor> readerExecutorFactory;
    private final Sleeper sleeper;

    private final RateCounter totalCounter = new RateCounter();
    private final RateCounter isoTpCounter = new RateCounter();
    private final IsoTpCanDecoder canDecoder = new IsoTpCanDecoder() {
        @Override
        protected void onTpFirstFrame() {
            sendCanPacket(DefaultFlowControl.FLOW_CONTROL);
        }
    };

    private final IsoTpConnector isoTpConnector = new IsoTpConnector(VariableRegistryValues.CAN_ECU_SERIAL_RX_ID) {
        @Override
        public void sendCanData(byte[] total) {
            sendCanPacket(total);
        }
    };
    private int logSkipRate;

    @Nullable
    public static PCanIoStream createStream() {
        return createStream((message) -> log.info(message));
    }

    public static PCanIoStream createStream(StatusConsumer statusListener) {
        PCANBasic can = PCanHelper.create();
        TPCANStatus status = PCanHelper.init(can);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            statusListener.logLine("Error initializing PCAN: " + status);
            return null;
        }
        statusListener.logLine("Creating PCAN stream...");
        return new PCanIoStream(can, statusListener);
    }

    private void sendCanPacket(byte[] payLoad) {
        if (log.debugEnabled())
            log.debug("-------sendIsoTp " + payLoad.length + " byte(s):");

        if (log.debugEnabled())
            log.debug("Sending " + HexBinary.printHexBinary(payLoad));

        TPCANStatus status = can.write(isoTpConnector.canId(), payLoad);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            statusListener.logLine("Unable to write the CAN message: " + status);
            System.exit(0);
        }
//        log.info("Send OK! length=" + payLoad.length);
    }

    private PCanIoStream(PCANBasic can, StatusConsumer statusListener) {
        this(new Driver() {
                 @Override
                 public TPCANStatus read(TPCANMsg message) {
                     return can.Read(PCanHelper.CHANNEL, message, null);
                 }

                 @Override
                 public TPCANStatus write(int id, byte[] payload) {
                     return PCanHelper.send(can, id, payload);
                 }

                 @Override
                 public TPCANStatus uninitialize() {
                     return can.Uninitialize(PCanHelper.CHANNEL);
                 }
             }, statusListener,
            () -> Executors.newSingleThreadExecutor(BinaryProtocolServer.getThreadFactory("PCAN reader")),
            milliseconds -> {
                if (milliseconds > 0) {
                    Thread.sleep(milliseconds);
                }
            });
    }

    // Hardware-free test seams: use the real reader loop with a scripted driver/executor.
    PCanIoStream(Driver can, StatusConsumer statusListener, Supplier<Executor> readerExecutorFactory, Sleeper sleeper) {
        this.can = can;
        this.statusListener = statusListener;
        this.readerExecutorFactory = readerExecutorFactory;
        this.sleeper = sleeper;
        dataBuffer = createDataBuffer();
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        IsoTpConnector.sendStrategy(bytes, isoTpConnector);
    }

    @Override
    public void setInputListener(DataListener listener) {
        Executor threadExecutor = readerExecutorFactory.get();
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
        TPCANStatus status = can.read(rx);
        if (status == TPCANStatus.PCAN_ERROR_OK) {
            totalCounter.add();
            if (rx.getID() != CAN_ECU_SERIAL_TX_ID) {
//                if (log.debugEnabled())
                logSkipRate ++;
                if (logSkipRate % INFO_SKIP_RATE == 0) {
                    PCanHelper.debugPacket(rx);
                    log.info("Skipping non " + String.format("%X", CAN_ECU_SERIAL_TX_ID) + " packet: " + String.format("%X", rx.getID()));
                    log.info("Total rate " + totalCounter.getCurrentRate() + ", isotp rate " + isoTpCounter.getCurrentRate());
                }
                return;
            }
            PCanHelper.debugPacket(rx);
            isoTpCounter.add();
            byte[] decode = canDecoder.decodePacket(rx.getData());
            listener.onDataArrived(decode);

            //            log.info("Decoded " + IoStream.printByteArray(decode));
        } else {
//                   log.info("Receive " + status);
            if (status == TPCANStatus.PCAN_ERROR_QRCVEMPTY) {
                try {
                    // An empty receive queue returns immediately; avoid busy-spinning.
                    sleeper.sleep(1);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    close();
                }
            }
        }
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    // Use AbstractIoStream's thread-safe isClosed() without taking this lock.
    // Otherwise, reading a response and closing the stream can each hold a lock
    // the other needs, leaving both threads stuck.
    @Override
    public synchronized void close() {
        if (isClosed()) {
            return;
        }
        try {
            // MacCAN requires releasing the old channel before reconnect can claim it.
            TPCANStatus status = can.uninitialize();
            if (status != TPCANStatus.PCAN_ERROR_OK) {
                statusListener.logLine("Unable to uninitialize PCAN: " + status);
            }
        } finally {
            super.close();
        }
    }

    @Override
    public String toString() {
        return "PCanIoStream{" + PCanHelper.CHANNEL + ", " +
            "totalCounter=" + totalCounter +
            '}';
    }
}
