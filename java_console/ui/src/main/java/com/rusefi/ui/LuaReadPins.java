package com.rusefi.ui;

import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * The pins a Lua script reads directly with {@code readPin("PE11")}. Lua reaches every other pin through tune
 * settings ({@code luaOutputPins}, {@code luaDigitalInputPins}), which the Pinout tab already shows as used.
 * {@code readPin} takes the MCU pin name, which the console cannot map to a connector pin (the board files name
 * pins through firmware headers not in the pinout archive), so the pickers can only warn that such a pin may be
 * one of those offered.
 */
final class LuaReadPins {
    private static final Pattern BLOCK_COMMENT = Pattern.compile("--\\[(=*)\\[.*?]\\1]", Pattern.DOTALL);
    private static final Pattern LINE_COMMENT = Pattern.compile("--[^\\n]*");
    private static final Pattern CALL = Pattern.compile("\\breadPin\\s*\\(");
    private static final Pattern LITERAL_CALL = Pattern.compile("\\breadPin\\s*\\(\\s*([\"'])([^\"'\\n]*)\\1");

    private LuaReadPins() {
    }

    /** Whether the script calls {@code readPin} outside comments. */
    static boolean callsReadPin(String script) {
        return script != null && CALL.matcher(code(script)).find();
    }

    /** The pin names passed as string literals, in order, without repeats; a computed name is not listed. */
    static List<String> literalPins(String script) {
        Set<String> pins = new LinkedHashSet<>();
        if (script != null) {
            Matcher matcher = LITERAL_CALL.matcher(code(script));
            while (matcher.find()) {
                pins.add(matcher.group(2).trim());
            }
        }
        return new ArrayList<>(pins);
    }

    /**
     * The pickers' warning, e.g. "Your Lua script reads pins directly with readPin("PE11"). The console cannot tell
     * which connector pins those are: check the script does not use B6."; empty when it does not call readPin.
     */
    static String note(String script, String pinName) {
        if (!callsReadPin(script)) {
            return "";
        }
        List<String> pins = literalPins(script);
        StringBuilder names = new StringBuilder();
        for (String pin : pins) {
            names.append(names.length() == 0 ? "" : ", ").append('"').append(pin).append('"');
        }
        return "Your Lua script reads pins directly with readPin(" + names + "). The console cannot tell which "
            + "connector pins those are: check the script does not use " + pinName + ".";
    }

    /** The script without comments, so commented-out calls do not count. */
    private static String code(String script) {
        return LINE_COMMENT.matcher(BLOCK_COMMENT.matcher(script).replaceAll(" ")).replaceAll("");
    }
}
