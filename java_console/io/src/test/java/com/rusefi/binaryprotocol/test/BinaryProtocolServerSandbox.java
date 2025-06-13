package com.rusefi.binaryprotocol.test;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModeSingleton;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.BinaryProtocolServer;

/**
 * Starts a fake device with TCP connector at {@link BinaryProtocolServer#DEFAULT_PROXY_PORT} port
 * @see TcpServerSandbox what's the difference?
 * BinaryProtocolServer#DEFAULT_PROXY_PORT
 */
class BinaryProtocolServerSandbox {
/*
    public static void main(String[] args) {
//        File f = new File(BinaryProtocolServer.TEST_FILE);
//        if (!f.exists())
//            throw new IllegalStateException("File not found: " + BinaryProtocolServer.TEST_FILE);

        IniFileModel model = IniFileModeSingleton.getInstance();

        BinaryProtocolState state = new BinaryProtocolState();
        ConfigurationImage controller = new ConfigurationImage(new byte[model.getMetaInfo().getPageSize(0)]);
//        ScalarIniField iniField = s
//        iniField.setValue(controller, new Constant(iniField.getName(), "", "1", iniField.getDigits()));
        state.setConfigurationImage(controller);
        state.setCurrentOutputs(new byte[1 + VariableRegistryValues.TS_TOTAL_OUTPUT_SIZE]);

        LinkManager linkManager = new LinkManager();
        linkManager.setConnector(LinkConnector.getDetachedConnector(state));
        new BinaryProtocolServer().start(linkManager);
    }
*/
}
