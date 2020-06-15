package com.opensr5.ini;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/1/2017
 */
public class IniFileReader {
    private IniFileReader() {
    }

    /**
     * <pre>
     * This method reads
     *    x = y, "Z"
     * lines returning
     *  {x, y, "Z"}
     * array of Strings
     *
     * equals sign, space, tab and commas are all equally considered not parts of the token, quotes are supported
     *
     * Technically a line like
     *     x, y = z
     * is also supported and would produce same valid output
     *
     * </pre>
     */
    public static String[] splitTokens(String str) {
        ArrayList<String> strings = new ArrayList<>();
        boolean inQuote = false;
        boolean hadQuote = false;
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            if (c == '\"' || isTokenSeparator(c) && !inQuote) {
                if (c == '\"') {
                    inQuote = !inQuote;
                    if (!inQuote) {
                        // we are here when we close quotes
                        hadQuote = true;
                    }
                }
                if (!inQuote && sb.length() > 0) {
                    strings.add(sb.toString());
                    sb.delete(0, sb.length());
                    hadQuote = false;
                } else if (hadQuote) {
                    strings.add("");
                    hadQuote = false;
                }
            } else
                sb.append(c);
        }
        if (sb.length() > 0)
            strings.add(sb.toString());

        return strings.toArray(new String[strings.size()]);
    }

    private static boolean isTokenSeparator(int c) {
        return c == ' ' || c == '\t' || c == '=' || c == ',';
    }

    /**
     * Just grabs an collection of lines, no parsing logic here
     */
    public static RawIniFile read(InputStream in) {
        List<RawIniFile.Line> lines = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new InputStreamReader(in));

        try {
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.trim().isEmpty()) {
                    continue;
                }
                lines.add(new RawIniFile.Line(line));
            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
        return new RawIniFile(lines);
    }

    public static RawIniFile read(File input) {
        try {
            InputStream in = new FileInputStream(input);
            return read(in);
        } catch (FileNotFoundException e) {
            throw new IllegalStateException(e);
        }
    }
}
