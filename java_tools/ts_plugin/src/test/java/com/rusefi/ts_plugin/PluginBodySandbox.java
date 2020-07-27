package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.servers.ControllerParameterServer;
import com.opensr5.ini.IniFileModel;
import com.rusefi.TsTuneReader;
import com.rusefi.ui.util.FrameHelper;

import java.util.ArrayList;
import java.util.Objects;

import static org.mockito.Matchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Sandbox for plugin body
 *
 * @see PluginLauncherSandbox for plugin auto-update launcher
 */
public class PluginBodySandbox {

    private static final String PROJECT_NAME = "dev";

    public static void main(String[] args) {
        String iniFile = TsTuneReader.getProjectModeFileName(PROJECT_NAME);
        IniFileModel model = new IniFileModel().readIniFile(iniFile);
        Objects.requireNonNull(model, "model");
        java.util.List<String> fieldNamesList = new ArrayList<>(model.allIniFields.keySet());
        String[] parameterNames = fieldNamesList.toArray(new String[0]);


        ControllerParameterServer controllerParameterServer = mock(ControllerParameterServer.class);
        when(controllerParameterServer.getParameterNames(any())).thenReturn(parameterNames);

        ControllerAccess controllerAccess = mock(ControllerAccess.class);
        when(controllerAccess.getEcuConfigurationNames()).thenReturn(new String[]{PROJECT_NAME});
        when(controllerAccess.getControllerParameterServer()).thenReturn(controllerParameterServer);


        new FrameHelper().showFrame(new PluginEntry(() -> {
            return controllerAccess;
        }).getContent());
    }

}
