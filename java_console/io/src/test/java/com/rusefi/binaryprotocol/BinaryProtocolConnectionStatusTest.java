package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.ini.field.IniField;
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
import com.rusefi.io.can.PCanIoStream;
import com.rusefi.io.serial.AbstractIoStream;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class BinaryProtocolConnectionStatusTest {
    private static final String SECONDS = WellKnownGauges.SECONDS.getOutputChannelName();
    private static final byte[] OUTPUTS = {Integration.TS_RESPONSE_OK, 42, 0};
    private final ConnectionStatusLogic status = ConnectionStatusLogic.INSTANCE;
    private final List<ConnectionStatusValue> transitions = new ArrayList<>();
    private final AtomicLong clock = new AtomicLong();
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
        protocol = spy(new BinaryProtocol(link, stream, null, clock::get));
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
        doReturn(crcResponse(IoHelper.getCrc32(new byte[]{1, 2, 3, 4})))
            .when(protocol).executeCommand(eq(Integration.TS_CRC_CHECK_COMMAND), any(byte[].class), anyString());

        assertTrue(readImage());
        assertArrayEquals(new byte[]{1, 2, 3, 4}, protocol.getControllerConfiguration().getContent());
        assertEquals(Arrays.asList(ConnectionStatusValue.LOADING, ConnectionStatusValue.CONNECTED), transitions);
    }

    @Test
    void issue10138TimedOutPartialImageIsRejectedBeforeSaveOrPublication() throws Exception {
        final int imageSize = 16816;
        final int blockingFactor = 1024;
        when(protocol.getIniFile().getBlockingFactor()).thenReturn(blockingFactor);
        List<byte[]> requests = new ArrayList<>();
        AtomicInteger readCount = new AtomicInteger();
        doAnswer(invocation -> {
            byte[] request = ((byte[]) invocation.getArgument(1)).clone();
            requests.add(request);
            switch (readCount.incrementAndGet()) {
                case 1:
                    return responseOfSize(blockingFactor, 1);
                case 2:
                    return responseOfSize(blockingFactor, 2);
                case 3:
                    return responseOfSize(blockingFactor, 3);
                case 4:
                    // The request at offset 3072 times out.
                    clock.set(60001);
                    return null;
                default:
                    throw new AssertionError("unexpected extra image read");
            }
        }).when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());
        doNothing().when(protocol).saveConfigurationImage(any(), any());

        // #10138: reject a timed-out partial image before save/publication.
        assertFalse(protocol.readImage(new BinaryProtocol.Arguments(true, true),
            new ConfigurationImageMetaVersion0_0(imageSize, "test")));
        verify(protocol, never()).saveConfigurationImage(any(), any());
        assertEquals(4, requests.size());
        assertEquals(3072, unsignedShort(requests.get(3), 2));
        assertEquals(blockingFactor, unsignedShort(requests.get(3), 4));
        assertNull(protocol.getControllerConfiguration());
        assertEquals(Arrays.asList(ConnectionStatusValue.LOADING, ConnectionStatusValue.NOT_CONNECTED), transitions);
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
    }

    @Test
    void issue10138CompleteImageWithWrongWholeImageCrcIsRejected() {
        final int imageSize = 16816;
        final int blockingFactor = 1024;
        when(protocol.getIniFile().getBlockingFactor()).thenReturn(blockingFactor);
        AtomicInteger requests = new AtomicInteger();
        doAnswer(invocation -> {
            assertTrue(requests.incrementAndGet() <= 17, "unexpected extra image read");
            byte[] request = invocation.getArgument(1);
            int size = unsignedShort(request, 4);
            return responseOfSize(size, 0x5a);
        }).when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());
        int explicitlyWrongCrc = 0;
        doReturn(new byte[]{Integration.TS_RESPONSE_OK, 0, 0, 0, 0})
            .when(protocol).executeCommand(eq(Integration.TS_CRC_CHECK_COMMAND), any(byte[].class), anyString());

        // #10138: reject a complete image with a wrong whole-image CRC.
        assertFalse(protocol.readImage(new BinaryProtocol.Arguments(true, false),
            new ConfigurationImageMetaVersion0_0(imageSize, "test")));
        assertNotEquals(explicitlyWrongCrc, IoHelper.getCrc32(responseContent(imageSize, 0x5a)));
        verify(protocol).executeCommand(eq(Integration.TS_CRC_CHECK_COMMAND), any(byte[].class), anyString());
        assertNull(protocol.getControllerConfiguration());
        assertEquals(Arrays.asList(ConnectionStatusValue.LOADING, ConnectionStatusValue.NOT_CONNECTED), transitions);
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
    }

    @Test
    void issue10138CompleteImageWithValidWholeImageCrcIsAccepted() {
        final int imageSize = 16816;
        final int blockingFactor = 1024;
        when(protocol.getIniFile().getBlockingFactor()).thenReturn(blockingFactor);
        doAnswer(invocation -> responseForRead(invocation, imageSize, blockingFactor, 0x5a))
            .when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());
        doReturn(crcResponse(IoHelper.getCrc32(responseContent(imageSize, 0x5a))))
            .when(protocol).executeCommand(eq(Integration.TS_CRC_CHECK_COMMAND), any(byte[].class), anyString());

        assertTrue(protocol.readImage(new BinaryProtocol.Arguments(true, false),
            new ConfigurationImageMetaVersion0_0(imageSize, "test")));
        assertEquals(imageSize, protocol.getControllerConfiguration().getSize());
        assertEquals(ConnectionStatusValue.CONNECTED, status.getValue());
    }

    @Test
    void issue10138UnavailableWholeImageCrcRejectsCompleteImage() throws Exception {
        final int imageSize = 16816;
        final int blockingFactor = 1024;
        when(protocol.getIniFile().getBlockingFactor()).thenReturn(blockingFactor);
        doAnswer(invocation -> responseForRead(invocation, imageSize, blockingFactor, 0x5a))
            .when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());
        doReturn(null).when(protocol)
            .executeCommand(eq(Integration.TS_CRC_CHECK_COMMAND), any(byte[].class), anyString());

        assertFalse(protocol.readImage(new BinaryProtocol.Arguments(true, true),
            new ConfigurationImageMetaVersion0_0(imageSize, "test")));
        verify(protocol, never()).saveConfigurationImage(any(), any());
        assertNull(protocol.getControllerConfiguration());
        assertEquals(Arrays.asList(ConnectionStatusValue.LOADING, ConnectionStatusValue.NOT_CONNECTED), transitions);
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
    }

    @Test
    void issue10138NullPcanImageChunkClosesTransportWithoutRetry() {
        PCanIoStream pcan = mock(PCanIoStream.class);
        AtomicBoolean closed = new AtomicBoolean();
        AtomicInteger requests = new AtomicInteger();
        doAnswer(invocation -> closed.get()).when(pcan).isClosed();
        doAnswer(invocation -> {
            closed.set(true);
            return null;
        }).when(pcan).close();

        BinaryProtocol pcanProtocol = spy(new BinaryProtocol(link, pcan, null, clock::get));
        pcanProtocol.setIniFileForUnitTest(protocol.getIniFile());
        doAnswer(invocation -> {
            requests.incrementAndGet();
            clock.set(60001);
            return null;
        }).when(pcanProtocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());

        // #10138: release the partial ISO-TP transport before retrying.
        ConfigurationImageWithMeta result = pcanProtocol.readFullImageFromController(
            new ConfigurationImageMetaVersion0_0(4, "test"));

        assertTrue(result.isEmpty());
        assertEquals(1, requests.get());
        assertTrue(closed.get());
        verify(pcan).close();
    }

    @Test
    void allOnesWholeImageCrcIsValidRatherThanAnErrorSentinel() {
        byte[] image = {(byte) 0xff, (byte) 0xff, (byte) 0xff, (byte) 0xff};
        assertEquals(-1, IoHelper.getCrc32(image));
        doReturn(new byte[]{Integration.TS_RESPONSE_OK, (byte) 0xff, (byte) 0xff})
            .when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());
        doReturn(crcResponse(-1)).when(protocol)
            .executeCommand(eq(Integration.TS_CRC_CHECK_COMMAND), any(byte[].class), anyString());

        assertTrue(readImage());
        assertArrayEquals(image, protocol.getControllerConfiguration().getContent());
    }

    @Test
    void closeDuringFinalCrcDoesNotSaveOrPublishImage() throws Exception {
        byte[] image = {1, 2, 3, 4};
        doReturn(new byte[]{Integration.TS_RESPONSE_OK, 1, 2}, new byte[]{Integration.TS_RESPONSE_OK, 3, 4})
            .when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());
        doAnswer(invocation -> {
            stream.close();
            return crcResponse(IoHelper.getCrc32(image));
        }).when(protocol).executeCommand(eq(Integration.TS_CRC_CHECK_COMMAND), any(byte[].class), anyString());

        assertFalse(protocol.readImage(new BinaryProtocol.Arguments(true, true),
            new ConfigurationImageMetaVersion0_0(image.length, "test")));
        verify(protocol, never()).saveConfigurationImage(any(), any());
        assertNull(protocol.getControllerConfiguration());
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
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
    void issue10282LateFinalImageResponseDoesNotMarkClosedStreamConnected() {
        doReturn(new byte[]{Integration.TS_RESPONSE_OK, 1, 2}).doAnswer(invocation -> {
            assertEquals(ConnectionStatusValue.LOADING, status.getValue());
            stream.close();
            assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
            return new byte[]{Integration.TS_RESPONSE_OK, 3, 4};
        }).when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());

        // Even a valid final chunk must not complete a connection that has closed.
        assertFalse(readImage());
        assertTrue(stream.isClosed());
        assertNull(protocol.getControllerConfiguration());
        assertNull(protocol.getBinaryProtocolState().getCachedImage());
        assertEquals(Arrays.asList(ConnectionStatusValue.LOADING, ConnectionStatusValue.NOT_CONNECTED), transitions);
        stream.close();
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
    }

    @Test
    void closeBetweenImageReadAndCompletionRejectsImage() {
        doAnswer(invocation -> {
            ConfigurationImageWithMeta image = new ConfigurationImageWithMeta(invocation.getArgument(0));
            stream.close();
            return image;
        }).when(protocol).readFullImageFromController(any(ConfigurationImageMeta.class));

        assertFalse(readImage());
        assertNull(protocol.getControllerConfiguration());
        assertNull(protocol.getBinaryProtocolState().getCachedImage());
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
    }

    @Test
    void closedStreamCannotCompleteConnectionWithoutImage() {
        stream.close();

        assertFalse(protocol.readImage(new BinaryProtocol.Arguments(false, false),
            new ConfigurationImageMetaVersion0_0(4, "test")));
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
    }

    @Test
    void failedOldImageReadDoesNotDisconnectReplacementSession() {
        doAnswer(invocation -> {
            stream.close();
            // Model a replacement session connecting while the old read unwinds.
            status.setValue(ConnectionStatusValue.CONNECTED);
            return null;
        }).when(protocol).executeCommand(eq(Integration.TS_READ_COMMAND), any(byte[].class), anyString());

        assertFalse(readImage());
        assertNull(protocol.getControllerConfiguration());
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
    void issue10282LateOutputResponseDoesNotResurrectLoadingAfterUnplug() {
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

        assertFalse(poll());
        assertTrue(stream.isClosed());
        assertEquals(Collections.singletonList(ConnectionStatusValue.NOT_CONNECTED), transitions);
        assertFalse(status.isConnected());
        assertNull(SensorCentral.getInstance().getCurrentSnapshot());
        assertTrue(Double.isNaN(SensorCentral.getInstance().getValue(SECONDS)));
        assertFalse(poll());
        stream.close();
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
    }

    @Test
    void closeDuringConfigErrorRequestDoesNotPublishEarlierOutputs() throws Exception {
        IniFileModel ini = protocol.getIniFile();
        when(ini.getMetaInfo().getOchBlockSize()).thenReturn(4);
        when(ini.getBlockingFactor()).thenReturn(4);
        ScalarIniField error = new ScalarIniField(BinaryProtocol.CONFIG_ERROR_CHANNEL,
            2, "", FieldType.UINT16, 1, "0", 0);
        Map<String, IniField> channels = new LinkedHashMap<>();
        channels.put(SECONDS, ini.getOutputChannel(SECONDS));
        channels.put(BinaryProtocol.CONFIG_ERROR_CHANNEL, error);
        when(ini.getAllOutputChannels()).thenReturn(channels);
        when(ini.getOutputChannel(BinaryProtocol.CONFIG_ERROR_CHANNEL)).thenReturn(error);
        doReturn(new byte[]{Integration.TS_RESPONSE_OK, 42, 0, 1, 0}).when(protocol)
            .executeCommand(eq(Integration.TS_OUTPUT_COMMAND), any(byte[].class), anyString());
        doAnswer(invocation -> {
            stream.close();
            return new byte[]{Integration.TS_RESPONSE_OK, 'E'};
        }).when(protocol).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
        status.setValue(ConnectionStatusValue.CONNECTED);

        assertFalse(poll());
        verify(protocol).executeCommand(eq(Integration.TS_GET_CONFIG_ERROR), anyString());
        assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
        assertNull(SensorCentral.getInstance().getCurrentSnapshot());
        assertNull(protocol.getBinaryProtocolState().getCurrentOutputs());
    }

    @Test
    void closeCompletesWhileOutputCommandIsWaitingOnAnotherThread() throws Exception {
        CountDownLatch commandStarted = new CountDownLatch(1);
        CountDownLatch releaseResponse = new CountDownLatch(1);
        ExecutorService workers = Executors.newFixedThreadPool(2);
        doAnswer(invocation -> {
            commandStarted.countDown();
            assertTrue(releaseResponse.await(5, TimeUnit.SECONDS));
            return OUTPUTS;
        }).when(protocol).executeCommand(eq(Integration.TS_OUTPUT_COMMAND), any(byte[].class), anyString());
        status.setValue(ConnectionStatusValue.CONNECTED);

        try {
            Future<Boolean> pollResult = workers.submit(this::poll);
            assertTrue(commandStarted.await(5, TimeUnit.SECONDS));
            // The bounds detect deadlock; latches, not timing, order the interleaving.
            workers.submit(stream::close).get(5, TimeUnit.SECONDS);
            assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
            releaseResponse.countDown();
            assertFalse(pollResult.get(5, TimeUnit.SECONDS));
            assertEquals(ConnectionStatusValue.NOT_CONNECTED, status.getValue());
            assertNull(SensorCentral.getInstance().getCurrentSnapshot());
        } finally {
            releaseResponse.countDown();
            workers.shutdownNow();
            assertTrue(workers.awaitTermination(5, TimeUnit.SECONDS));
        }
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

    private static byte[] responseOfSize(int size, int value) {
        byte[] response = new byte[size + 1];
        response[0] = Integration.TS_RESPONSE_OK;
        java.util.Arrays.fill(response, 1, response.length, (byte) value);
        return response;
    }

    private static byte[] responseContent(int size, int value) {
        byte[] content = new byte[size];
        java.util.Arrays.fill(content, (byte) value);
        return content;
    }

    private static byte[] responseForRead(org.mockito.invocation.InvocationOnMock invocation,
                                          int imageSize, int blockingFactor, int value) {
        byte[] request = invocation.getArgument(1);
        int offset = unsignedShort(request, 2);
        int size = unsignedShort(request, 4);
        assertTrue(offset < imageSize);
        assertTrue(size <= blockingFactor);
        return responseOfSize(size, value);
    }

    private static byte[] crcResponse(int crc) {
        return new byte[]{
            Integration.TS_RESPONSE_OK,
            (byte) (crc >>> 24),
            (byte) (crc >>> 16),
            (byte) (crc >>> 8),
            (byte) crc
        };
    }

    private static int unsignedShort(byte[] bytes, int offset) {
        return ByteBuffer.wrap(bytes, offset, Short.BYTES)
            .order(ByteOrder.LITTLE_ENDIAN)
            .getShort() & 0xffff;
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
