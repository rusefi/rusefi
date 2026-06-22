package com.rusefi.autodetect;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.IoUtil;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IniNotFoundException;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import com.rusefi.core.MessagesCentral;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionWatchdog;
import com.rusefi.io.LinkManager;
import com.rusefi.io.lua.LuaService;
import com.rusefi.ui.lua.LuaIncludeSyntax;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

/**
 * Headless manual harness that scans serial ports, connects to ECU, resolves INI, and prints live gauges/messages.
 */
public class HeadlessConnectivitySandbox {
    private static final long STATUS_PRINT_PERIOD_MS = 1000;

    public static void main(String[] args) {
        String luaFilePath = args.length > 0 ? args[0] : null;

        LinkManager linkManager = new LinkManager();
        AtomicInteger reconnectCounter = new AtomicInteger();
        AtomicInteger disconnectedCounter = new AtomicInteger();
        AtomicLong frameCounter = new AtomicLong();
        AtomicLong messageCounter = new AtomicLong();

        waitForDeviceAndStart(linkManager);
        ConnectionWatchdog.init(linkManager);

        ConnectionStatusLogic.INSTANCE.addListener(new ConnectionStatusLogic.Listener() {
            private volatile boolean seenConnected;

            @Override
            public void onConnectionStatus(boolean isConnected) {
                if (isConnected) {
                    if (seenConnected) {
                        int reconnects = reconnectCounter.incrementAndGet();
                        System.out.println("[CONNECTION] reconnect #" + reconnects);
                    } else {
                        seenConnected = true;
                        System.out.println("[CONNECTION] first successful connection");

                        if (luaFilePath != null) {
                            System.out.println("[LUA] uploading " + luaFilePath);
                            try {
                                Path scriptPath = Paths.get(luaFilePath);
                                String script = new String(Files.readAllBytes(scriptPath), StandardCharsets.US_ASCII);
                                // Support --include relative to the script path
                                script = LuaIncludeSyntax.reloadScript(script, name -> {
                                    try {
                                        return new String(Files.readAllBytes(scriptPath.getParent().resolve(name)), StandardCharsets.US_ASCII);
                                    } catch (IOException e) {
                                        throw new RuntimeException("Error reading included file [" + name + "]: " + e.getMessage());
                                    }
                                });

                                LuaService.LuaApplyResult r = LuaService.applyLuaScript(linkManager, script, 120_000L);
                                System.out.println("[LUA] result: " + r);
                            } catch (Exception e) {
                                System.err.println("[LUA] Error: " + e.getMessage());
                            }
                        }
                    }
                    printIniAndGaugeAvailability(linkManager);
                } else {
                    int disconnects = disconnectedCounter.incrementAndGet();
                    System.out.println("[CONNECTION] disconnected #" + disconnects);
                }
            }
        });

        MessagesCentral.getInstance().addListener((clazz, message) -> {
            long index = messageCounter.incrementAndGet();
            System.out.println("[MESSAGE #" + index + "] " + clazz.getSimpleName() + ": " + message);
        });

        SensorCentral.getInstance().addListener(() -> frameCounter.incrementAndGet(), null);

        long lastFrameCount = 0;
        while (true) {
            IoUtil.sleepSeconds((int) (STATUS_PRINT_PERIOD_MS / 1000));
            long nowFrames = frameCounter.get();
            long framesDelta = nowFrames - lastFrameCount;
            lastFrameCount = nowFrames;

            double rpm = SensorCentral.getInstance().getValue("RPMValue");
            double tps = SensorCentral.getInstance().getValue("TPSValue");
            double map = SensorCentral.getInstance().getValue("MAPValue");
            double dataRateHz = SensorCentral.getInstance().getValue("runtimeDataRateGauge");

            System.out.printf("[STATUS] frames=%d (+%d/s), reconnects=%d, disconnects=%d, messages=%d, dataRate=%.1f Hz, RPM=%.0f, TPS=%.1f, MAP=%.1f%n",
                nowFrames,
                framesDelta,
                reconnectCounter.get(),
                disconnectedCounter.get(),
                messageCounter.get(),
                dataRateHz,
                rpm,
                tps,
                map);
        }
    }

    private static void printIniAndGaugeAvailability(LinkManager linkManager) {
        BinaryProtocol bp = linkManager.getBinaryProtocol();
        if (bp == null) {
            System.out.println("[INI] BinaryProtocol is not ready yet");
            return;
        }
        String signature = bp.signature;
        if (signature == null) {
            System.out.println("[INI] signature is not known yet");
            return;
        }

        System.out.println("[INI] signature=" + signature);

        try {
            IniFileModel model = new RealIniFileProvider().provide(signature);
            System.out.println("[INI] resolved via local/download cache and TS cache primed");
            printGaugeChannelIfPresent(model, "RPM", "RPMValue");
            printGaugeChannelIfPresent(model, "TPS", "TPSValue");
            printGaugeChannelIfPresent(model, "MAP", "MAPValue");
        } catch (IniNotFoundException e) {
            System.out.println("[INI] failed to resolve for signature: " + e.getMessage());
        }
    }

    private static void printGaugeChannelIfPresent(IniFileModel model, String gaugeLabel, String preferredChannel) {
        GaugeModel gauge = model.getGauges().get(gaugeLabel);
        if (gauge == null) {
            GaugeModel maybeByChannel = model.getGauges().values().stream()
                .filter(g -> preferredChannel.equalsIgnoreCase(g.getChannel()))
                .findFirst()
                .orElse(null);

            if (maybeByChannel == null) {
                System.out.println("[INI] " + gaugeLabel + " gauge not present");
                return;
            }
            System.out.println("[INI] " + gaugeLabel + " gauge channel=" + maybeByChannel.getChannel());
            return;
        }
        System.out.println("[INI] " + gaugeLabel + " gauge channel=" + gauge.getChannel());
    }

    private static void waitForDeviceAndStart(LinkManager linkManager) {
        while (true) {
            SerialAutoChecker.AutoDetectResult autoDetectResult = PortDetector.autoDetectSerial(null);
            String port = autoDetectResult.getSerialPort();
            System.out.println("[SCAN] port=" + port + ", signature=" + autoDetectResult.getSignature());
            if (port == null) {
                IoUtil.sleepSeconds(1);
                continue;
            }
            System.out.println("[CONNECT] first detected port " + port);
            linkManager.startAndConnect(port, ConnectionStatusLogic.Listener.VOID);
            return;
        }
    }
}
