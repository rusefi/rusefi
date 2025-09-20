package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.ControllerException;
import com.efiAnalytics.plugin.ecu.ControllerParameter;
import com.efiAnalytics.plugin.ecu.servers.ControllerParameterServer;
import com.efiAnalytics.plugin.ecu.servers.OutputChannelServer;
import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.TsTuneReader;
import com.rusefi.core.ui.FrameHelper;
import com.rusefi.ts_plugin.knock.KnockAnalyzerTab;

import javax.swing.*;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Objects;

import static com.rusefi.MockitoTestHelper.NEGATIVE_ANSWER;
import static org.mockito.Mockito.*;

/**
 * Sandbox for plugin body
 *
 * @see PluginLauncherSandbox for plugin auto-update launcher
 */
public class PluginBodySandbox {

    private static final String PROJECT_NAME = "dev";
    public static final ControllerParameter cylinderResult = new ControllerParameter() {
        @Override
        public double getScalarValue() {
            return 2;
        }
    };
    public static final ControllerParameter result2 = new ControllerParameter() {
        @Override
        public String getStringValue() {
            return "true";
        }
    };
    public static void main(String[] args) throws ControllerException, FileNotFoundException {
        String iniFile = TsTuneReader.getProjectModeFileName(PROJECT_NAME);
        IniFileModelImpl model = IniFileModelImpl.readIniFile(iniFile);
        Objects.requireNonNull(model, "model");
        java.util.List<String> fieldNamesList = new ArrayList<>(model.getAllIniFields().keySet());
        String[] parameterNames = fieldNamesList.toArray(new String[0]);

        OutputChannelServer outputChannelServer = mock(OutputChannelServer.class, NEGATIVE_ANSWER);
        doNothing().when(outputChannelServer).subscribe(anyString(), anyString(), any());
        doReturn(new String[]{}).when(outputChannelServer).getOutputChannels(anyString());

        ControllerParameterServer controllerParameterServer = mock(ControllerParameterServer.class, NEGATIVE_ANSWER);
        doReturn(parameterNames).when(controllerParameterServer).getParameterNames(any());
        doReturn(cylinderResult).when(controllerParameterServer).getControllerParameter(any(), eq(KnockAnalyzerTab.CYLINDERS_COUNT));
        doReturn(result2).when(controllerParameterServer).getControllerParameter(any(), eq(KnockAnalyzerTab.ENABLE_KNOCK_SPECTROGRAM));
        doNothing().when(controllerParameterServer).subscribe(any(), any(), any());

        ControllerAccess controllerAccess = mock(ControllerAccess.class, NEGATIVE_ANSWER);
        doReturn(new String[]{PROJECT_NAME}).when(controllerAccess).getEcuConfigurationNames();
        doReturn(controllerParameterServer).when(controllerAccess).getControllerParameterServer();
        doReturn(outputChannelServer).when(controllerAccess).getOutputChannelServer();


        SwingUtilities.invokeLater(() -> {
            FrameHelper frameHelper = new FrameHelper();
            frameHelper.getFrame().setDefaultCloseOperation(JDialog.EXIT_ON_CLOSE);
            frameHelper.showFrame(new PluginEntry(() -> controllerAccess).getContent());
        });
    }
}
