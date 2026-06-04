package com.rusefi.tools;

import com.devexperts.logging.Logging;
import com.rusefi.io.LinkManager;
import com.rusefi.io.lua.LuaService;

import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;

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

        System.out.println("set_lua: auto-detecting ECU serial port...");
        String autoDetectedPort = LuaService.autoDetectPort();
        if (autoDetectedPort == null) {
            System.err.println("set_lua: " + ConsoleTools.RUS_EFI_NOT_DETECTED);
            System.exit(1);
            return;
        }
        System.out.println("set_lua: detected ECU on port [" + autoDetectedPort + "], connecting...");

        LinkManager linkManager;
        try {
            linkManager = LuaService.connect(autoDetectedPort, 60_000);
        } catch (Exception e) {
            System.err.println("set_lua: " + e.getMessage());
            System.exit(1);
            return;
        }
        System.out.println("set_lua: connection established, applying script...");

        try {
            LuaService.LuaApplyResult result = LuaService.applyLuaScript(linkManager, script, 120_000);
            if (result.success) {
                System.out.println("set_lua: Lua script applied successfully (" + result + ")");
                System.exit(0);
            } else {
                System.err.println("set_lua: FAILED to apply lua script: " + result.error);
                System.exit(3);
            }
        } finally {
            linkManager.close();
        }
    }
}
