package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.io.DataListener;
import com.rusefi.Timeouts;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.AbstractIoStream;
import org.junit.jupiter.api.Test;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

/** Connection completion must not restart logging after stream cleanup. */
class BinaryProtocolLifecycleDefectTest {
    @Test
    void disconnectAtConnectionCompletionRejectsConnectionWithoutRetainingRpmListener() throws Exception {
        SensorCentral sensors = SensorCentral.getInstance();
        int before = sensors.getSensorListenerCountForTests();
        IniFileProvider previousProvider = BinaryProtocol.iniFileProvider;
        IniFileModel ini = mock(IniFileModel.class);
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(meta.getPageReadCommand(0)).thenReturn("R");
        when(meta.getPageSize(0)).thenReturn(4);
        List<TestStream> streams = new ArrayList<>();
        try {
            BinaryProtocol.iniFileProvider = signature -> ini;
            for (int cycle = 1; cycle <= 10; cycle++) {
                TestStream stream = new TestStream();
                streams.add(stream);
                LinkManager link = new LinkManager() {
                    @Override
                    public boolean getNeedPullData() {
                        // Deterministic transport-close interleaving after the calibration isClosed check,
                        // before BinaryProtocolLogger.start(). No polling thread is needed.
                        stream.close();
                        return false;
                    }
                }.setNotifyGlobalStatusOnClose(false);
                BinaryProtocol protocol = new BinaryProtocol(link, stream);
                assertNotNull(protocol.connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false),
                    bytes -> { }), "Connection completion must reject a closed stream");
                assertTrue(protocol.isClosed());
                protocol.close(); // Idempotent stream close does not replay logger cleanup.
                assertEquals(before, sensors.getSensorListenerCountForTests(),
                    "Closed connections must not retain RPM listeners");
            }
        } finally {
            BinaryProtocol.iniFileProvider = previousProvider;
            for (TestStream stream : streams) {
                stream.close();
            }
        }
        assertEquals(before, sensors.getSensorListenerCountForTests());
    }

    @Test
    void loggerStartIsIdempotentAndCannotRestartAfterClose() {
        assertLoggerLifecycle(false);
    }

    @Test
    void loggerClosedBeforeStartupNeverRegistersAListener() {
        assertLoggerLifecycle(true);
    }

    private void assertLoggerLifecycle(boolean closeBeforeStart) {
        SensorCentral sensors = SensorCentral.getInstance();
        int before = sensors.getSensorListenerCountForTests();
        LinkManager link = new LinkManager().setNotifyGlobalStatusOnClose(false);
        BinaryProtocolLogger logger = new BinaryProtocolLogger(link);
        try {
            if (closeBeforeStart) {
                logger.close();
            }
            logger.start();
            logger.start();
            assertEquals(before + (closeBeforeStart ? 0 : 1), sensors.getSensorListenerCountForTests());
            logger.close();
            logger.close();
            logger.start();
            assertEquals(before, sensors.getSensorListenerCountForTests());
        } finally {
            logger.close();
            link.close();
        }
    }

    private static class TestStream extends AbstractIoStream {
        private final IncomingDataBuffer buffer = mock(IncomingDataBuffer.class);

        TestStream() throws Exception {
            when(buffer.getPacket(Timeouts.BINARY_IO_TIMEOUT, "[hello]"))
                .thenReturn("\u0000lifecycle-test\u0000".getBytes(StandardCharsets.US_ASCII));
        }

        @Override
        public void setInputListener(DataListener listener) { }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            return buffer;
        }
    }
}
