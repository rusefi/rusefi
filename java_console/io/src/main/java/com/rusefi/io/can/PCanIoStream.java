package com.rusefi.io.can;

import com.devexperts.logging.Logging;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.io.can.isotp.DefaultFlowControl;
import com.rusefi.util.HexBinary;
import com.rusefi.io.can.isotp.IsoTpCanDecoder;
import com.rusefi.io.can.isotp.IsoTpConnector;
import com.rusefi.io.can.isotp.IsoTpConstants;
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
import static com.rusefi.config.generated.VariableRegistryValues.CAN_ECU_SERIAL_TX_ID;

public class PCanIoStream extends AbstractIoStream {
    private static final int INFO_SKIP_RATE = 300;
    /** how long to wait for the ECU's ISO-TP flow control frame before sending the consecutive burst */
    private static final int FLOW_CONTROL_TIMEOUT_MS = 200;
    static Logging log = getLogging(PCanIoStream.class);

    private final IncomingDataBuffer dataBuffer;
    private final PCANBasic can;
    private final StatusConsumer statusListener;

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

        TPCANStatus status = PCanHelper.send(can, isoTpConnector.canId(), payLoad);
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            statusListener.logLine("Unable to write the CAN message: " + status);
            System.exit(0);
        }
//        log.info("Send OK! length=" + payLoad.length);
    }

    private PCanIoStream(PCANBasic can, StatusConsumer statusListener) {
        this.can = can;
        this.statusListener = statusListener;
        dataBuffer = createDataBuffer();
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
        TPCANStatus status = can.Read(PCanHelper.CHANNEL, rx, null);
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
            try {
                byte[] decode = canDecoder.decodePacket(rx.getData(), rx.getLength());
                listener.onDataArrived(decode);
            } catch (RuntimeException e) {
                // A foreign device on the bus may use the same CAN ID with non-ISO-TP content
                // (e.g. the OEM bus on a car). One bad frame must not kill the reader thread.
                log.info("Ignoring undecodable frame " + String.format("%X", rx.getID()) + " " + HexBinary.printHexBinary(rx.getData()) + ": " + e);
            }

            //            log.info("Decoded " + IoStream.printByteArray(decode));
        } else {
//                   log.info("Receive " + status);
            if (status == TPCANStatus.PCAN_ERROR_QRCVEMPTY) {
                // MacCAN's Read (unlike the Windows PCANBasic) does not block - it returns
                // QRCVEMPTY immediately, and a tight re-poll busy-spins a whole core.
                // 1 ms granularity is plenty: even the 800+ fps flood only arrives every ~1.2 ms.
                try {
                    Thread.sleep(1);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
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

    @Override
    public void close() {
        super.close();
        /* Release the PCAN channel: MacCAN is strictly single-client, so a
         * ConnectionWatchdog reconnect that skips Uninitialize makes the next
         * Initialize fail with PCAN_ERROR_INITIALIZE forever - the device is
         * occupied by this very process. */
        can.Uninitialize(PCanHelper.CHANNEL);
    }

    @Override
    public String toString() {
        return "PCanIoStream{" + PCanHelper.CHANNEL + ", " +
            "totalCounter=" + totalCounter +
            '}';
    }
}
