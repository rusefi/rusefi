package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;

import java.io.IOException;
import java.util.List;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;

public class PlainSerialPortScannerSandbox {
    public static void main(String[] args) {

        Set<String> serialPorts = LinkManager.getCommPorts();
        final List<Thread> threads = serialPorts.stream().map(port -> {
            final String threadName = "PlainSerialPortScanner inspectPort " + port;

            Thread t = new Thread(() -> {

                try (LinkManager linkManager = new LinkManager()
                        .setNeedPullText(false)
                        .setNeedPullLiveData(false)
                ) {
                    linkManager.start(port, s -> {
                    });
                    linkManager.getConnector().connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false),
                            new ConnectionStateListener() {
                                @Override
                                public void onConnectionEstablished() {
                                    System.out.println("onConnectionEstablished " + port);
                                    PlainSerialPortScannerSandbox.onConnectionEstablished(port, linkManager);
                                }

                                @Override
                                public void onConnectionFailed(String s) {
                                    System.out.println("onConnectionFailed " + port);

                                }
                            });

                }
            });

            t.setName(threadName);
            t.setDaemon(true);
            t.start();

            return t;
        }).collect(Collectors.toList());

        // Give everyone a chance to finish
        try {
            // todo: see if everyone has already finished - make this sleep conditional!
            // todo: lower this timeout?
            Thread.sleep(50000000);
        } catch (InterruptedException e) {
            // We got interrupted because the last port got found, nothing to do
        }

        SerialPortScanner.interruptThreads(threads);
    }

    private static void onConnectionEstablished(String port, LinkManager linkManager) {
        try {
            BinaryProtocol binaryProtocol = linkManager.getBinaryProtocol();
            Objects.requireNonNull(binaryProtocol, "binaryProtocol");
            IoStream stream = binaryProtocol.getStream();
            boolean hasOpenBlt = OpenbltDetectorStrategy.streamHasOpenBlt(stream);
            String signature = binaryProtocol.signature;
            linkManager.close();
            if (hasOpenBlt) {
                RusEfiSignature s = SignatureHelper.parse(signature);
                System.out.println(port + " hasOpenBlt=" + hasOpenBlt + ", signature=" + s);

            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

}
