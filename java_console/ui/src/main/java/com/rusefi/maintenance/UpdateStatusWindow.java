package com.rusefi.maintenance;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.StatusWindow;

public class UpdateStatusWindow extends StatusWindow implements UpdateOperationCallbacks {
    public UpdateStatusWindow(String title) {
        showFrame(title);
    }

    @Override
    public void log(final String message, final boolean breakLineOnTextArea, boolean sendToLogger) {
        getContent().appendStatus(message, breakLineOnTextArea, sendToLogger);
    }

    @Override
    public void done() {
        getContent().setSuccessState();
    }

    @Override
    public void error() {
        getContent().setErrorState();
    }
}
