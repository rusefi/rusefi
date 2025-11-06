package com.rusefi.tools;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.*;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.*;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.can.PCanIoStream;
import com.rusefi.io.can.SocketCANIoStream;
import com.rusefi.io.tcp.BinaryProtocolProxy;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.ServerSocketReference;
import com.rusefi.maintenance.ExecHelper;
import com.rusefi.tools.online.Online;
import com.rusefi.ui.AuthTokenPanel;
import com.rusefi.ui.StatusConsumer;
import com.rusefi.io.UiLinkManagerHelper;
import com.rusefi.ui.basic.BasicStartupFrame;
import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.Map;
import java.util.TreeMap;
import java.util.function.Function;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
import static com.rusefi.binaryprotocol.IoHelper.getCrc32;

public class ConsoleTools {
    private static final Logging log = Logging.getLogging(ConsoleTools.class);
    public static final String SET_AUTH_TOKEN = "set_auth_token";
    public static final String RUS_EFI_NOT_DETECTED = "rusEFI not detected";
    private static final Map<String, ConsoleTool> TOOLS = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    private static final Map<String, String> toolsHelp = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    private static final StatusConsumer statusListener = new StatusConsumer() {
        final Logging log = getLogging(CANConnectorStartup.class);
        @Override
        public void logLine(final String message) {
            log.info(message);
        }
    };

    static {
        registerTool("help", args -> printTools(), "Print this help.");
        registerTool("headless", ConsoleTools::runHeadless, "Connect to rusEFI controller and start saving logs.");
        registerTool("basic-ui", BasicStartupFrame::runTool, "Basic UI");

        registerTool("functional_test", ConsoleTools::runFunctionalTest, "NOT A USER TOOL. Development tool related to functional testing");
//        registerTool("convert_binary_configuration_to_xml", ConsoleTools::convertBinaryToXml, "NOT A USER TOOL. Development tool to convert binary configuration into XML form.");

        registerTool("get_image_tune_crc", ConsoleTools::calcBinaryImageTuneCrc, "Calculate tune CRC for given binary tune");
//        registerTool("get_xml_tune_crc", ConsoleTools::calcXmlImageTuneCrc, "Calculate tune CRC for given XML tune");

//        registerTool("network_connector", strings -> NetworkConnectorStartup.start(), "Connect your rusEFI ECU to rusEFI Online");
//        registerTool("network_authenticator", strings -> LocalApplicationProxy.start(), "rusEFI Online Authenticator");
//        registerTool("elm327_connector", strings -> Elm327ConnectorStartup.start(), "Connect your rusEFI ECU using ELM327 CAN-bus adapter");
        registerTool("pcan_connector", strings -> {

            PCanIoStream stream = PCanIoStream.createStream();
            CANConnectorStartup.start(stream, statusListener);
        }, "Connect your rusEFI ECU using PCAN CAN-bus adapter");
        if (!FileLog.isWindows()) {
            registerTool("socketcan_connector", strings -> CANConnectorStartup.start(SocketCANIoStream.create(), statusListener), "Connect your rusEFI ECU using SocketCAN CAN-bus adapter");
        }
        registerTool("print_auth_token", args -> printAuthToken(), "Print current rusEFI Online authentication token.");
        registerTool("print_vehicle_token", args -> printVehicleToken(), "Prints vehicle access token.");
        registerTool(SET_AUTH_TOKEN, ConsoleTools::setAuthToken, "Set rusEFI Online authentication token.");
        registerTool("upload_tune", ConsoleTools::uploadTune, "Upload specified tune file to rusEFI Online using auth token from settings");

        registerTool("read_tune", args -> readTune(), "Read tune from controller");
        registerTool("get_performance_trace", args -> PerformanceTraceHelper.getPerformanceTune(), "DEV TOOL: Get performance trace from ECU");

        registerTool("version", ConsoleTools::version, "Only print version");

/*
    on the one hand we can do low level DFU programming but c'mon we are not planning to maintain it any day soon!
        registerTool("dfu", DfuTool::run, "Program specified file into ECU via DFU");
*/
        // java -jar rusefi_console.jar local_proxy
        registerTool("local_proxy", ConsoleTools::localProxy, "Detect rusEFI ECU and proxy serial <> TCP");

        registerTool("detect", ConsoleTools::detect, "Find attached rusEFI");
        registerTool("send_command", new ConsoleTool() {
            @Override
            public void runTool(String[] args) throws Exception {
                if (args.length < 1)
                    throw new IllegalStateException("argument expected");
                String command = CommandHelper.assembleCommand(args);
                log.info("Sending command [" + command + "]");
                IoStream stream = sendNonBlockingCommandDoNotWaitForConfirmation(command);
                stream.close(); // this would close connector non-daemon thread
//                sleepAndPrintNonDaemons(4000);
            }
        }, "Sends command specified as second argument");
        registerTool("reboot_ecu", args -> sendNonBlockingCommandDoNotWaitForConfirmation(Integration.CMD_REBOOT), "Sends a command to reboot rusEFI controller.");
        registerTool(Integration.CMD_REBOOT_DFU, args -> {
            sendNonBlockingCommandDoNotWaitForConfirmation(Integration.CMD_REBOOT_DFU);
            /**
             * AndreiKA reports that auto-detect fails to interrupt communication threads while in native code
             * See https://github.com/rusefi/rusefi/issues/3300
             */
            System.exit(0);
        }, "Sends a command to switch rusEFI controller into DFU mode.");
    }

