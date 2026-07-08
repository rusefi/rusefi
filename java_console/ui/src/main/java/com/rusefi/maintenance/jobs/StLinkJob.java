package com.rusefi.maintenance.jobs;

import com.rusefi.core.FindFileHelper;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.StLinkFlasher;

import javax.swing.*;

public class StLinkJob extends AsyncJobWithContext<ParentComponentContext> {
    private final ConnectedEcuTarget connectedEcuTarget;

    public StLinkJob(final JComponent parent, final ConnectedEcuTarget connectedEcuTarget) {
        super("", new ParentComponentContext(parent));
        this.connectedEcuTarget = connectedEcuTarget;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        // todo: add ST-LINK no-assert mode? or not?
        StLinkFlasher.doUpdateFirmware(FindFileHelper.findFirmwareFile(), callbacks, onJobFinished, connectedEcuTarget);
    }
}
