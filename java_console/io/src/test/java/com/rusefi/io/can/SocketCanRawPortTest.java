package com.rusefi.io.can;

import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;
import tel.schich.javacan.CanFilter;
import tel.schich.javacan.CanFrame;
import tel.schich.javacan.CanId;
import tel.schich.javacan.NetworkDevice;
import tel.schich.javacan.RawCanChannel;
import tel.schich.javacan.platform.linux.LinuxNativeOperationException;

import java.io.IOException;
import java.time.Duration;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static tel.schich.javacan.CanFrame.FD_NO_FLAGS;
import static tel.schich.javacan.CanSocketOptions.FILTER;
import static tel.schich.javacan.CanSocketOptions.RECV_OWN_MSGS;
import static tel.schich.javacan.CanSocketOptions.SO_RCVTIMEO;

class SocketCanRawPortTest {
    private final NetworkDevice device = mock(NetworkDevice.class);
    private final RawCanChannel channel = mock(RawCanChannel.class);

    @Test
    void openConfiguresBlockingSocketWithoutEchoAndFiltersExactExtendedId() throws IOException {
        SocketCanRawPort port = port();

        port.open(new CanAddress(0x1abcde, true));

        verify(channel).bind(device);
        verify(channel).configureBlocking(true);
        verify(channel).setOption(RECV_OWN_MSGS, false);
        ArgumentCaptor<CanFilter[]> filters = ArgumentCaptor.forClass(CanFilter[].class);
        verify(channel).setOption(org.mockito.ArgumentMatchers.eq(FILTER), filters.capture());
        assertEquals(1, filters.getValue().length);
        assertEquals(0x1abcde | CanId.EFF_FLAG, filters.getValue()[0].getId());
        assertEquals(CanFilter.EXACT & ~CanId.ERR_FLAG, filters.getValue()[0].getMask());
    }

    @Test
    void sendMapsExtendedAndStandardAddressesToTheirWireFormats() throws IOException {
        SocketCanRawPort port = port();
        port.open(new CanAddress(0x123, false));

        port.send(new ClassicCanFrame(new CanAddress(0x1abcde, true), new byte[]{1, 2}));
        port.send(new ClassicCanFrame(new CanAddress(0x123, false), new byte[]{3}));

        ArgumentCaptor<CanFrame> frames = ArgumentCaptor.forClass(CanFrame.class);
        verify(channel, org.mockito.Mockito.times(2)).write(frames.capture());
        assertTrue(frames.getAllValues().get(0).isExtended());
        assertEquals(0x1abcde, frames.getAllValues().get(0).getId());
        assertFalse(frames.getAllValues().get(1).isExtended());
        assertEquals(0x123, frames.getAllValues().get(1).getId());
    }

    @Test
    void socketCanSendOnLinuxTransmitQueueBackpressure() throws IOException {
        LinuxNativeOperationException queueFull =
            new LinuxNativeOperationException("write", 105, "No buffer space available");
        when(channel.write(any(CanFrame.class))).thenThrow(queueFull).thenReturn(channel);

        assertDoesNotThrow(() -> SocketCANHelper.send(0x710, new byte[]{1, 2}, channel));

        verify(channel, times(2)).write(any(CanFrame.class));
    }

    @Test
    void socketCanSendBoundsLinuxTransmitQueueBackpressure() throws IOException {
        LinuxNativeOperationException queueFull =
            new LinuxNativeOperationException("write", 105, "No buffer space available");
        when(channel.write(any(CanFrame.class))).thenThrow(queueFull);

        IllegalStateException exception = assertThrows(IllegalStateException.class,
            () -> SocketCANHelper.send(0x710, new byte[]{1, 2}, channel));

        assertEquals(queueFull, exception.getCause());
        verify(channel, times(SocketCANHelper.ENOBUFS_RETRY_COUNT + 1)).write(any(CanFrame.class));
    }

    @Test
    void socketCanSendDoesNotRetryOtherLinuxWriteFailures() throws IOException {
        LinuxNativeOperationException tryAgain = new LinuxNativeOperationException(
            "write", LinuxNativeOperationException.EAGAIN, "Try again");
        when(channel.write(any(CanFrame.class))).thenThrow(tryAgain);

        IllegalStateException exception = assertThrows(IllegalStateException.class,
            () -> SocketCANHelper.send(0x710, new byte[]{1, 2}, channel));

        assertEquals(tryAgain, exception.getCause());
        verify(channel).write(any(CanFrame.class));
    }

    @Test
    void receiveMapsTimeoutToEmptyAndConfiguresSocketTimeout() throws IOException {
        SocketCanRawPort port = port();
        port.open(new CanAddress(0x123, false));
        doThrow(new LinuxNativeOperationException("read", LinuxNativeOperationException.EAGAIN, "try again"))
            .when(channel).read();

        assertEquals(Optional.empty(), port.receive(125));
        verify(channel).setOption(SO_RCVTIMEO, Duration.ofMillis(125));
    }

    @Test
    void receiveConvertsAddressAndPayload() throws IOException {
        SocketCanRawPort port = port();
        port.open(new CanAddress(0x123, false));
        when(channel.read()).thenReturn(CanFrame.createExtended(0x1abcde, FD_NO_FLAGS, new byte[]{1, 2, 3}));

        ClassicCanFrame frame = port.receive(20).orElseThrow(AssertionError::new);

        assertEquals(0x1abcde, frame.getAddress().getId());
        assertTrue(frame.getAddress().isExtended());
        assertArrayEquals(new byte[]{1, 2, 3}, frame.getPayload());
    }

    @Test
    void openClosesPartiallyConfiguredChannel() throws IOException {
        doThrow(new IOException("configuration failed")).when(channel).configureBlocking(true);
        SocketCanRawPort port = port();

        assertThrows(IOException.class, () -> port.open(new CanAddress(0x123, false)));

        verify(channel).close();
    }

    @Test
    void closeIsIdempotent() throws IOException {
        SocketCanRawPort port = port();
        port.open(new CanAddress(0x123, false));

        port.close();
        port.close();

        verify(channel).close();
    }

    private SocketCanRawPort port() {
        return new SocketCanRawPort("vcan0", name -> device, () -> channel);
    }
}
