package com.rusefi.io.test;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.config.Field;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.tune.xml.Constant;
import org.jetbrains.annotations.NotNull;
import org.junit.Test;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import static org.junit.Assert.assertEquals;

public class TcpCommunicationIntegrationTest {
    @Test
    public void testConnect() throws InterruptedException {
        ConfigurationImage ci = prepareImage(239);

        int port = 6100;

        BinaryProtocolState state = new BinaryProtocolState();
        state.setController(ci);
        state.setCurrentOutputs(new byte[1 + Fields.TS_OUTPUT_SIZE]);

        LinkManager linkManager = new LinkManager(FileLog.LOGGER);
        linkManager.setConnector(LinkConnector.getDetachedConnector(state));
        BinaryProtocolServer server = new BinaryProtocolServer(FileLog.LOGGER);
        server.start(linkManager, port);

        CountDownLatch countDownLatch = new CountDownLatch(1);

        // todo: remove CONFIGURATION_RUSEFI_BINARY or nicer API to disable local file load

        LinkManager clientManager = new LinkManager(FileLog.LOGGER);
        clientManager.startAndConnect(Integer.toString(port), new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                countDownLatch.countDown();
            }

            @Override
            public void onConnectionFailed() {
                System.out.println("Failed");
            }
        });
        countDownLatch.await(30, TimeUnit.SECONDS);
        assertEquals(0, server.unknownCommands.get());
    }

    @NotNull
    private ConfigurationImage prepareImage(int input) {
        ConfigurationImage ci = new ConfigurationImage(Fields.TOTAL_CONFIG_SIZE);

        Field field = Fields.CYLINDERSCOUNT;

        ScalarIniField scalarIniField = new ScalarIniField(field.getName(), field.getOffset(), "", field.getType(), 1);
        scalarIniField.setValue(ci, new Constant(field.getName(), "", Integer.toString(input)));
        return ci;
    }
}
