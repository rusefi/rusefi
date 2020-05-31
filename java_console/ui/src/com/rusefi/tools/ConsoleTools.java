package com.rusefi.tools;

import com.fathzer.soft.javaluator.DoubleEvaluator;
import com.opensr5.ConfigurationImage;
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
import com.rusefi.tools.online.Online;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.OnlineTab;
import org.jetbrains.annotations.Nullable;

import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Arrays;
import java.util.Map;
import java.util.TreeMap;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class ConsoleTools {
    public static final String SET_AUTH_TOKEN = "set_auth_token";
    private static Map<String, ConsoleTool> TOOLS = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    private static Map<String, String> toolsHelp = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    static {
        TOOLSput("help", args -> printTools(), "Print this help.");
        TOOLSput("headless", ConsoleTools::runHeadless, "Connect to rusEFI controller and start saving logs.");

        TOOLSput("ptrace_enums", ConsoleTools::runPerfTraceTool, "NOT A USER TOOL. Development tool to process pefrormance trace enums");
        TOOLSput("firing_order", ConsoleTools::runFiringOrderTool, "NOT A USER TOOL. Development tool relating to adding new firing order into rusEFI firmware.");
        TOOLSput("functional_test", ConsoleTools::runFunctionalTest, "NOT A USER TOOL. Development tool related to functional testing");
        TOOLSput("convert_binary_configuration_to_xml", ConsoleTools::convertBinaryToXml, "NOT A USER TOOL. Development tool to convert binary configuration into XML form.");

        TOOLSput("compile_fsio_line", ConsoleTools::invokeCompileExpressionTool, "Convert a line to RPN form.");
        TOOLSput("compile_fsio_file", ConsoleTools::runCompileTool, "Convert all lines from a file to RPN form.");

        TOOLSput("print_auth_token", args -> printAuthToken(), "Print current rusEFI Online authentication token.");
        TOOLSput(SET_AUTH_TOKEN, ConsoleTools::setAuthToken, "Set rusEFI authentication token.");

        TOOLSput("reboot_ecu", args -> sendCommand(Fields.CMD_REBOOT), "Sends a command to reboot rusEFI controller.");
        TOOLSput(Fields.CMD_REBOOT_DFU, args -> sendCommand(Fields.CMD_REBOOT_DFU), "Sends a command to switch rusEFI controller into DFU mode.");
    }

    private static void TOOLSput(String command, ConsoleTool callback, String help) {
        TOOLS.put(command, callback);
        toolsHelp.put(command, help);
    }

    public static void printTools() {
        for (String key : TOOLS.keySet()) {
            System.out.println("Tool available: " + key);
            String help = toolsHelp.get(key);
            if (help != null) {
                System.out.println("\t" + help);
                System.out.println("\n");
            }
        }
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

    private static void setAuthToken(String[] args) {
        String newToken = args[1];
        getConfig().getRoot().setProperty(OnlineTab.AUTH_TOKEN, newToken);
    }

    private static void printAuthToken() {
        String authToken = getConfig().getRoot().getProperty(OnlineTab.AUTH_TOKEN);
        if (authToken.trim().isEmpty()) {
            System.out.println("Auth token not defined. Please use " + SET_AUTH_TOKEN + " command");
            System.out.println("\tPlease see https://github.com/rusefi/rusefi/wiki/Online");
            return;
        }
        System.out.println("Auth token: " + authToken);
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

        if (args.length < 2) {
            System.err.println("Binary file input expected");
            System.exit(-1);
        }
        String inputBinaryFileName = args[1];
        ConfigurationImage image = ConfigurationImageFile.readFromFile(inputBinaryFileName);
        System.out.println("Got " + image.getSize() + " of configuration from " + inputBinaryFileName);

        Msq tune = Msq.toMsq(image);
        tune.writeXmlFile(Msq.outputXmlFileName);
        Online.upload(new File(Msq.outputXmlFileName), "x");
    }

    public static long classBuildTimeMillis() throws URISyntaxException, IllegalStateException, IllegalArgumentException {
        Class<?> clazz = ConsoleTools.class;
        URL resource = clazz.getResource(clazz.getSimpleName() + ".class");
        if (resource == null) {
            throw new IllegalStateException("Failed to find class file for class: " +
                    clazz.getName());
        }

        if (resource.getProtocol().equals("file")) {
            return new File(resource.toURI()).lastModified();
        } else if (resource.getProtocol().equals("jar")) {
            String path = resource.getPath();
            return new File(path.substring(5, path.indexOf("!"))).lastModified();
        } else {
            throw new IllegalArgumentException("Unhandled url protocol: " +
                    resource.getProtocol() + " for class: " +
                    clazz.getName() + " resource: " + resource.toString());
        }
    }

    interface ConsoleTool {
        void runTool(String args[]) throws Exception;
    }
}
