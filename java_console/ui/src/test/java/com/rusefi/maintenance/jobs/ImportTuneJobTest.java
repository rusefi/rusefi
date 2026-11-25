package com.rusefi.maintenance.jobs;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class ImportTuneJobTest {
    @Test
    void startsOnlyAfterExplicitImportAndBurnApproval() {
        Msq tune = new Msq();
        AtomicReference<Msq> started = new AtomicReference<>();

        ImportTuneJob.startIfApproved(false, tune, started::set);
        assertNull(started.get());

        ImportTuneJob.startIfApproved(true, tune, started::set);
        assertSame(tune, started.get());
    }

    @Test
    void establishedConnectionMustStillBeCurrentAndOpen() {
        BinaryProtocol established = mock(BinaryProtocol.class);
        BinaryProtocol replacement = mock(BinaryProtocol.class);
        LinkManager linkManager = mock(LinkManager.class);
        when(linkManager.getBinaryProtocol()).thenReturn(established);
        when(established.isClosed()).thenReturn(false);

        assertTrue(ImportTuneJob.isEstablishedConnectionCurrent(established, linkManager));

        when(linkManager.getBinaryProtocol()).thenReturn(replacement);
        assertFalse(ImportTuneJob.isEstablishedConnectionCurrent(established, linkManager));

        when(linkManager.getBinaryProtocol()).thenReturn(established);
        when(established.isClosed()).thenReturn(true);
        assertFalse(ImportTuneJob.isEstablishedConnectionCurrent(established, linkManager));
    }
}
