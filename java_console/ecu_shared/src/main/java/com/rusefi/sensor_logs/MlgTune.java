package com.rusefi.sensor_logs;

import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamConstants;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.XMLStreamReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/** TunerStudio's length-delimited MSQ provider in the MLVLG v2 information section. */
public final class MlgTune {
    private static final String MAIN_TUNE = "LogStart_MAIN_TUNE";
    private static final Pattern PROVIDER = Pattern.compile(
            "NEW_INFO_PROVIDER,([^,]+),Type:([^,]+?)Visible:(true|false)Length:([0-9]+)");
    private static final int MAX_INFO_SIZE = 32 * 1024 * 1024;

    private MlgTune() {
    }

    public static Path defaultOutput(Path input) {
        String name = input.getFileName().toString();
        int dot = name.lastIndexOf('.');
        return input.resolveSibling((dot > 0 ? name.substring(0, dot) : name) + ".msq");
    }

    /** Publish the original MSQ bytes only after validation; never overwrite an existing file. */
    public static Path extract(Path input, Path output) throws IOException {
        Path target = output.toAbsolutePath().normalize();
        if (input.toAbsolutePath().normalize().equals(target) || Files.exists(target)) {
            throw new IOException("Output already exists or is the input file: " + target);
        }
        byte[] tune = read(input);
        Path temporary = Files.createTempFile(target.getParent(), ".mlg-tune-", ".tmp");
        try {
            Files.write(temporary, tune);
            Files.move(temporary, target);
            return target;
        } finally {
            Files.deleteIfExists(temporary);
        }
    }

    /** Preserve the serialized MSQ bytes, including its XML encoding declaration. */
    static byte[] informationBlock(String headerText, byte[] tune) throws IOException {
        ByteArrayOutputStream info = new ByteArrayOutputStream();
        info.write(headerText.getBytes(StandardCharsets.UTF_8));
        if (tune != null) {
            info.write(("\nNEW_INFO_PROVIDER," + MAIN_TUNE + ",Type:msqVisible:falseLength:"
                    + tune.length + "\n").getBytes(StandardCharsets.US_ASCII));
            info.write(tune);
            // TS includes this terminator in dataBegin, but excludes it from Length.
            info.write(0);
        }
        return info.toByteArray();
    }

    /** Reads only the bounded information section, never scans binary sample data for XML. */
    public static byte[] read(Path log) throws IOException {
        byte[] info;
        try (RandomAccessFile file = new RandomAccessFile(log.toFile(), "r")) {
            byte[] magic = new byte[6];
            file.readFully(magic);
            if (!Arrays.equals(magic, new byte[]{'M', 'L', 'V', 'L', 'G', 0})
                    || file.readUnsignedShort() != 2) {
                throw new IOException("Only MLVLG version 2 logs are supported");
            }
            file.readInt(); // capture timestamp
            long infoStart = Integer.toUnsignedLong(file.readInt());
            long dataBegin = Integer.toUnsignedLong(file.readInt());
            file.readUnsignedShort(); // record size
            int fieldCount = file.readUnsignedShort();
            if (infoStart == 0) {
                throw new IOException("No embedded tune in log");
            }
            if (infoStart < 24L + 89L * fieldCount || dataBegin < infoStart
                    || dataBegin > file.length() || dataBegin - infoStart > MAX_INFO_SIZE) {
                throw new IOException("Invalid or oversized MLG information section");
            }
            info = new byte[(int) (dataBegin - infoStart)];
            file.seek(infoStart);
            file.readFully(info);
        }
        int offset = 0;
        while (offset < info.length) {
            int end = offset;
            while (end < info.length && info[end] != '\n' && info[end] != 0) {
                end++;
            }
            String line = new String(info, offset, end - offset, StandardCharsets.ISO_8859_1).trim();
            offset = end + 1;
            if (!line.startsWith("NEW_INFO_PROVIDER,")) {
                continue;
            }
            Matcher provider = PROVIDER.matcher(line);
            if (!provider.matches()) {
                throw new IOException("Malformed MLG information provider");
            }
            long length;
            try {
                length = Long.parseLong(provider.group(4));
            } catch (NumberFormatException failure) {
                throw new IOException("Invalid MLG provider length", failure);
            }
            if (length > info.length - offset) {
                throw new IOException("Truncated MLG information provider");
            }
            if (MAIN_TUNE.equals(provider.group(1)) && "msq".equals(provider.group(2))) {
                byte[] tune = Arrays.copyOfRange(info, offset, offset + (int) length);
                validateMsq(tune);
                return tune;
            }
            // Skip other providers by length: their contents may contain fake provider markers.
            offset += (int) length;
        }
        throw new IOException("No embedded LogStart_MAIN_TUNE in log");
    }

    private static void validateMsq(byte[] tune) throws IOException {
        XMLInputFactory factory = XMLInputFactory.newFactory();
        factory.setProperty(XMLInputFactory.SUPPORT_DTD, false);
        factory.setProperty(XMLInputFactory.IS_SUPPORTING_EXTERNAL_ENTITIES, false);
        try {
            XMLStreamReader xml = factory.createXMLStreamReader(new ByteArrayInputStream(tune));
            try {
                boolean rootSeen = false;
                while (xml.hasNext()) {
                    int event = xml.next();
                    if (event == XMLStreamConstants.DTD) {
                        throw new IOException("DTDs are not allowed in embedded tunes");
                    }
                    if (event == XMLStreamConstants.START_ELEMENT && !rootSeen) {
                        if (!"msq".equals(xml.getLocalName())) {
                            throw new IOException("Embedded tune is not an MSQ document");
                        }
                        rootSeen = true;
                    }
                }
                if (!rootSeen) {
                    throw new IOException("Empty embedded tune");
                }
            } finally {
                xml.close();
            }
        } catch (XMLStreamException failure) {
            throw new IOException("Invalid embedded MSQ XML", failure);
        }
    }
}
