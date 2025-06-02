package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.basic.UnitLabelPrinter;

import javax.swing.*;

public class PrintUnitLabelJob extends  AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    private final ConnectivityContext connectivityContext;

    public PrintUnitLabelJob(ConnectivityContext connectivityContext, final PortResult port, final JComponent parent) {
        super("Print unit label", new SerialPortWithParentComponentJobContext(port, parent));
        this.connectivityContext = connectivityContext;
    }

    @Override
    public void doJob(UpdateOperationCallbacks callbacks, Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (UnitLabelPrinter.INSTANCE.printUnitLabel(context.getParent(), context.getPort(), callbacks, connectivityContext)) {
                    callbacks.done();
                } else {
                    callbacks.error();
                }
            },
            onJobFinished
        );
    }
}
