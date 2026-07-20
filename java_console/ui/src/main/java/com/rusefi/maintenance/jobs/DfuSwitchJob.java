package com.rusefi.maintenance.jobs;

import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.BootloaderHelper;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;

public class DfuSwitchJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    interface Rebooter {
        void rebootToDfu(JComponent parent, BinaryProtocol bp, UpdateOperationCallbacks callbacks);
    }

    @Nullable
    private final LinkManager linkManager;
    private final Rebooter rebooter;

    public DfuSwitchJob(final PortResult port, final JComponent parent, final @Nullable LinkManager linkManager) {
        this(port, parent, linkManager, DfuSwitchJob::rebootToDfu);
    }

    DfuSwitchJob(final PortResult port, final JComponent parent, final @Nullable LinkManager linkManager,
                 final Rebooter rebooter) {
        super("DFU switch", new SerialPortWithParentComponentJobContext(port, parent));
        this.linkManager = linkManager;
        this.rebooter = rebooter;
    }

    private static void rebootToDfu(final JComponent parent, final BinaryProtocol bp,
                                    final UpdateOperationCallbacks callbacks) {
        if (bp == null) {
            callbacks.logLine("Not connected?");
            callbacks.error();
            return;
        }
        BootloaderHelper.sendBootloaderRebootCommand(parent, bp.signature, bp.getStream(), callbacks,
            Integration.CMD_REBOOT_DFU);
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (linkManager != null) {
                    rebooter.rebootToDfu(context.getParent(), linkManager.getBinaryProtocol(), callbacks);
                    linkManager.close();
                } else {
                    ProgramSelector.rebootToDfu(context.getParent(), context.getPort().port, callbacks);
                }
            },
            onJobFinished
        );
    }
}
