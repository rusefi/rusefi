package com.rusefi.io.can;

import com.devexperts.logging.Logging;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.io.can.isotp.DefaultFlowControl;
import com.rusefi.io.can.isotp.IsoTpCanDecoder;
import com.rusefi.io.can.isotp.IsoTpConnector;
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
    private static final int INFO_SKIP_RATE = 3 - 00;
    private static final int FLOW_CONTROL_TIMEOUT_MS = 200;
    static Logging log = getLogging(PCanIoStream.class);

    private final IncomingDataBuffer dataBuffer;
    private final RawCanPort can;
    private final StatusConsumer statusListener;
    private final Supplier<Executor> readerExecutorFactory;
    private final Sleeper sleeper;
    private final Object flowControlMonitor = new Object();
    private boolean waitingForFlowControl;
    private boolean flowControlTerminal;
    private FlowControl flowControl;
    private volatile DataListener inputListener;

    private final RateCounter totalCounter = new RateCounter();
    private final RateCounter isoTpCounter = new RateCounter();
    private final IsoTpCanDecoder canDecoder = new IsoTpCanDecoder() {
        @Override
        protected void onTpFirstFrame() {
            sendCanPacket(DefaultFlowControl.FLOW_CONTROL);
        }

        @Override
        protected void onTpDecodeError(String message) {
            throw new IllegalStateException(message);
        }
    };

    private final IsoTpConnector isoTpConnector = new IsoTpConnector(VariableRegistryValues.CAN_ECU_SERIAL_RX_ID) {
        @Override
        public void sendCanData(byte[] total) {
            sendCanPacket(total);
        }

        @Override
        public void receiveData() {
            awaitFlowControl();
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
        // One reader feeds both the buffer and the optional listener.
        dataBuffer = new IncomingDataBuffer(getClass().getSimpleName(), getStreamStats());
        startReader();
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        boolean multiFrame = bytes.length > 7;
        if (multiFrame) {
            prepareForFlowControl();
        }
        try {
            IsoTpConnector.sendStrategy(bytes, isoTpConnector);
        } catch (UncheckedIOException e) {
            close();
            throw e.getCause();
        } catch (RuntimeException e) {
            close();
            throw e;
        } finally {
            if (multiFrame) {
                cancelFlowControlWait();
            }
        }
    }

    private void prepareForFlowControl() {
        synchronized (flowControlMonitor) {
            waitingForFlowControl = true;
            flowControlTerminal = false;
            flowControl = null;
        }
    }

    private void cancelFlowControlWait() {
        synchronized (flowControlMonitor) {
            waitingForFlowControl = false;
            flowControlTerminal = false;
            flowControl = null;
            flowControlMonitor.notifyAll();
        }
    }

    private void awaitFlowControl() {
        final long deadline = System.nanoTime() + FLOW_CONTROL_TIMEOUT_MS * 1_000_000L;
        while (true) {
            FlowControl received;
            synchronized (flowControlMonitor) {
                while (waitingForFlowControl && flowControl == null && !isClosed()) {
                    long remainingNanos = deadline - System.nanoTime();
                    if (remainingNanos <= 0) {
                        throw flowControlFailure("ISO-TP Flow Control timeout");
                    }
                    try {
                        flowControlMonitor.wait(Math.max(1, (remainingNanos + 999_999L) / 1_000_000L));
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        throw flowControlFailure("ISO-TP Flow Control wait interrupted", e);
                    }
                }
                if (!waitingForFlowControl || isClosed()) {
                    throw flowControlFailure("PCAN closed during Flow Control wait");
                }
                received = flowControl;
                flowControl = null;
            }

            if (received.failure != null) {
                throw flowControlFailure(received.failure);
            }
            if (received.status == 1) {
                // Keep waiting for CTS.
                continue;
            }
            return;
        }
    }

    private boolean publishFlowControl(byte[] payload) {
        synchronized (flowControlMonitor) {
            if (!waitingForFlowControl) {
                return true;
            }
            if (flowControlTerminal) {
                return true;
            }
            if (payload.length < 3) {
                flowControl = FlowControl.failure("Malformed ISO-TP Flow Control");
            } else {
                int status = payload[0] & 0x0f;
                int blockSize = payload[1] & 0xff;
                int separationTime = payload[2] & 0xff;
                String failure = null;
                if (status == 0 && blockSize != 0) {
                    failure = "Unsupported ISO-TP block size: " + blockSize;
                } else if (status == 0 && separationTime != 0) {
                    failure = "Unsupported ISO-TP STmin: " + separationTime;
                } else if (status == 2) {
                    failure = "ECU aborted ISO-TP transfer";
                } else if (status != 0 && status != 1) {
                    failure = "Unknown ISO-TP Flow Control status: " + status;
                }
                flowControl = new FlowControl(status, failure);
            }
            flowControlTerminal = flowControl.status != 1;
            flowControlMonitor.notifyAll();
            return true;
        }
    }

    private static final class FlowControl {
        final int status;
        final String failure;

        FlowControl(int status, String failure) {
            this.status = status;
            this.failure = failure;
        }

        static FlowControl failure(String message) {
            return new FlowControl(-1, message);
        }
    }

    private UncheckedIOException flowControlFailure(String message) {
        return new UncheckedIOException(new IOException(message));
    }

    private UncheckedIOException flowControlFailure(String message, Throwable cause) {
        return new UncheckedIOException(new IOException(message, cause));
    }

    private void startReader() {
        Executor threadExecutor = readerExecutorFactory.get();
        try {
            threadExecutor.execute(() -> {
                try {
                    while (!isClosed()) {
                        try {
                            readOnePacket();
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

    @Override
    public void setInputListener(DataListener listener) {
        inputListener = listener;
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

    private void readOnePacket() throws IOException {
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
            byte[] payload = frame.getPayload();
            if (payload.length > 0 && (payload[0] & 0xf0) == 0x30) {
                publishFlowControl(payload);
                return;
            }
            final byte[] decode;
            try {
                decode = canDecoder.decodePacket(payload);
            } catch (UncheckedIOException e) {
                throw e.getCause();
            } catch (RuntimeException e) {
                statusListener.logLine("PCAN decode failed: " + e.getMessage());
                log.error("PCAN ISO-TP decode failed", e);
                close();
                throw e;
            }
            dataBuffer.addData(decode);
            DataListener listener = inputListener;
            if (listener != null) {
                listener.onDataArrived(decode);
            }
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
        synchronized (flowControlMonitor) {
            waitingForFlowControl = false;
            flowControl = null;
            flowControlMonitor.notifyAll();
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
