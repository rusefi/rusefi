package com.rusefi.tools;

import com.opensr5.ConfigurationImage;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.*;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.EngineState;
import com.rusefi.core.Pair;
import com.rusefi.core.ResponseBuffer;
import com.rusefi.core.SignatureHelper;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.maintenance.ExecHelper;
import org.jetbrains.annotations.Nullable;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.Map;
import java.util.TreeMap;
import java.util.function.Consumer;
import java.util.function.Function;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
import static com.rusefi.binaryprotocol.IoHelper.getCrc32;

public class ConsoleTools {
    public static final String RUS_EFI_NOT_DETECTED = "rusEFI not detected";
    private static final Map<String, ConsoleTool> TOOLS = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    private static final Map<String, String> toolsHelp = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    static {
        registerTool("help", args -> printTools(), "Print this help.");
        registerTool("headless", ConsoleTools::runHeadless, "Connect to rusEFI controller and start saving logs.");

        registerTool("ptrace_enums", ConsoleTools::runPerfTraceTool, "NOT A USER TOOL. Development tool to process performance trace enums");
        registerTool("convert_binary_configuration_to_xml", ConsoleTools::convertBinaryToXml, "NOT A USER TOOL. Development tool to convert binary configuration into XML form.");

        registerTool("get_image_tune_crc", ConsoleTools::calcBinaryImageTuneCrc, "Calculate tune CRC for given binary tune");

        registerTool("get_performance_trace", args -> PerformanceTraceHelper.getPerformanceTune(), "DEV TOOL: Get performance trace from ECU");

        registerTool("version", ConsoleTools::version, "Only print version");

        registerTool("detect", ConsoleTools::detect, "Find attached rusEFI");
        registerTool("send_command", args -> {
            String command = args[1];
            System.out.println("Sending command " + command);
            sendCommand(command);
        }, "Sends command specified as second argument");
        registerTool("reboot_ecu", args -> sendCommand(Fields.CMD_REBOOT), "Sends a command to reboot rusEFI controller.");
        registerTool(Fields.CMD_REBOOT_DFU, args -> {
            sendCommand(Fields.CMD_REBOOT_DFU);
            /**
             * AndreiKA reports that auto-detect fails to interrupt communication threads while in native code
             * See https://github.com/rusefi/rusefi/issues/3300
             */
            System.exit(0);
        }, "Sends a command to switch rusEFI controller into DFU mode.");
    }

    private static void version(String[] strings) {
        // version is printed by already, all we need is to do nothing
    }

    public static void main(String[] args) throws Exception {
        System.out.println(Arrays.toString(new File(".").list()));
        System.setProperty("ini_file_path", "../firmware/tunerstudio");
//        calcBinaryImageTuneCrc(null, "current_configuration.rusefi_binary");

    }

    private static void calcBinaryImageTuneCrc(String... args) throws IOException {
        String fileName = args[1];
        ConfigurationImage image = ConfigurationImageFile.readFromFile(fileName);
        printCrc(image);
    }

    private static void printCrc(ConfigurationImage image) {
        int crc32 = getCrc32(image.getContent());
        int crc16 = crc32 & 0xFFFF;
        System.out.printf("tune_CRC32_hex=0x%x\n", crc32);
        System.out.printf("tune_CRC16_hex=0x%x\n", crc16);
        System.out.println("tune_CRC16=" + crc16);
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

    private static void sendCommand(String command) throws IOException {
        String autoDetectedPort = autoDetectPort();
        if (autoDetectedPort == null)
            return;
        IoStream stream = LinkManager.open(autoDetectedPort);
        byte[] commandBytes = BinaryProtocol.getTextCommandBytes(command);
        stream.sendPacket(commandBytes);
    }


    private static void runPerfTraceTool(String[] args) throws IOException {
        PerfTraceTool.readPerfTrace(args[1], args[2], args[3], args[4]);
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

        String autoDetectedPort = PortDetector.autoDetectSerial().getSerialPort();
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
        String autoDetectedPort = PortDetector.autoDetectSerial().getSerialPort();
        if (autoDetectedPort == null) {
            System.err.println(RUS_EFI_NOT_DETECTED);
            return null;
        }
        return autoDetectedPort;
    }

    private static void convertBinaryToXml(String[] args) throws IOException {
        if (args.length < 2) {
            System.err.println("Binary file input expected");
            System.exit(-1);
        }
        String inputBinaryFileName = args[1];
        ConfigurationImage image = ConfigurationImageFile.readFromFile(inputBinaryFileName);
        System.out.println("Got " + image.getSize() + " of configuration from " + inputBinaryFileName);
    }

    static void detect(String[] strings) throws IOException {
        SerialAutoChecker.AutoDetectResult detectResult = PortDetector.autoDetectSerial();
        String autoDetectedPort = detectResult.getSerialPort();
        if (autoDetectedPort == null) {
            System.out.println(RUS_EFI_NOT_DETECTED);
            return;
        }
        IoStream stream = LinkManager.open(autoDetectedPort);
        IncomingDataBuffer incomingData = stream.getDataBuffer();
        byte[] commandBytes = BinaryProtocol.getTextCommandBytes("hello");
        stream.sendPacket(commandBytes);
        // skipping response
        incomingData.getPacket("");

        sleep(300);
        stream.sendPacket(new byte[]{Fields.TS_GET_TEXT});
        sleep(300);

        byte[] response = incomingData.getPacket("");
        if (response == null) {
            System.out.println("No response");
            return;
        }
        String textResponse = new String(response, 1, response.length - 1);

        StringBuilder messages = new StringBuilder();

        ResponseBuffer responseBuffer = new ResponseBuffer(unpack -> {
            Consumer<String> callback = (String value) -> {
                if (value.startsWith(Fields.PROTOCOL_HELLO_PREFIX)) {
                    messages.append(value);
                    messages.append("\n");
                }
            };
            while (!unpack.isEmpty()) {
                String original = unpack;
                unpack = EngineState.handleStringActionPair(unpack, new EngineState.StringActionPair(Fields.PROTOCOL_MSG, callback), null);
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
