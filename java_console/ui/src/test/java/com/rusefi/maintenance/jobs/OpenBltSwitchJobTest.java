package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import org.mockito.InOrder;

/**
 * Tests the switch-to-OpenBLT choreography, in particular the renumber-follow invariant: after
 * sending the reboot command over the live connection the job must release the serial port with
 * {@link LinkManager#close()} and NOT {@link LinkManager#disconnect()} — disconnect() sets
 * isDisconnectedByUser, which permanently blocks the watchdog restart and the ConsoleUI
 * follow-renumbered-port listener, so the session would never come back. Driven through the
 * injected {@link OpenBltSwitchJob.Rebooter}, so no hardware. [tag:better_ux_for_flashing]
 */
public class OpenBltSwitchJobTest {
    private static final String PORT = "COM9";

    private final OpenBltSwitchJob.Rebooter rebooter = mock(OpenBltSwitchJob.Rebooter.class);
    private final UpdateOperationCallbacks callbacks = UpdateOperationCallbacks.DUMMY;
    private final AtomicInteger jobFinishedCount = new AtomicInteger();

    private OpenBltSwitchJob newJob(final LinkManager linkManager) {
        return new OpenBltSwitchJob(new PortResult(PORT, SerialPortType.EcuWithOpenblt), null, linkManager, rebooter);
    }

    @Test
    public void liveSwitchSendsRebootOverConnectionThenClosesLinkWithoutDisconnect() {
        final LinkManager linkManager = mock(LinkManager.class);
        final BinaryProtocol bp = mock(BinaryProtocol.class);
        when(linkManager.getBinaryProtocol()).thenReturn(bp);

        newJob(linkManager).doJob(callbacks, jobFinishedCount::incrementAndGet);

        // reboot goes out over the still-open connection, and only then is the port released
        final InOrder order = inOrder(rebooter, linkManager);
        order.verify(rebooter).rebootToOpenblt(null, bp, callbacks);
        order.verify(linkManager).close();
        // disconnect() would set isDisconnectedByUser and kill any chance of auto-reconnecting
        // to the (possibly renumbered) port the board comes back on
        verify(linkManager, never()).disconnect();
        assertEquals(1, jobFinishedCount.get());
    }

    @Test
    public void liveSwitchWithoutBinaryProtocolStillReleasesThePort() {
        final LinkManager linkManager = mock(LinkManager.class);
        when(linkManager.getBinaryProtocol()).thenReturn(null);

        newJob(linkManager).doJob(callbacks, jobFinishedCount::incrementAndGet);

        // the production rebooter reports "Not connected?" on a null protocol; the port must be
        // released either way so the scanner can classify whatever the board does next
        verify(rebooter).rebootToOpenblt(null, (BinaryProtocol) null, callbacks);
        verify(linkManager).close();
        verify(linkManager, never()).disconnect();
        assertEquals(1, jobFinishedCount.get());
    }

    @Test
    public void offlineSwitchRebootsTheSelectedPortDirectly() {
        newJob(null).doJob(callbacks, jobFinishedCount::incrementAndGet);

        verify(rebooter).rebootToOpenblt(null, PORT, callbacks);
        assertEquals(1, jobFinishedCount.get());
    }
}
