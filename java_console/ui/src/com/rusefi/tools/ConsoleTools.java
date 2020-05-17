package com.rusefi.tools;

import com.fathzer.soft.javaluator.DoubleEvaluator;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.*;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.SerialIoStreamJSerialComm;
import com.rusefi.maintenance.ExecHelper;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.CurveConstant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.xml.XmlUtil;
import org.jetbrains.annotations.Nullable;

import javax.xml.bind.JAXBException;
import java.io.IOException;
import java.util.Arrays;
import java.util.Map;
import java.util.Objects;
import java.util.TreeMap;

public class ConsoleTools {
    private static Map<String, ConsoleTool> TOOLS = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    static {
        TOOLS.put("help", args -> printTools());
        TOOLS.put("headless", ConsoleTools::runHeadless);
        TOOLS.put("compile", ConsoleTools::invokeCompileExpressionTool);
        TOOLS.put("ptrace_enums", ConsoleTools::runPerfTraceTool);
        TOOLS.put("save_binary_configuration", ConsoleTools::saveBinaryConfig);
        TOOLS.put("functional_test", ConsoleTools::runFunctionalTest);
        TOOLS.put("compile_fsio_file", ConsoleTools::runCompileTool);
        TOOLS.put("firing_order", ConsoleTools::runFiringOrderTool);
        TOOLS.put("fun_convert", ConsoleTools::convertBinaryToXml);
        TOOLS.put("reboot_ecu", args -> sendCommand(Fields.CMD_REBOOT));
        TOOLS.put(Fields.CMD_REBOOT_DFU, args -> sendCommand(Fields.CMD_REBOOT_DFU));
    }

    public static void printTools() {
        for (String key : TOOLS.keySet()) {
            System.out.println("Tool available: " + key);
        }
    }

    private static void saveBinaryConfig(String[] args) throws IOException {
        if (args.length < 2) {
            System.out.println("Please specify output file name for binary configuration");
            System.exit(-1);
        }
        String fileName = args[1];

        String autoDetectedPort = autoDetectPort();
        if (autoDetectedPort == null)
            return;
        LinkManager.startAndConnect(autoDetectedPort, new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                BinaryProtocol binaryProtocol = LinkManager.connector.getBinaryProtocol();
                Objects.requireNonNull(binaryProtocol, "binaryProtocol");
                ConfigurationImage configurationImage = binaryProtocol.getControllerConfiguration();
                Objects.requireNonNull(configurationImage, "configurationImage");

                try {
                    ConfigurationImageFile.saveToFile(configurationImage, fileName);
                    System.exit(0);
                } catch (IOException e) {
                    System.out.println("While writing " + e);
                    System.exit(-1);
                }
            }

            @Override
            public void onConnectionFailed() {
                System.out.println("onConnectionFailed");
                System.exit(-1);
            }
        });
    }

    private static void sendCommand(String command) throws IOException {
        String autoDetectedPort = autoDetectPort();
        if (autoDetectedPort == null)
            return;
        IoStream stream = SerialIoStreamJSerialComm.openPort(autoDetectedPort);
        byte[] commandBytes = BinaryProtocol.getTextCommandBytes(command);
        stream.sendPacket(commandBytes, FileLog.LOGGER);
    }


    private static void runPerfTraceTool(String[] args) throws IOException {
        PerfTraceTool.readPerfTrace(args[1], args[2], args[3], args[4]);
    }

    private static void runFiringOrderTool(String[] args) throws IOException {
        FiringOrderTSLogic.invoke(args[1]);
    }

    private static void runCompileTool(String[] args) throws IOException {
        int returnCode = invokeCompileFileTool(args);
        System.exit(returnCode);
    }

    private static void runFunctionalTest(String[] args) throws InterruptedException {
        // passing port argument if it was specified
        String[] toolArgs = args.length == 1 ? new String[0] : new String[]{args[1]};
        RealHwTest.main(toolArgs);
    }

    private static void runHeadless(String[] args) {
        String onConnectedCallback = args.length > 1 ? args[1] : null;
        String onDisconnectedCallback = args.length > 2 ? args[2] : null;

        ConnectionStatusLogic.INSTANCE.addListener(new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                if (isConnected) {
                    invokeCallback(onConnectedCallback);
                } else {
                    invokeCallback(onDisconnectedCallback);
                }
            }
        });

        String autoDetectedPort = PortDetector.autoDetectSerial();
        if (autoDetectedPort == null) {
            System.err.println("rusEFI not detected");
            return;
        }
        LinkManager.startAndConnect(autoDetectedPort, new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                SensorLogger.init();
            }

            @Override
            public void onConnectionFailed() {

            }
        });
    }

    private static void invokeCallback(String callback) {
        if (callback == null)
            return;
        System.out.println("Invoking " + callback);
        ExecHelper.submitAction(new Runnable() {
            @Override
            public void run() {
                try {
                    Runtime.getRuntime().exec(callback);
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }
        }, "callback");
    }

    private static int invokeCompileFileTool(String[] args) throws IOException {
        /**
         * re-packaging array which contains input and output file names
         */
        return CompileTool.run(Arrays.asList(args).subList(1, args.length));
    }

    private static void invokeCompileExpressionTool(String[] args) {
        if (args.length != 2) {
            System.err.println("input expression parameter expected");
            System.exit(-1);
        }
        String expression = args[1];
        System.out.println(DoubleEvaluator.process(expression).getPosftfixExpression());
    }

    public static boolean runTool(String[] args) throws Exception {
        if (args == null || args.length == 0)
            return false;
        String toolName = args[0];
        ConsoleTool consoleTool = TOOLS.get(toolName);
        if (consoleTool != null) {
            consoleTool.runTool(args);
            return true;
        }
        return false;
    }

    @Nullable
    private static String autoDetectPort() {
        String autoDetectedPort = PortDetector.autoDetectPort(null);
        if (autoDetectedPort == null) {
            System.err.println("rusEFI not detected");
            return null;
        }
        return autoDetectedPort;
    }

    private static void convertBinaryToXml(String[] args) throws IOException, JAXBException {
        Msq tune = new Msq();

        if (args.length < 2) {
            System.err.println("Binary file input expected");
            System.exit(-1);
        }
        String fileName = args[1];
        ConfigurationImage image = ConfigurationImageFile.readFromFile(fileName);
        System.out.println("Got " + image.getSize() + " of configuration from " + fileName);

        IniFileModel ini = IniFileModel.getInstance(Launcher.INI_FILE_PATH);

        handle(tune, ini, "tpsMin");
//        handle(tune, ini, "injector_battLagCorrBins");


        XmlUtil.writeXml(tune, Msq.class, "a.xml");
    }

    private static void handle(Msq tune, IniFileModel ini, String key) {
        DialogModel.Field field = ini.getField(key);
        tune.getPage().constants.add(prepareConstant(field));
    }

    private static Constant prepareConstant(DialogModel.Field field) {
        return new CurveConstant(field.getKey(), null);
    }

    interface ConsoleTool {
        void runTool(String args[]) throws Exception;
    }
}
