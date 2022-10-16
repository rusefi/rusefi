package com.rusefi.proxy;

import com.devexperts.logging.Logging;
import com.rusefi.NamedThreadFactory;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.commands.HelloCommand;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;
import com.rusefi.server.SessionDetails;

import java.io.IOException;
import java.net.Socket;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.core.FileUtil.close;

public class BaseBroadcastingThread {
    private static final Logging log = getLogging(BaseBroadcastingThread.class);
    private static final NamedThreadFactory BASE_BROADCASTING_THREAD = new NamedThreadFactory("network connector");
    private final Thread thread;

    @SuppressWarnings("InfiniteLoopStatement")
    public BaseBroadcastingThread(Socket socket, SessionDetails sessionDetails, TcpIoStream.DisconnectListener disconnectListener, NetworkConnectorContext context) {
        thread = BASE_BROADCASTING_THREAD.newThread(() -> {
            TcpIoStream stream = null;
            try {
                stream = new TcpIoStream("[network connector] ", socket, disconnectListener);
                IncomingDataBuffer in = stream.getDataBuffer();
                boolean isFirstHello = true;
                while (true) {
                    int ioTimeout;
                    if (isFirstHello) {
                        log.info("Waiting for proxy server to request session details");
                        ioTimeout = context.firstPacketTimeout();
                    } else {
                        ioTimeout = context.consecutivePacketTimeout();
                    }
                    int length = BinaryProtocolServer.getPacketLength(in, () -> {
                        throw new UnsupportedOperationException();
                    }, ioTimeout);
                    BinaryProtocolServer.Packet packet = BinaryProtocolServer.readPromisedBytes(in, length);
                    byte[] payload = packet.getPacket();

                    byte command = payload[0];

                    if (isFirstHello && command == Fields.TS_HELLO_COMMAND) {
                        // first TS_HELLO_COMMAND is PROXY request, consecutive TS_HELLO_COMMAND would be real deal from user desktop application
                        isFirstHello = false;
                        // respond on hello request with information about session
                        log.info("Replying to controller connector@proxy: " + sessionDetails);
                        new HelloCommand(sessionDetails.toJson()).handle(stream);
                    } else {
                        handleCommand(packet, stream);
                    }
                }
            } catch (IOException e) {
                log.error("exiting thread " + e);
                close(stream);
            }
        });
    }

    protected void handleCommand(BinaryProtocolServer.Packet packet, TcpIoStream stream) throws IOException {
    }

    public void start() {
        thread.start();
    }
}
