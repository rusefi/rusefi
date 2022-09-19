package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.MockitoTestHelper;
import com.rusefi.ui.util.FrameHelper;

import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.mock;

public class LogUploadSelectorSandbox {
    public static void main(String[] args) {
        String projectName = "mre_f4";
        ControllerAccess controllerAccess = mock(ControllerAccess.class, MockitoTestHelper.NEGATIVE_ANSWER);
        doReturn(new String[]{projectName}).when(controllerAccess).getEcuConfigurationNames();

        new FrameHelper().showFrame(new LogUploadSelector(() -> controllerAccess).getContent());
    }

}
