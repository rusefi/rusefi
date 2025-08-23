package com.rusefi.tools.tune;

import com.devexperts.logging.Logging;
import com.rusefi.tune.xml.Constant;
import org.jetbrains.annotations.NotNull;

import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;

public class TuneTools {
    private static final Logging log = getLogging(TuneTools.class);

    private static String quote(String string) {
        return "\"" + string + "\"";
    }

    public static int resolveEnumByName(
        final String tsCustomLine,
        final String key,
        final Map<String, List<String>> iniDefines
    ) {
        final int magicOffset = 4; // skipping meta info header
        List<String> tokens = Arrays.stream(tsCustomLine.split(","))
            .skip(magicOffset)
            .map(String::trim)
            .collect(Collectors.toList());
        // it looks like we use syntax with dollar only with single enumeration, otherwise I do not understand
        // meaning of the ordinal, that we return from this method
        if ((tokens.size() == 1) && (tokens.get(0).startsWith("$"))) {
            final String defineName = tokens.get(0).substring(1);
            final List<String> expandedDefine = iniDefines.get(defineName);
            if (expandedDefine != null) {
                log.info(String.format("Replacing `$%s` with .ini definition %s", defineName, expandedDefine));
                tokens = expandedDefine.stream()
                    .map(TuneTools::quote)
                    .collect(Collectors.toList());
            }
        }
        final String quotedKey = quote(key);
        for (int i = 0; i < tokens.size(); i++) {
            final String token = tokens.get(i);
            if (token.equalsIgnoreCase(quotedKey))
                return i;
        }
        throw new IllegalStateException("Enum name [" + key + "] not found in " + tsCustomLine);
    }

    @NotNull
    public static String getAssignmentCode(Constant defaultValue, String parent ,String name, String value) {
        return "    // default " + defaultValue.getValue() + "\n" +
                "    " + parent + name + " = " + value + ";\n";
    }
}
