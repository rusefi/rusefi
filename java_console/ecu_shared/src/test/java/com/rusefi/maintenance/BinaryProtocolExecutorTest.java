package com.rusefi.maintenance;

import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;

import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class BinaryProtocolExecutorTest {
    @Test
    void recoveryConnectDoesNotReadThePotentiallyLockedImage() {
        // [tag:post_flash_layout_reset] Preparation must run before the first image download.
        LinkManager link = mock(LinkManager.class);
        LinkConnector connector = mock(LinkConnector.class);
        when(link.getConnector()).thenReturn(connector);
        ConnectionStatusLogic.Listener listener = mock(ConnectionStatusLogic.Listener.class);

        BinaryProtocolExecutor.connect(link, "test", listener, false);

        verify(link).start("test", listener);
        verify(link, never()).startAndConnect(anyString(), any());
        ArgumentCaptor<BinaryProtocol.Arguments> arguments = ArgumentCaptor.forClass(BinaryProtocol.Arguments.class);
        verify(connector).connectAndReadConfiguration(arguments.capture(), same(listener));
        try (LinkManager protocolLink = new LinkManager().setNotifyGlobalStatusOnClose(false)) {
            BinaryProtocol protocol = spy(new BinaryProtocol(protocolLink, mock(IoStream.class)));
            assertTrue(protocol.readImage(arguments.getValue(), new ConfigurationImageMetaVersion0_0(6, "test")));
            verify(protocol, never()).readFullImageFromController(any(ConfigurationImageMeta.class));
        }
    }

    @Test
    void normalActionsKeepTheirImageBasedConnection() {
        LinkManager link = mock(LinkManager.class);
        ConnectionStatusLogic.Listener listener = mock(ConnectionStatusLogic.Listener.class);

        BinaryProtocolExecutor.connect(link, "test", listener, true);

        verify(link).startAndConnect("test", listener);
        verifyNoMoreInteractions(link);
    }
}
