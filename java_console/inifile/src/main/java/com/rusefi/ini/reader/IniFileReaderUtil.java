package com.rusefi.ini.reader;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/1/2017
 */
public class IniFileReaderUtil {
    private static final Logging log = Logging.getLogging(IniFileReaderUtil.class);

    private IniFileReaderUtil() {
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
        int openedBraceCount = 0;
        StringBuilder sb = new StringBuilder();
        for (final char c: str.toCharArray()) {
            if (c == '\"' || (isTokenSeparator(c) && !inQuote && (openedBraceCount == 0))) {
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
            } else {
                sb.append(c);
                switch (c) {
                    case '{': {
                        openedBraceCount++;
                        break;
                    }
                    case '}': {
                        if (0 < openedBraceCount) {
                            openedBraceCount--;
                        } else {
                            throw new IllegalArgumentException(String.format("Unexpected closing brace: %s", str));
                        }
                        break;
                    }
                }
            }
        }
        if (sb.length() > 0)
            strings.add(sb.toString());

        return strings.toArray(new String[strings.size()]);
    }

    private static boolean isTokenSeparator(int c) {
        return c == ' ' || c == '\t' || c == '=' || c == ',';
    }

    public static RawIniFile read(InputStream in) {
        return read(in, "unknown");
    }
    /**
     * Just grabs an collection of lines, no parsing logic here
     */
    public static RawIniFile read(InputStream in, String msg) {
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
        return new RawIniFile(lines, msg);
    }

    public static RawIniFile read(File input) throws FileNotFoundException {
        InputStream in = new FileInputStream(input);
        return read(in, input.getAbsolutePath());
    }

    public static @NotNull IniFileModel readIniFile(RawIniFile content, String iniFilePath, IniFileMetaInfo metaInfo) {
        final IniFileReader result = new IniFileReader(
            metaInfo,
            iniFilePath
        );
        for (RawIniFile.Line line : content.getLines()) {
            result.handleLine(line);
        }
        result.finishDialog();
        result.finishGaugeCategory();
        return result.getIniFileModel();
    }

    @NotNull
    public static IniFileModel readIniFile(String fileName) throws FileNotFoundException {
        Objects.requireNonNull(fileName, "fileName");
        log.info("Reading " + fileName);
        File input = new File(fileName);
        RawIniFile content = read(input);
        return readIniFile(content, fileName, new IniFileMetaInfoImpl(content));
    }
}
