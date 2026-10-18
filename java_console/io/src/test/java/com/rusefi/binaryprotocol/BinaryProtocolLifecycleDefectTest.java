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

/** Passing characterization: logger startup registers a listener after close. Update expectations with the fix. */
class BinaryProtocolLifecycleDefectTest {
    @Test
    void disconnectAtConnectionCompletionRegistersRpmListenerAfterClose() throws Exception {
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
                        // Deterministic transport-close interleaving after the final isClosed check,
                        // before BinaryProtocolLogger.start(). No polling thread is needed.
                        stream.close();
                        return false;
                    }
                }.setNotifyGlobalStatusOnClose(false);
                BinaryProtocol protocol = new BinaryProtocol(link, stream);
                assertNull(protocol.connectAndReadConfiguration(new BinaryProtocol.Arguments(false, false),
                    bytes -> { }), "Current defect: connection completion reports success after close");
                assertTrue(protocol.isClosed());
                protocol.close(); // Idempotent stream close does not replay logger cleanup.
                assertEquals(before + cycle, sensors.getSensorListenerCountForTests(),
                    "Current defect: one permanent RPM listener per close/start interleaving");
            }
        } finally {
            BinaryProtocol.iniFileProvider = previousProvider;
            for (TestStream stream : streams) {
                stream.close();
                // Undo the leak by explicitly replaying the callbacks captured by the fake transport.
                for (Runnable callback : stream.closeCallbacks) {
                    callback.run();
                }
            }
        }
        assertEquals(before, sensors.getSensorListenerCountForTests());
    }

    private static class TestStream extends AbstractIoStream {
        final List<Runnable> closeCallbacks = new ArrayList<>();
        private final IncomingDataBuffer buffer = mock(IncomingDataBuffer.class);

        TestStream() throws Exception {
            when(buffer.getPacket(Timeouts.BINARY_IO_TIMEOUT, "[hello]"))
                .thenReturn("\u0000lifecycle-test\u0000".getBytes(StandardCharsets.US_ASCII));
        }

        @Override
        public void addCloseListener(Runnable callback) {
            closeCallbacks.add(callback);
            super.addCloseListener(callback);
        }

        @Override
        public void setInputListener(DataListener listener) { }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            return buffer;
        }
    }
}
