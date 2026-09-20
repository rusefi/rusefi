package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.io.DataListener;
import com.rusefi.config.FieldType;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.OutputChannelDemand;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.WellKnownGauges;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.AbstractIoStream;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class BinaryProtocolConnectionStatusTest {
    private static final String SECONDS = WellKnownGauges.SECONDS.getOutputChannelName();
    private static final byte[] OUTPUTS = {Integration.TS_RESPONSE_OK, 42, 0};
    private final ConnectionStatusLogic status = ConnectionStatusLogic.INSTANCE;
    private final List<ConnectionStatusValue> transitions = new ArrayList<>();
    private final ConnectionStatusLogic.Listener listener = connected -> transitions.add(status.getValue());
    private ConnectionStatusValue previousStatus;
    private boolean previousCacheDisabled;
    private TestStream stream;
    private LinkManager link;
    private BinaryProtocol protocol;

    @BeforeEach
    void setUp() throws Exception {
        previousStatus = status.getValue();
        previousCacheDisabled = BinaryProtocolLocalCache.DISABLE_LOCAL_CONFIGURATION_CACHE;
        BinaryProtocolLocalCache.DISABLE_LOCAL_CONFIGURATION_CACHE = true;
        SensorCentral.getInstance().reset();
        status.setValue(ConnectionStatusValue.NOT_CONNECTED);
        status.addListener(listener);
        stream = new TestStream();
        link = new LinkManager();
        protocol = spy(new BinaryProtocol(link, stream));
        IniFileModel ini = IniFileModelMocks.empty();
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(meta.getOchBlockSize()).thenReturn(2);
        when(meta.getPageReadCommand(0)).thenReturn("R");
        when(ini.getBlockingFactor()).thenReturn(2);
        ScalarIniField seconds = new ScalarIniField(SECONDS, 0, "s", FieldType.UINT16, 1, "0", 0);
        when(ini.getAllOutputChannels()).thenReturn(Collections.singletonMap(SECONDS, seconds));
        when(ini.getOutputChannel(SECONDS)).thenReturn(seconds);
        protocol.setIniFileForUnitTest(ini);
        doReturn(OUTPUTS).when(protocol)
            .executeCommand(eq(Integration.TS_OUTPUT_COMMAND), any(byte[].class), anyString());
    }

    @AfterEach
    void tearDown() {
        status.removeListener(listener);
        stream.close();
        SensorCentral.getInstance().reset();
        BinaryProtocolLocalCache.DISABLE_LOCAL_CONFIGURATION_CACHE = previousCacheDisabled;
        status.setValue(previousStatus);
    }

    @Test
    void completeImageMovesThroughLoadingToConnected() {
        doReturn(new byte[]{Integration.TS_RESPONSE_OK, 1, 2}, new byte[]{Integration.TS_RESPONSE_OK, 3, 4})
            .when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());

        assertTrue(readImage());
        assertArrayEquals(new byte[]{1, 2, 3, 4}, protocol.getControllerConfiguration().getContent());
        assertEquals(Arrays.asList(ConnectionStatusValue.LOADING, ConnectionStatusValue.CONNECTED), transitions);
    }

    @Test
    void unplugDuringImageReadClearsLoadingAndRejectsPartialImage() {
        doReturn(new byte[]{Integration.TS_RESPONSE_OK, 1, 2}).doAnswer(invocation -> {
            assertEquals(ConnectionStatusValue.LOADING, status.getValue());
            stream.close();
            return null;
        }).when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());

        assertFalse(readImage());
        assertNull(protocol.getControllerConfiguration());
        assertEquals(Arrays.asList(ConnectionStatusValue.LOADING, ConnectionStatusValue.NOT_CONNECTED), transitions);
    }

    @Test
    void issue10282LateFinalImageResponseCurrentlyMarksClosedStreamConnected() {
        doReturn(new byte[]{Integration.TS_RESPONSE_OK, 1, 2}).doAnswer(invocation -> {
            assertEquals(ConnectionStatusValue.LOADING, status.getValue());
            stream.close();
            assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
            return new byte[]{Integration.TS_RESPONSE_OK, 3, 4};
        }).when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());

        // Related coverage-first characterization: unlike a failed middle chunk,
        // a valid final response bypasses the next loop's closed-stream check.
        // A fix should reject completion and leave the status NOT_CONNECTED.
        assertTrue(readImage());
        assertTrue(stream.isClosed());
        assertArrayEquals(new byte[]{1, 2, 3, 4}, protocol.getControllerConfiguration().getContent());
        assertEquals(Arrays.asList(ConnectionStatusValue.LOADING, ConnectionStatusValue.NOT_CONNECTED,
            ConnectionStatusValue.LOADING, ConnectionStatusValue.CONNECTED), transitions);
        stream.close();
        assertEquals(ConnectionStatusValue.CONNECTED, status.getValue());
    }

    @Test
    void unplugAfterOutputPollingClearsStatusAndSensorSnapshot() {
        status.setValue(ConnectionStatusValue.CONNECTED);
        assertTrue(poll());
        assertEquals(ConnectionStatusValue.CONNECTED, status.getValue());
        assertEquals(42, SensorCentral.getInstance().getValue(SECONDS));
        assertNotNull(SensorCentral.getInstance().getCurrentSnapshot());

        stream.close();

        assertFalse(status.isConnected());
        assertNull(SensorCentral.getInstance().getCurrentSnapshot());
        assertTrue(Double.isNaN(SensorCentral.getInstance().getValue(SECONDS)));
    }

    @Test
    void pollOnAlreadyClosedStreamDoesNotIssueRequestsOrReconnect() {
        status.setValue(ConnectionStatusValue.CONNECTED);
        stream.close();

        assertFalse(poll());
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
        verify(protocol, never()).executeCommand(eq(Integration.TS_OUTPUT_COMMAND), any(byte[].class), anyString());
    }

    @Test
    void failedResponseAfterUnplugDoesNotPublishOrReconnect() {
        status.setValue(ConnectionStatusValue.CONNECTED);
        doAnswer(invocation -> {
            stream.close();
            return null;
        }).when(protocol).executeCommand(eq(Integration.TS_OUTPUT_COMMAND), any(byte[].class), anyString());

        assertFalse(poll());
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
        assertNull(SensorCentral.getInstance().getCurrentSnapshot());
    }

    @Test
    void issue10282LateOutputResponseCurrentlyResurrectsLoadingAfterUnplug() {
        status.setValue(ConnectionStatusValue.CONNECTED);
        assertTrue(poll());
        assertEquals(42, SensorCentral.getInstance().getValue(SECONDS));
        transitions.clear();
        doAnswer(invocation -> {
            // Deterministic interleaving: the response has arrived, then the stream's
            // close callback runs before the communication thread decodes that response.
            stream.close();
            assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
            assertNull(SensorCentral.getInstance().getCurrentSnapshot());
            return OUTPUTS;
        }).when(protocol).executeCommand(eq(Integration.TS_OUTPUT_COMMAND), any(byte[].class), anyString());

        // Coverage-first characterization of #10282, NOT desired behavior. A fix should
        // reject this stale response and keep NOT_CONNECTED with no sensor snapshot.
        assertTrue(poll());
        assertTrue(stream.isClosed());
        assertEquals(Arrays.asList(ConnectionStatusValue.NOT_CONNECTED, ConnectionStatusValue.LOADING), transitions);
        assertTrue(status.isConnected(), "LOADING also drives the green connected indicator");
        assertNotNull(SensorCentral.getInstance().getCurrentSnapshot());
        assertEquals(42, SensorCentral.getInstance().getValue(SECONDS),
            "Even unchanged seconds are republished because close reset the sensor values");
        assertFalse(poll());
        stream.close(); // Close is idempotent; it cannot clear the resurrected status again.
        assertEquals(ConnectionStatusValue.LOADING, status.getValue());
    }

    @Test
    void probeCloseDoesNotDisconnectTheConsole() {
        link.setNotifyGlobalStatusOnClose(false);
        status.setValue(ConnectionStatusValue.CONNECTED);
        transitions.clear();

        stream.close();

        assertEquals(ConnectionStatusValue.CONNECTED, status.getValue());
        assertTrue(transitions.isEmpty());
    }

    @Test
    void probeWithoutImageDoesNotMarkTheConsoleConnected() {
        link.setNotifyGlobalStatusOnClose(false);

        assertTrue(protocol.readImage(new BinaryProtocol.Arguments(false, false),
            new ConfigurationImageMetaVersion0_0(4, "test")));

        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
        assertTrue(transitions.isEmpty());
    }

    private boolean readImage() {
        return protocol.readImage(new BinaryProtocol.Arguments(true, false),
            new ConfigurationImageMetaVersion0_0(4, "test"));
    }

    private boolean poll() {
        return protocol.requestOutputChannels(OutputChannelDemand.full(0));
    }

    private static class TestStream extends AbstractIoStream {
        private final IncomingDataBuffer buffer = new IncomingDataBuffer("connection status test", getStreamStats());

        @Override
        public void setInputListener(DataListener listener) {
        }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            return buffer;
        }
    }
}
