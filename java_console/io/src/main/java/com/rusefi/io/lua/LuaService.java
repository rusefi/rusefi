package com.rusefi.io.lua;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import com.rusefi.io.commands.BurnCommand;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.Objects;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Headless, reusable Lua-script-on-ECU operations.
 *
 * Extracted from {@code SetLuaTool} so it can be driven by the CLI, the MCP server,
 * tests and any other host without {@code System.exit} / stdout side effects.
 *
 * All ECU access is funnelled through {@link LinkManager#submit(Runnable)} as required
 * by the binary protocol.
 */
public class LuaService {
    private static final Logging log = getLogging(LuaService.class);

    /** Maximum time we wait for {@code luareset} to flush over the wire before returning. */
    private static final long LUARESET_GRACE_MS = 2000;

    public static final String LUASCRIPT_FIELD = "LUASCRIPT";

    private LuaService() {
    }

    /**
     * Locate the {@code LUASCRIPT} {@link StringIniField} backing the Lua script on the ECU.
     * Headless equivalent of {@code LuaScriptPanel.getLuaScriptField}.
     */
    public static StringIniField getLuaScriptField(BinaryProtocol bp) {
        Objects.requireNonNull(bp, "BinaryProtocol");
        IniFileModel iniFile = bp.getIniFileNullable();
        if (iniFile == null)
            return null;
        // findIniField searches both the main page (old firmware) and secondary
        // pages (new firmware places luaScript on its own dedicated TS page)
        return iniFile.findIniField(LUASCRIPT_FIELD)
                .filter(f -> f instanceof StringIniField)
                .map(f -> (StringIniField) f)
                .orElse(null);
    }

    /** Autodetect a serial port; returns {@code null} if no ECU is found. */
    public static String autoDetectPort() {
        return PortDetector.autoDetectSerial(null).getSerialPort();
    }

    /**
     * Connect to the given port (or autodetect if {@code null}) and return a connected
     * {@link LinkManager}. Throws on connection failure / timeout. Caller owns the
     * returned manager and is responsible for {@link LinkManager#close()}.
     */
    public static LinkManager connect(String portOrNull, long timeoutMs) throws Exception {
        String port = portOrNull != null ? portOrNull : autoDetectPort();
        if (port == null)
            throw new IllegalStateException("rusEFI not detected");

        LinkManager linkManager = new LinkManager();
        CountDownLatch connected = new CountDownLatch(1);
        AtomicReference<String> failure = new AtomicReference<>();

        linkManager.startAndConnect(port, new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
            }

            @Override
            public void onConnectionEstablished() {
                connected.countDown();
            }

            @Override
            public void onConnectionFailed(String s) {
                failure.set(s);
                connected.countDown();
            }
        });

        if (!connected.await(timeoutMs, TimeUnit.MILLISECONDS)) {
            linkManager.close();
            throw new IllegalStateException("Timeout connecting to " + port);
        }
        if (failure.get() != null) {
            linkManager.close();
            throw new IllegalStateException("Connection failed: " + failure.get());
        }
        return linkManager;
    }

    /**
     * Apply a Lua script to the ECU: write LUASCRIPT, burn, then luareset.
     * Runs synchronously, blocking up to {@code timeoutMs}.
     */
    public static LuaApplyResult applyLuaScript(LinkManager linkManager, String script, long timeoutMs)
            throws InterruptedException {
        Objects.requireNonNull(linkManager, "linkManager");
        Objects.requireNonNull(script, "script");

        AtomicReference<LuaApplyResult> result = new AtomicReference<>();
        CountDownLatch done = new CountDownLatch(1);

        linkManager.submit(() -> {
            try {
                BinaryProtocol bp = linkManager.getBinaryProtocol();
                if (bp == null) {
                    result.set(LuaApplyResult.failure("BinaryProtocol is null"));
                    return;
                }
                StringIniField field = getLuaScriptField(bp);
                if (field == null) {
                    result.set(LuaApplyResult.failure("LUASCRIPT field not found in ini file"));
                    return;
                }
                if (script.length() >= field.getSize()) {
                    result.set(LuaApplyResult.failure(
                            script.length() + " bytes does not fit lua script field size " + field.getSize()));
                    return;
                }
                byte[] padded = new byte[field.getSize()];
                byte[] scriptBytes = script.getBytes(StandardCharsets.US_ASCII);
                System.arraycopy(scriptBytes, 0, padded, 0, scriptBytes.length);
                log.info("Writing " + scriptBytes.length + " bytes to ECU at offset " + field.getOffset()
                        + " page " + field.getPageIndex());
                bp.writeInBlocks(padded, 0, field.getOffset(), padded.length, field.getPageIndex());
                boolean burned = BurnCommand.execute(bp, field.getPageIndex());
                linkManager.getCommandQueue().write(Integration.CMD_LUA_RESET);
                Thread.sleep(LUARESET_GRACE_MS);
                result.set(LuaApplyResult.success(scriptBytes.length, field.getSize(), burned));
            } catch (Throwable t) {
                log.error("applyLuaScript failed", t);
                result.set(LuaApplyResult.failure(t.toString()));
            } finally {
                done.countDown();
            }
        });

        if (!done.await(timeoutMs, TimeUnit.MILLISECONDS))
            return LuaApplyResult.failure("Timeout after " + timeoutMs + "ms");
        return result.get();
    }

    /** Read the current LUASCRIPT field: cached image for page 0, live page read otherwise. */
    public static String readLuaScript(LinkManager linkManager) throws InterruptedException {
        // secondary-page reads talk to the ECU, so everything runs on the link thread
        AtomicReference<Object> outcome = new AtomicReference<>();
        CountDownLatch done = new CountDownLatch(1);
        linkManager.submit(() -> {
            try {
                outcome.set(readLuaScriptOnLinkThread(linkManager));
            } catch (Throwable t) {
                outcome.set(t);
            } finally {
                done.countDown();
            }
        });
        if (!done.await(60, TimeUnit.SECONDS))
            throw new IllegalStateException("Timeout reading luaScript");
        if (outcome.get() instanceof Throwable) {
            Throwable t = (Throwable) outcome.get();
            throw t instanceof RuntimeException ? (RuntimeException) t : new IllegalStateException(t);
        }
        return (String) outcome.get();
    }

    private static String readLuaScriptOnLinkThread(LinkManager linkManager) {
        BinaryProtocol bp = linkManager.getBinaryProtocol();
        if (bp == null)
            throw new IllegalStateException("BinaryProtocol is null");
        StringIniField field = getLuaScriptField(bp);
        if (field == null)
            throw new IllegalStateException("LUASCRIPT field not found");
        byte[] arr;
        if (field.getPageIndex() == 0) {
            // main settings page: comes from the already-fetched controller image
            ConfigurationImage image = bp.getControllerConfiguration();
            if (image == null)
                throw new IllegalStateException("No configuration image");
            ByteBuffer buf = image.getByteBuffer(field.getOffset(), field.getSize());
            arr = new byte[field.getSize()];
            buf.get(arr);
        } else {
            // secondary page (new firmware places luaScript on its own TS page);
            // the cached image only holds page 0, so fetch directly from the ECU
            arr = bp.readFromPage(field.getPageIndex(), field.getOffset(), field.getSize());
            if (arr == null)
                throw new IllegalStateException("Failed to read luaScript from page " + field.getPageIndex());
        }
        int end = 0;
        while (end < arr.length && arr[end] != 0) end++;
        return new String(arr, 0, end, StandardCharsets.US_ASCII);
    }

    /** Send {@code luareset} via the standard command queue. */
    public static void luaReset(LinkManager linkManager) {
        linkManager.getCommandQueue().write(Integration.CMD_LUA_RESET);
    }

    /** Send an arbitrary text command via the standard command queue. */
    public static void sendCommand(LinkManager linkManager, String command) {
        linkManager.getCommandQueue().write(command);
    }

    public static final class LuaApplyResult {
        public final boolean success;
        public final String error;
        public final int bytesWritten;
        public final int fieldSize;
        public final boolean burnSucceeded;

        private LuaApplyResult(boolean success, String error, int bytesWritten, int fieldSize, boolean burnSucceeded) {
            this.success = success;
            this.error = error;
            this.bytesWritten = bytesWritten;
            this.fieldSize = fieldSize;
            this.burnSucceeded = burnSucceeded;
        }

        static LuaApplyResult success(int bytesWritten, int fieldSize, boolean burnSucceeded) {
            return new LuaApplyResult(true, null, bytesWritten, fieldSize, burnSucceeded);
        }

        static LuaApplyResult failure(String error) {
            return new LuaApplyResult(false, error, 0, 0, false);
        }

        @Override
        public String toString() {
            return success
                    ? "LuaApplyResult{ok, bytes=" + bytesWritten + "/" + fieldSize + ", burn=" + burnSucceeded + "}"
                    : "LuaApplyResult{FAIL: " + error + "}";
        }
    }
}
