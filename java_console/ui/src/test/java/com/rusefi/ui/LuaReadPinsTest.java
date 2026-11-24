package com.rusefi.ui;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.SwingUtilities;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;

/** A Lua script reading pins with readPin() makes the Pinout pickers warn: the console cannot map those pins. */
class LuaReadPinsTest {
    private static final String B6 = "B6 Low Side output";

    @Test
    void findsReadPinCallsOutsideComments() {
        assertFalse(LuaReadPins.callsReadPin(null));
        assertFalse(LuaReadPins.callsReadPin("setTickRate(10)\nstartPwm(0, 100, 0.5)"),
            "Lua PWM uses tune settings, which Pinout already shows");
        assertFalse(LuaReadPins.callsReadPin("-- x = readPin(\"PE11\")\n--[[ readPin('PA0') ]]\n--[==[\nreadPin(\"PB1\")\n]==]"),
            "commented-out calls do not count");
        assertFalse(LuaReadPins.callsReadPin("myreadPin(\"PE11\")"), "only the readPin hook");

        String script = "local a = readPin(\"PE11\")\n"
            + "local b = readPin( 'PD5' ) -- door switch\n"
            + "local c = readPin(\"PE11\")\n"
            + "local name = \"PA\" .. 3\nlocal d = readPin(name)\n";
        assertTrue(LuaReadPins.callsReadPin(script));
        assertEquals(Arrays.asList("PE11", "PD5"), LuaReadPins.literalPins(script),
            "named pins in order, once each; a computed name is not listed");
        assertEquals("Your Lua script reads pins directly with readPin(\"PE11\", \"PD5\"). The console cannot tell "
            + "which connector pins those are: check the script does not use B6.", LuaReadPins.note(script, "B6"));
        assertEquals("", LuaReadPins.note("print('hi')", "B6"));
        assertTrue(LuaReadPins.note("readPin(name)", "B6").startsWith("Your Lua script reads pins directly with readPin()."));
    }

    @Test
    void pickersShowTheNoteOnceTheScriptArrives() {
        PinoutPin b6 = new PinoutPin("B6", "Low side", "outputs", "outputs", B6, "");
        PinAssignPanel assign = new PinAssignPanel(b6, "Connector B", Collections.emptyList(), ts -> ts);
        assertFalse(assign.luaNote.isVisible(), "nothing until the script is read");
        assign.setLuaScript("print('no pins')");
        assertFalse(assign.luaNote.isVisible());
        assign.setLuaScript("if readPin(\"PE11\") == 1 then end");
        assertTrue(assign.luaNote.isVisible());
        assertTrue(assign.luaNote.getText().endsWith("check the script does not use B6."));

        PinSwapPanel swap = new PinSwapPanel(b6, "Connector B", Collections.emptyList(), Collections.emptyList());
        swap.setLuaScript("readPin(\"PE11\")");
        assertTrue(swap.luaNote.isVisible());
        assertTrue(swap.luaNote.getText().endsWith("check the script does not use B6 or the pin you swap it with."));
        swap.setLuaScript(null);
        assertFalse(swap.luaNote.isVisible(), "an unknown script shows no note");
    }

    /** Old firmware and offline tunes keep the script in the main tune image: no ECU read needed. */
    @Test
    void scriptOnTheMainPageIsReadFromTheTune() throws Throwable {
        String text = "[Constants]\n"
            + "signature = \"pinout-lua-test\"\n"
            + "nPages = 1\npageSize = 33\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "fanPin = bits, U08, 0, [0:0], \"NONE\", \"" + B6 + "\"\n"
            + "luaScript = string, ASCII, 1, 32\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        IniFileModel ini = IniFileReaderUtil.readIniFile(raw, "pinout-lua-test.ini", new IniFileMetaInfoImpl(raw));
        byte[] content = new byte[33];
        byte[] script = "x = readPin(\"PE11\")".getBytes(StandardCharsets.US_ASCII);
        System.arraycopy(script, 0, content, 1, script.length);
        UIContext context = new UIContext();
        AtomicReference<PinoutPane> pinoutRef = new AtomicReference<>();
        List<String> scripts = new ArrayList<>();

        try {
            SwingUtilities.invokeAndWait(() -> {
                context.setOfflineMode(true);
                context.setIniFileModelForOffline(ini);
                pinoutRef.set(new PinoutPane(context, false));
                context.fireConfigImageChanged(ConfigurationImageTestHelper.newLittleEndian(content));
            });
            // the pane takes the tune on the next event
            SwingUtilities.invokeAndWait(() -> pinoutRef.get().luaScriptSource.accept(scripts::add));
            assertEquals(Collections.singletonList("x = readPin(\"PE11\")"), scripts);
        } finally {
            SwingUtilities.invokeAndWait(() -> context.getLinkManager().close());
        }
    }
}
