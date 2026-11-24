package com.rusefi.ini.reader;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
import com.rusefi.core.IniTokenUtil;
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
        return IniTokenUtil.splitTokens(str);
    }

    public static String removeMenuAmpersand(String name) {
        if (name == null) {
            return null;
        }
        return name.replace("&", "");
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

    private static RawIniFile read(File input) throws FileNotFoundException {
        // closed at once, so the .ini can be replaced or deleted (Windows refuses while it is open)
        try (InputStream in = new FileInputStream(input)) {
            return read(in, input.getAbsolutePath());
        } catch (FileNotFoundException e) {
            throw e;
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public static @NotNull IniFileModel readIniFile(RawIniFile content, String iniFilePath, IniFileMetaInfo metaInfo) throws IniParsingException {
        final IniFileReader result = new IniFileReader(
            metaInfo,
            iniFilePath
        );
        for (RawIniFile.Line line : content.getLines()) {
            result.handleLine(line);
        }
        result.finishDialog();
        result.finishGaugeCategory();
        result.finishTable();
        result.finishCurve();
        return result.getIniFileModel();
    }

    public static IniFileModel readIniFile(String fileName) throws FileNotFoundException {
        try {
            return readIniFileChecked(fileName);
        } catch (IniParsingException e) {
            throw new RuntimeException(e);
        }
    }

    @NotNull
    public static IniFileModel readIniFileChecked(String fileName) throws FileNotFoundException, IniParsingException {
        Objects.requireNonNull(fileName, "fileName");
        log.info("Reading " + fileName);
        File input = new File(fileName);
        RawIniFile content = read(input);
        return readIniFile(content, fileName, new IniFileMetaInfoImpl(content));
    }
}
