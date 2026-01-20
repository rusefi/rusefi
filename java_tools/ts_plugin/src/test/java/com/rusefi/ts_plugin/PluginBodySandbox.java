package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.efiAnalytics.plugin.ecu.ControllerException;
import com.efiAnalytics.plugin.ecu.ControllerParameter;
import com.efiAnalytics.plugin.ecu.servers.ControllerParameterServer;
import com.efiAnalytics.plugin.ecu.servers.OutputChannelServer;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
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
    private static final ControllerParameter MOCK_CYLINDER_COUNT = new ControllerParameter() {
        @Override
        public double getScalarValue() {
            return 2;
        }
    };
    private static final ControllerParameter MOCK_ENABLE_KNOCK_SPECTROGRAM = new ControllerParameter() {
        @Override
        public String getStringValue() {
            return "true";
        }
    };

    public static void main(String[] args) throws ControllerException, FileNotFoundException {
        String iniFile = TsTuneReader.getProjectModeFileName(PROJECT_NAME);
        IniFileModel model = IniFileReaderUtil.readIniFile(iniFile);
        Objects.requireNonNull(model, "model");
        java.util.List<String> fieldNamesList = new ArrayList<>(model.getAllIniFields().keySet());
        String[] parameterNames = fieldNamesList.toArray(new String[0]);

        ControllerAccess controllerAccess = getControllerAccess(parameterNames);

        SwingUtilities.invokeLater(() -> {
            FrameHelper frameHelper = new FrameHelper();
            frameHelper.getFrame().setDefaultCloseOperation(JDialog.EXIT_ON_CLOSE);
            frameHelper.showFrame(new TsPluginUiImpl(() -> controllerAccess).getContent());
        });
    }

    private static ControllerAccess getControllerAccess(String[] parameterNames) throws ControllerException {
        OutputChannelServer outputChannelServer = getOutputChannelServer();

        ControllerParameterServer controllerParameterServer = getControllerParameterServer(parameterNames);

        return getControllerAccess(controllerParameterServer, outputChannelServer);
    }

    private static ControllerAccess getControllerAccess(ControllerParameterServer controllerParameterServer, OutputChannelServer outputChannelServer) {
        ControllerAccess controllerAccess = mock(ControllerAccess.class, NEGATIVE_ANSWER);
        doReturn(new String[]{PROJECT_NAME}).when(controllerAccess).getEcuConfigurationNames();
        doReturn(controllerParameterServer).when(controllerAccess).getControllerParameterServer();
        doReturn(outputChannelServer).when(controllerAccess).getOutputChannelServer();
        return controllerAccess;
    }

    private static ControllerParameterServer getControllerParameterServer(String[] parameterNames) throws ControllerException {
        ControllerParameterServer controllerParameterServer = mock(ControllerParameterServer.class, NEGATIVE_ANSWER);
        doReturn(parameterNames).when(controllerParameterServer).getParameterNames(any());
        doReturn(MOCK_CYLINDER_COUNT).when(controllerParameterServer).getControllerParameter(any(), eq(KnockAnalyzerTab.CYLINDERS_COUNT));
        doReturn(MOCK_ENABLE_KNOCK_SPECTROGRAM).when(controllerParameterServer).getControllerParameter(any(), eq(KnockAnalyzerTab.ENABLE_KNOCK_SPECTROGRAM));
        doNothing().when(controllerParameterServer).subscribe(any(), any(), any());
        return controllerParameterServer;
    }

    private static OutputChannelServer getOutputChannelServer() throws ControllerException {
        OutputChannelServer outputChannelServer = mock(OutputChannelServer.class, NEGATIVE_ANSWER);
        doNothing().when(outputChannelServer).subscribe(anyString(), anyString(), any());
        doReturn(new String[]{}).when(outputChannelServer).getOutputChannels(anyString());
        return outputChannelServer;
    }
}