    private static void localProxy(String[] strings) throws IOException {
        String autoDetectedPort = autoDetectPort();
        if (autoDetectedPort == null) {
            System.out.println(RUS_EFI_NOT_DETECTED);
            return;
        }
        IoStream ecuStream = UiLinkManagerHelper.open(autoDetectedPort);

        ServerSocketReference serverHolder = BinaryProtocolProxy.createProxy(ecuStream, 29001, new BinaryProtocolProxy.ClientApplicationActivityListener() {
            @Override
            public void onActivity(BinaryProtocolServer.Packet clientRequest) {

            }
        }, StatusConsumer.ANONYMOUS);

    }

    private static void version(String[] strings) {
        // version is printed by already, all we need is to do nothing
    }

    public static void main(String[] args) throws Exception {
        System.out.println(Arrays.toString(new File(".").list()));
        System.setProperty("ini_file_path", "../firmware/tunerstudio");
//        calcBinaryImageTuneCrc(null, "current_configuration.rusefi_binary");

//        calcXmlImageTuneCrc(null, "CurrentTune.msq");
    }

    private static void calcBinaryImageTuneCrc(String... args) throws IOException {
        String fileName = args[1];
        ConfigurationImage image = ConfigurationImageFile.readFromFile(fileName).getConfigurationImage();
        printCrc(image);
    }

    private static void printCrc(ConfigurationImage image) {
        int crc32 = getCrc32(image.getContent());
        int crc16 = crc32 & 0xFFFF;
        System.out.printf("tune_CRC32_hex=0x%x\n", crc32);
        System.out.printf("tune_CRC16_hex=0x%x\n", crc16);
        System.out.println("tune_CRC16=" + crc16);
    }

    private static void uploadTune(String[] args) {
        String fileName = args[1];
        String authToken = AuthTokenPanel.getAuthToken();
        System.out.println("Trying to upload " + fileName + " using " + authToken);
        Online.upload(new File(fileName), authToken);
    }

