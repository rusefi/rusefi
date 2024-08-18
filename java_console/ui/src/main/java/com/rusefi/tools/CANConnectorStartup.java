package com.rusefi.tools;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.io.serial.RateCounter;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.ui.StatusConsumer;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.io.IOException;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

public class CANConnectorStartup {
    public static void start(AbstractIoStream tsStream, StatusConsumer statusListener) throws IOException {
        if (tsStream == null)
            throw new IOException("Failed to initialise connector");

        String signature = BinaryProtocol.getSignature(tsStream);
        if (signature == null) {
            statusListener.append("Error: no ECU signature from " + tsStream);
        } else {
            statusListener.append("Got [" + signature + "] ECU signature via " + tsStream);
        }
        Map<Byte, RateCounter> rateCounters = new HashMap<>();

        Timer everySecond = new Timer(1000, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent event) {
                for (Map.Entry<Byte, RateCounter> e : rateCounters.entrySet()) {
                    String name = BinaryProtocol.findCommand(e.getKey());

                    statusListener.append(new Date() + ": Command " + name + ": " + e.getValue().getCurrentRate());
                }
            }
        });
        everySecond.start();
        BinaryProtocolProxy.createProxy(tsStream, TcpConnector.DEFAULT_PORT, clientRequest -> {
            byte[] packet = clientRequest.getPacket();
            if (packet.length == 0)
                throw new IllegalStateException("Zero size packet not expected");
            byte commandId = packet[0];
            RateCounter counter = rateCounters.computeIfAbsent(commandId, command -> new RateCounter());
            counter.add();
        }, statusListener);
    }
}
