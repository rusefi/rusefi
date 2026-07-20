package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;
import org.mockito.InOrder;

import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

public class DfuSwitchJobTest {
    @Test
    public void connectedSwitchUsesLiveConnectionThenReleasesThePort() {
        final LinkManager linkManager = mock(LinkManager.class);
        final BinaryProtocol bp = mock(BinaryProtocol.class);
        when(linkManager.getBinaryProtocol()).thenReturn(bp);
        final DfuSwitchJob.Rebooter rebooter = mock(DfuSwitchJob.Rebooter.class);
        final UpdateOperationCallbacks callbacks = UpdateOperationCallbacks.DUMMY;
        final AtomicInteger jobFinishedCount = new AtomicInteger();

        new DfuSwitchJob(new PortResult("COM6", SerialPortType.Ecu), null, linkManager, rebooter)
            .doJob(callbacks, jobFinishedCount::incrementAndGet);

        // Reopening COM6 would fail because LinkManager already owns it. Send the command through
        // that live connection, then close it so the board can re-enumerate in DFU mode.
        final InOrder order = inOrder(rebooter, linkManager);
        order.verify(rebooter).rebootToDfu(null, bp, callbacks);
        order.verify(linkManager).close();
        verify(linkManager, never()).disconnect();
        assertEquals(1, jobFinishedCount.get());
    }
}
