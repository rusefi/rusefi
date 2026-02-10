package com.rusefi.updater;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ScannerHelper;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.List;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;

public class PlainSerialPortScanner {
    private static final Logging log = getLogging(PlainSerialPortScanner.class);

    public static void findEcu(final PlainSerialEcuConsumer consumer, int delayMillis) {

        Set<String> serialPorts = LinkManager.getCommPorts();
        final List<Thread> threads = serialPorts.stream().map(port -> {
            final String threadName = "PlainSerialPortScanner inspectPort " + port;

            Thread t = new Thread(() -> {

                try (LinkManager linkManager = new LinkManager()
                    .setNeedPullText(false)
                    .setNeedPullLiveData(false)
                ) {
                    linkManager.start(port, ConnectionStatusLogic.Listener.VOID);
                    linkManager.getConnector().connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false),
                        new ConnectionStatusLogic.ConnectionStateListener() {
                            @Override
                            public void onConnectionEstablished() {
                                System.out.println("onConnectionEstablished " + port);
                                PlainSerialPortScanner.onConnectionEstablished(port, linkManager, consumer);
                            }

                            @Override
                            public void onConnectionFailed(String s) {
                                log.info("onConnectionFailed " + port);
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
            Thread.sleep(delayMillis);
        } catch (InterruptedException e) {
            // We got interrupted because the last port got found, nothing to do
        }

        ScannerHelper.interruptThreads(threads);
    }

    private static void onConnectionEstablished(String port, LinkManager linkManager, PlainSerialEcuConsumer consumer) {
        try {
            BinaryProtocol binaryProtocol = linkManager.getBinaryProtocol();
            Objects.requireNonNull(binaryProtocol, "binaryProtocol");
            IoStream stream = binaryProtocol.getStream();
            boolean hasOpenBlt = OpenbltDetectorStrategy.streamHasOpenBlt(stream);
            String signature = binaryProtocol.signature;
            linkManager.close();
            if (hasOpenBlt) {
                consumer.onFind(port, binaryProtocol.getIniFile());
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public interface PlainSerialEcuConsumer {
        void onFind(String port, @NotNull IniFileModel signature);
    }
}
