package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.ControllerException;
import com.efiAnalytics.plugin.ecu.servers.ControllerParameterServer;
import com.opensr5.ini.IniFileModel;
import com.rusefi.TsTuneReader;
import com.rusefi.ui.util.FrameHelper;

import javax.swing.*;
import java.util.ArrayList;
import java.util.Objects;

import static com.rusefi.MockitoTestHelper.NEGATIVE_ANSWER;
import static org.mockito.Matchers.any;
import static org.mockito.Mockito.*;

/**
 * Sandbox for plugin body
 *
 * @see PluginLauncherSandbox for plugin auto-update launcher
 */
public class PluginBodySandbox {

    private static final String PROJECT_NAME = "dev";

    public static void main(String[] args) throws ControllerException {
        String iniFile = TsTuneReader.getProjectModeFileName(PROJECT_NAME);
        IniFileModel model = new IniFileModel().readIniFile(iniFile);
        Objects.requireNonNull(model, "model");
        java.util.List<String> fieldNamesList = new ArrayList<>(model.allIniFields.keySet());
        String[] parameterNames = fieldNamesList.toArray(new String[0]);

        ControllerParameterServer controllerParameterServer = mock(ControllerParameterServer.class, NEGATIVE_ANSWER);
        doReturn(parameterNames).when(controllerParameterServer).getParameterNames(any());
        doNothing().when(controllerParameterServer).subscribe(any(), any(), any());

        ControllerAccess controllerAccess = mock(ControllerAccess.class, NEGATIVE_ANSWER);
        doReturn(new String[]{PROJECT_NAME}).when(controllerAccess).getEcuConfigurationNames();
        doReturn(controllerParameterServer).when(controllerAccess).getControllerParameterServer();

        FrameHelper frameHelper = new FrameHelper();
        frameHelper.getFrame().setDefaultCloseOperation(JDialog.EXIT_ON_CLOSE);
        frameHelper.showFrame(new PluginEntry(() -> controllerAccess).getContent());
    }

}
