package com.rusefi.io.can;

import com.devexperts.logging.Logging;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.io.can.isotp.DefaultFlowControl;
import com.rusefi.io.can.isotp.IsoTpCanDecoder;
import com.rusefi.io.can.isotp.IsoTpConnector;
import com.rusefi.io.can.isotp.IsoTpConstants;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.serial.RateCounter;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.ui.StatusConsumer;
import com.rusefi.util.HexBinary;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.io.UncheckedIOException;
import java.util.Optional;
import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.config.generated.VariableRegistryValues.CAN_ECU_SERIAL_TX_ID;

public class PCanIoStream extends AbstractIoStream {
    private static final int INFO_SKIP_RATE = 300;
    /** how long to wait for the ECU's ISO-TP flow control frame before sending the consecutive burst */
    private static final int FLOW_CONTROL_TIMEOUT_MS = 200;
    static Logging log = getLogging(PCanIoStream.class);

    private final IncomingDataBuffer dataBuffer;
    private final RawCanPort can;
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

    // Flow control handshake state, shared between the reader thread (readOnePacket) and
    // the writer thread (IsoTpConnector.receiveData() override below)
    private final Object flowControlMonitor = new Object();
    private boolean flowControlReceived;

    private final IsoTpConnector isoTpConnector = new IsoTpConnector(VariableRegistryValues.CAN_ECU_SERIAL_RX_ID) {
        @Override
        public void sendCanData(byte[] total) {
            sendCanPacket(total);
        }

        @Override
        public void receiveData() {
            // Wait for the ECU's flow control frame (sent in response to our FIRST frame) before
            // bursting the consecutive frames. If no FC shows up within the timeout we send anyway,
            // which is no worse than the previous burst behavior.
            synchronized (flowControlMonitor) {
                try {
                    if (!flowControlReceived) {
                        flowControlMonitor.wait(FLOW_CONTROL_TIMEOUT_MS);
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                } finally {
                    flowControlReceived = false;
                }
            }
        }
    };
    private int logSkipRate;

    interface Sleeper {
        void sleep(long milliseconds) throws InterruptedException;
    }

    @Nullable
    public static PCanIoStream createStream() {
        return createStream((message) -> log.info(message));
    }

    public static PCanIoStream createStream(StatusConsumer statusListener) {
        return createStream(statusListener, new PCanRawPort());
    }

    static PCanIoStream createStream(StatusConsumer statusListener, RawCanPort can) {
        try {
            can.open(new CanAddress(CAN_ECU_SERIAL_TX_ID, false));
        } catch (IOException e) {
            statusListener.logLine("Error initializing PCAN: " + e.getMessage());
            return null;
        }
        statusListener.logLine("Creating PCAN stream...");
        return new PCanIoStream(can, statusListener);
    }

    private void sendCanPacket(byte[] payLoad) {
        if (log.debugEnabled()) {
            log.debug("-------sendIsoTp " + payLoad.length + " byte(s):");
            log.debug("Sending " + HexBinary.printHexBinary(payLoad));
        }

        try {
            can.send(new ClassicCanFrame(new CanAddress(isoTpConnector.canId(), false), payLoad));
        } catch (IOException e) {
            statusListener.logLine("Unable to write the CAN message: " + e.getMessage());
            throw new UncheckedIOException(e);
        }
    }

    private PCanIoStream(RawCanPort can, StatusConsumer statusListener) {
        this(can, statusListener,
            () -> Executors.newSingleThreadExecutor(BinaryProtocolServer.getThreadFactory("PCAN reader")),
            milliseconds -> {
                if (milliseconds > 0) {
                    Thread.sleep(milliseconds);
                }
            });
    }

    // Hardware-free test seams: use the real reader loop with a scripted port/executor.
    PCanIoStream(RawCanPort can, StatusConsumer statusListener,
                 Supplier<Executor> readerExecutorFactory, Sleeper sleeper) {
        this.can = can;
        this.statusListener = statusListener;
        this.readerExecutorFactory = readerExecutorFactory;
        this.sleeper = sleeper;
        dataBuffer = createDataBuffer();
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        try {
            IsoTpConnector.sendStrategy(bytes, isoTpConnector);
        } catch (UncheckedIOException e) {
            throw e.getCause();
        }
    }

    @Override
    public void setInputListener(DataListener listener) {
        Executor threadExecutor = readerExecutorFactory.get();
        try {
            threadExecutor.execute(() -> {
                try {
                    while (!isClosed()) {
                        try {
                            readOnePacket(listener);
                        } catch (IOException e) {
                            if (!isClosed()) {
                                statusListener.logLine("Unable to read the CAN message: " + e.getMessage());
                                log.error("PCAN read failed", e);
                                close();
                            }
                        }
                    }
                } finally {
                    closeStreamAndReaderExecutor(threadExecutor);
                }
            });
        } catch (RuntimeException | Error e) {
            closeStreamAndReaderExecutor(threadExecutor);
            throw e;
        }
    }

    private void closeStreamAndReaderExecutor(Executor executor) {
        try {
            // A failed decoder/listener or rejected task must not retain the native channel.
            close();
        } finally {
            if (executor instanceof ExecutorService) {
                ((ExecutorService) executor).shutdown();
            }
        }
    }

    private void readOnePacket(DataListener listener) throws IOException {
        Optional<ClassicCanFrame> received = can.receive(100);
        if (received.isPresent()) {
            ClassicCanFrame frame = received.get();
            totalCounter.add();
            if (frame.getAddress().isExtended() || frame.getAddress().getId() != CAN_ECU_SERIAL_TX_ID) {
                logSkipRate++;
                if (logSkipRate % INFO_SKIP_RATE == 0) {
                    log.info("Skipping non " + String.format("%X", CAN_ECU_SERIAL_TX_ID)
                        + " packet: " + frame.getAddress());
                    log.info("Total rate " + totalCounter.getCurrentRate() + ", isotp rate " + isoTpCounter.getCurrentRate());
                }
                return;
            }
            isoTpCounter.add();

            // The ECU's flow control frame (0x30 00 00...) carries no payload - it only paces our
            // multi-frame sends. Do not feed it to the ISO-TP decoder (it would surface as an empty
            // data chunk), just release the writer waiting in IsoTpConnector.receiveData().
            if (isFlowControlFrame(rx)) {
                onFlowControlReceived();
                return;
            }

            // decodePacket(byte[]) passes the buffer length (127, see TPCANMsg workaround above) instead of
            // the actual DLC, so multi-frame assembly and the CRC check read garbage zero padding. Use the
            // size-aware overload with the real frame length.
            byte[] decode = canDecoder.decodePacket(rx.getData(), rx.getLength());
            listener.onDataArrived(decode);
        } else {
            try {
                // An empty receive queue returns immediately; avoid busy-spinning.
                sleeper.sleep(1);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                close();
            }
        }
    }

    private static boolean isFlowControlFrame(TPCANMsg rx) {
        return rx.getLength() >= 1 && ((rx.getData()[0] >> 4) & 0xf) == IsoTpConstants.ISO_TP_FRAME_FLOW_CONTROL;
    }

    private void onFlowControlReceived() {
        synchronized (flowControlMonitor) {
            flowControlReceived = true;
            flowControlMonitor.notifyAll();
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
            // RawCanPort releases the native channel before close() returns. Do this
            // before super.close(), since close listeners may immediately reconnect.
            can.close();
        } catch (IOException e) {
            statusListener.logLine("Unable to close PCAN: " + e.getMessage());
            log.error("PCAN close failed", e);
        } finally {
            // This also makes close idempotent if a close listener throws.
            super.close();
        }
    }

    @Override
    public String toString() {
        return "PCanIoStream{" +
            "totalCounter=" + totalCounter +
            '}';
    }
}
