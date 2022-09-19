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
import tel.schich.javacan.CanChannels;
import tel.schich.javacan.CanFrame;
import tel.schich.javacan.NetworkDevice;
import tel.schich.javacan.RawCanChannel;

import java.io.IOException;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.config.generated.Fields.CAN_ECU_SERIAL_TX_ID;
import static tel.schich.javacan.CanFrame.FD_NO_FLAGS;
import static tel.schich.javacan.CanSocketOptions.RECV_OWN_MSGS;

public class SocketCANIoStream extends AbstractIoStream {
    static Logging log = getLogging(SocketCANIoStream.class);
    private final IncomingDataBuffer dataBuffer;
    private final RawCanChannel socket;

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

    private void sendCanPacket(byte[] total) {
        if (log.debugEnabled())
            log.debug("-------sendIsoTp " + total.length + " byte(s):");

        if (log.debugEnabled())
            log.debug("Sending " + IoStream.printHexBinary(total));

        CanFrame packet = CanFrame.create(Fields.CAN_ECU_SERIAL_RX_ID, FD_NO_FLAGS, total);
        try {
            socket.write(packet);
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public SocketCANIoStream() {
        try {
            NetworkDevice canInterface = NetworkDevice.lookup(System.getProperty("CAN_DEVICE_NAME", "can0"));
            socket = CanChannels.newRawChannel();
            socket.bind(canInterface);

            socket.configureBlocking(true); // we want reader thread to wait for messages
            socket.setOption(RECV_OWN_MSGS, false);
        } catch (IOException e) {
            throw new IllegalStateException("Error looking up", e);
        }
        // buffer could only be created once socket variable is not null due to callback
        dataBuffer = createDataBuffer("[SocketCAN] ");
    }

    @Nullable
    public static SocketCANIoStream create() {
        return new SocketCANIoStream();
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        IsoTpConnector.sendStrategy(bytes, isoTpConnector);
    }

    @Override
    public void setInputListener(DataListener listener) {
        Executor threadExecutor = Executors.newSingleThreadExecutor(BinaryProtocolServer.getThreadFactory("SocketCAN reader"));
        threadExecutor.execute(() -> {
            while (!isClosed()) {
                readOnePacket(listener);
            }
        });
    }

    private void readOnePacket(DataListener listener) {
        try {
            CanFrame rx = socket.read();
            if (log.debugEnabled())
                log.debug("GOT " + String.format("%X", rx));
            if (rx.getId() != CAN_ECU_SERIAL_TX_ID) {
                if (log.debugEnabled())
                    log.debug("Skipping non " + String.format("%X", CAN_ECU_SERIAL_TX_ID) + " packet: " + String.format("%X", rx.getId()));
                return;
            }
            byte[] raw = new byte[rx.getDataLength()];
            rx.getData(raw, 0, raw.length);
            byte[] decode = canDecoder.decodePacket(raw);
            listener.onDataArrived(decode);
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    @Override
    public IncomingDataBuffer getDataBuffer() {
        return dataBuffer;
    }

    public static IoStream createStream() {
        return new SocketCANIoStream();
    }
}