    private static void registerTool(String command, ConsoleTool callback, String help) {
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

    private static IoStream sendNonBlockingCommandDoNotWaitForConfirmation(String command) throws IOException {
        String autoDetectedPort = autoDetectPort();
        if (autoDetectedPort == null)
            return null;
        IoStream stream = UiLinkManagerHelper.open(autoDetectedPort);
        byte[] commandBytes = BinaryProtocol.getTextCommandBytes(command);
        stream.sendPacket(commandBytes);
        return stream;
    }
/*
    private static void sleepAndPrintNonDaemons(final int millis) {
        new Thread(null, new Runnable() {
            @Override
            public void run() {
                System.out.println("Sleeping " + millis);
                try {
                    Thread.sleep(millis);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                for (Thread thread : Thread.getAllStackTraces().keySet()) {
                    // Daemon thread will not prevent the JVM from exiting
                    if (!thread.isDaemon())
                        System.out.println("Non-daemon: " + thread.getName() + "\n");

                }

            }
        }, "test").start();
    }
*/
    private static void setAuthToken(String[] args) {
        String newToken = args[1];
        System.out.println("Saving auth token " + newToken);
        AuthTokenPanel.setAuthToken(newToken);
    }

    private static void printVehicleToken() {
        int vehicleToken = VehicleToken.getOrCreate();
        System.out.println("Vehicle token: " + vehicleToken);
    }

    private static void printAuthToken() {
        String authToken = AuthTokenPanel.getAuthToken();
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
        HwCiF4Discovery.main(toolArgs);
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

        startAndConnect(linkManager -> {
            new BinaryProtocolServer().start(linkManager);
            return null;
        });
    }

    public static void startAndConnect(final Function<LinkManager, Void> onConnectionEstablished) {

        String autoDetectedPort = PortDetector.autoDetectSerial(null).getSerialPort();
        if (autoDetectedPort == null) {
            System.err.println(RUS_EFI_NOT_DETECTED);
            return;
        }
        LinkManager linkManager = new LinkManager();
        linkManager.startAndConnect(autoDetectedPort, new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                onConnectionEstablished.apply(linkManager);
            }

            @Override
            public void onConnectionFailed(String s) {

            }
        });
    }

    private static void readTune() {
        startAndConnect(linkManager -> {
            System.out.println("Loaded! Exiting");
            System.exit(0);
            return null;
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
        String autoDetectedPort = PortDetector.autoDetectSerial(null).getSerialPort();
        if (autoDetectedPort == null) {
            System.err.println(RUS_EFI_NOT_DETECTED);
            return null;
        }
        return autoDetectedPort;
    }

    static void detect(String[] strings) throws IOException {
        SerialAutoChecker.AutoDetectResult detectResult = PortDetector.autoDetectSerial(null);
        String autoDetectedPort = detectResult.getSerialPort();
        if (autoDetectedPort == null) {
            System.out.println(RUS_EFI_NOT_DETECTED);
            return;
        }
        IoStream stream = UiLinkManagerHelper.open(autoDetectedPort);
        IncomingDataBuffer incomingData = stream.getDataBuffer();
        byte[] commandBytes = BinaryProtocol.getTextCommandBytes("hello");
        stream.sendPacket(commandBytes);
        // skipping response
        incomingData.getPacket("");

        sleep(300);
        stream.sendPacket(new byte[]{Integration.TS_GET_TEXT});
        sleep(300);

        byte[] response = incomingData.getPacket("");
        if (response == null) {
            System.out.println("No response");
            return;
        }
        String textResponse = new String(response, 1, response.length - 1);

        StringBuilder messages = new StringBuilder();

        ResponseBuffer responseBuffer = new ResponseBuffer(unpack -> {
            EngineState.ValueCallback<String> callback = new EngineState.ValueCallback<String>() {
                @Override
                public void onUpdate(String value) {
                    if (value.startsWith(Integration.PROTOCOL_HELLO_PREFIX)) {
                        messages.append(value);
                        messages.append("\n");
                    }
                }
            };
            while (!unpack.isEmpty()) {
                String original = unpack;
                unpack = EngineState.handleStringActionPair(unpack, new EngineState.StringActionPair(Integration.PROTOCOL_MSG, callback), null);
                if (original.length() == unpack.length()) {
                    // skip key
                    unpack = EngineState.skipToken(unpack);
                    // skip value
                    unpack = EngineState.skipToken(unpack);
                }
            }
        });
        responseBuffer.append(textResponse + "\r\n", LinkManager.ENCODER);

        System.out.println("Signature: " + detectResult.getSignature());
        System.out.println("It says " + messages);
        Pair<String, String> stringPair = SignatureHelper.getUrl(detectResult.getSignature());
        if (stringPair != null)
            System.out.println("Ini file: " + stringPair.first);
        System.exit(0);
    }

    interface ConsoleTool {
        void runTool(String[] args) throws Exception;
    }
}
