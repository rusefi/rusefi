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
        if (args.length < 2)
            throw new IllegalStateException("luaFileName argument expected");
        String luaFileName = args[1];
        File luaFile = new File(luaFileName);
        if (!luaFile.exists())
            throw new IllegalStateException("Lua file not found: " + luaFile.getAbsolutePath());
        String script = new String(Files.readAllBytes(Paths.get(luaFileName)), StandardCharsets.US_ASCII);
        log.info("Read " + script.length() + " bytes from " + luaFile.getAbsolutePath());

        CountDownLatch latch = new CountDownLatch(1);
        ConsoleTools.startAndConnect(linkManager -> {
            linkManager.submit(() -> {
                try {
                    BinaryProtocol bp = linkManager.getBinaryProtocol();
                    StringIniField luaScriptField = LuaScriptPanel.getLuaScriptField(bp);
                    if (luaScriptField == null) {
                        System.err.println("LUASCRIPT field not found in ini file");
                        latch.countDown();
                        return;
                    }
                    if (script.length() >= luaScriptField.getSize()) {
                        System.err.println(script.length() + " bytes does not fit lua script field size " + luaScriptField.getSize());
                        latch.countDown();
                        return;
                    }
                    byte[] padded = new byte[luaScriptField.getSize()];
                    byte[] scriptBytes = script.getBytes(StandardCharsets.US_ASCII);
                    System.arraycopy(scriptBytes, 0, padded, 0, scriptBytes.length);
                    log.info("Writing " + scriptBytes.length + " bytes to ECU at offset " + luaScriptField.getOffset());
                    bp.writeInBlocks(padded, 0, luaScriptField.getOffset(), padded.length);
                    log.info("Burning to ECU...");
                    BurnCommand.execute(bp);
                    log.info("Resetting Lua...");
                    linkManager.getCommandQueue().write("luareset");
                } finally {
                    latch.countDown();
                }
            });
            return null;
        });
        if (!latch.await(60, TimeUnit.SECONDS)) {
            System.err.println("Timeout while applying lua script");
        } else {
            System.out.println("Lua script applied successfully");
        }
        System.exit(0);
    }
}
