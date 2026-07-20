package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.io.UpdateOperationCallbacks;
import org.junit.jupiter.api.Test;

import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;

public class DfuSwitchJobTest {
    @Test
    public void switchIncorrectlyReopensSelectedPortEvenWhenConsoleMayAlreadyOwnIt() {
        // A connected Console already owns this port through LinkManager. Opening it again prevents
        // the signature check from reaching the ECU, producing the "rusEFI has not responded" error.
        // This intentionally asserts the bad behavior
        final DfuSwitchJob.Rebooter rebooter = mock(DfuSwitchJob.Rebooter.class);
        final UpdateOperationCallbacks callbacks = UpdateOperationCallbacks.DUMMY;
        final AtomicInteger jobFinishedCount = new AtomicInteger();

        new DfuSwitchJob(new PortResult("COM6", SerialPortType.Ecu), null, rebooter)
            .doJob(callbacks, jobFinishedCount::incrementAndGet);

        verify(rebooter).rebootToDfu(null, "COM6", callbacks);
        assertEquals(1, jobFinishedCount.get());
    }
}
