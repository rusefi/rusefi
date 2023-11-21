package com.rusefi.ui.lua;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class LuaIncludeSyntax {
    static String getScriptName(String lua) {
        Pattern p = Pattern.compile(".*--\\s*scriptname\\s*([\\S]+)", Pattern.MULTILINE);
        Matcher m = p.matcher(lua);
        if (m.find())
            return m.group(1);
        return null;
    }

    static String reloadScript(String lua, ContentSource source) {

        int index = 0;
        Pattern startPattern = Pattern.compile(".*--\\s*include\\s*([\\S]+)", Pattern.MULTILINE);
        Pattern endPattern = Pattern.compile(".*--\\s*endinclude", Pattern.MULTILINE);

        StringBuilder result = new StringBuilder();

        Matcher startMatcher = startPattern.matcher(lua);
        Matcher endMatcher = endPattern.matcher(lua);
        while (true) {
            if (!startMatcher.find(index)) {
                break;
            }
            int endOfIncludeStartIndex = startMatcher.end();

            result.append(lua.substring(index, endOfIncludeStartIndex));
            result.append("\n");

            String includeName = startMatcher.group(1);
            result.append(source.getContent(includeName));

            if (!endMatcher.find(endOfIncludeStartIndex))
                throw new IllegalStateException("No end for " + includeName);

            index = endMatcher.end();


        }

        result.append(lua.substring(index));

        return result.toString();


    }
}
