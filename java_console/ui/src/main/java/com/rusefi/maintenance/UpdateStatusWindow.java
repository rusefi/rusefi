package com.rusefi.maintenance;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.StatusWindow;

public class UpdateStatusWindow extends StatusWindow implements UpdateOperationCallbacks {
    public UpdateStatusWindow(String title) {
        showFrame(title);
    }

    @Override
    public void log(String message) {
        append(message);
    }

    @Override
    public void done() {
        setSuccessState();
    }

    @Override
    public void error() {
        setErrorState();
    }
}
