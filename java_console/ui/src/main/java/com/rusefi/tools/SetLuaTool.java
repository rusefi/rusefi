package com.rusefi.tools;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.commands.BurnCommand;
import com.rusefi.ui.lua.LuaScriptPanel;

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class SetLuaTool {
    private static final Logging log = Logging.getLogging(ConsoleTools.class);

    static void setLua(String[] args) throws Exception {
        System.out.println("set_lua: starting with args=" + java.util.Arrays.toString(args));
        if (args.length < 2)
            throw new IllegalStateException("luaFileName argument expected");
        String luaFileName = args[1];
        File luaFile = new File(luaFileName);
        System.out.println("set_lua: resolving lua file [" + luaFileName + "] absolute=[" + luaFile.getAbsolutePath() + "]");
        if (!luaFile.exists())
            throw new IllegalStateException("Lua file not found: " + luaFile.getAbsolutePath());
        String script = new String(Files.readAllBytes(Paths.get(luaFileName)), StandardCharsets.US_ASCII);
        System.out.println("set_lua: read " + script.length() + " bytes from " + luaFile.getAbsolutePath());
        log.info("Read " + script.length() + " bytes from " + luaFile.getAbsolutePath());

        java.util.concurrent.atomic.AtomicBoolean success = new java.util.concurrent.atomic.AtomicBoolean(false);
        CountDownLatch latch = new CountDownLatch(1);

        System.out.println("set_lua: auto-detecting ECU serial port...");
        String autoDetectedPort = com.rusefi.autodetect.PortDetector.autoDetectSerial(null).getSerialPort();
        if (autoDetectedPort == null) {
            System.err.println("set_lua: " + ConsoleTools.RUS_EFI_NOT_DETECTED);
            System.exit(1);
            return;
        }
        System.out.println("set_lua: detected ECU on port [" + autoDetectedPort + "], connecting...");

        com.rusefi.io.LinkManager linkManager = new com.rusefi.io.LinkManager();
        linkManager.startAndConnect(autoDetectedPort, new com.rusefi.io.ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                System.out.println("set_lua: onConnectionStatus isConnected=" + isConnected);
            }

            @Override
            public void onConnectionEstablished() {
                System.out.println("set_lua: connection established, submitting write/burn task");
                linkManager.submit(() -> {
                    try {
                        BinaryProtocol bp = linkManager.getBinaryProtocol();
                        if (bp == null) {
                            System.err.println("set_lua: BinaryProtocol is null");
                            return;
                        }
                        System.out.println("set_lua: looking up LUASCRIPT field in ini...");
                        StringIniField luaScriptField = LuaScriptPanel.getLuaScriptField(bp);
                        if (luaScriptField == null) {
                            System.err.println("set_lua: LUASCRIPT field not found in ini file");
                            return;
                        }
                        System.out.println("set_lua: LUASCRIPT field offset=" + luaScriptField.getOffset() + " size=" + luaScriptField.getSize());
                        if (script.length() >= luaScriptField.getSize()) {
                            System.err.println("set_lua: " + script.length() + " bytes does not fit lua script field size " + luaScriptField.getSize());
                            return;
                        }
                        byte[] padded = new byte[luaScriptField.getSize()];
                        byte[] scriptBytes = script.getBytes(StandardCharsets.US_ASCII);
                        System.arraycopy(scriptBytes, 0, padded, 0, scriptBytes.length);
                        System.out.println("set_lua: writing " + scriptBytes.length + " bytes to ECU at offset " + luaScriptField.getOffset());
                        bp.writeInBlocks(padded, 0, luaScriptField.getOffset(), padded.length);
                        System.out.println("set_lua: write complete, burning to ECU...");
                        boolean burnResult = BurnCommand.execute(bp);
                        System.out.println("set_lua: burn result=" + burnResult);
                        System.out.println("set_lua: queueing luareset...");
                        linkManager.getCommandQueue().write("luareset");
                        // give luareset a moment to flush over the wire before we exit
                        Thread.sleep(2000);
                        success.set(true);
                        System.out.println("set_lua: task completed successfully");
                    } catch (Throwable t) {
                        System.err.println("set_lua: ERROR while applying lua: " + t);
                        t.printStackTrace();
                    } finally {
                        latch.countDown();
                    }
                });
            }

            @Override
            public void onConnectionFailed(String s) {
                System.err.println("set_lua: connection failed: " + s);
                latch.countDown();
            }
        });

        System.out.println("set_lua: awaiting completion (up to 120s)...");
        if (!latch.await(120, TimeUnit.SECONDS)) {
            System.err.println("set_lua: TIMEOUT while applying lua script");
            System.exit(2);
        } else if (success.get()) {
            System.out.println("set_lua: Lua script applied successfully");
            System.exit(0);
        } else {
            System.err.println("set_lua: FAILED to apply lua script (see errors above)");
            System.exit(3);
        }
    }
}
