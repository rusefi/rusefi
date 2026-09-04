package com.rusefi.maintenance.jobs;

import com.rusefi.PortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class OpenBltCanJob extends AsyncJobWithContext<ParentComponentContext> {
    private final PortScanner scanner;

    public OpenBltCanJob(final JComponent parent, PortScanner scanner) {
        super("OpenBLT via CAN", new ParentComponentContext(parent));
        this.scanner = scanner;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(() -> ProgramSelector.flashOpenBltCan(context.getParent(), callbacks, scanner), onJobFinished);
    }
}
