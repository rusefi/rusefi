package com.rusefi.tools.openblt;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public final class OpenBltWipeSrecGenerator {
    static final long APPLICATION_START = 0x08008000L;
    static final int RECORD_DATA_SIZE = 240;
    private static final long MAX_ADDRESS_EXCLUSIVE = 0x1_0000_0000L;
    private static final byte[] EMPTY_DATA = new byte[0];
    private static final byte[] HEADER = "rusEFI OpenBLT wipe".getBytes(StandardCharsets.US_ASCII);
    // see comment on appendByte for this
    private static final char[] HEX = "0123456789ABCDEF".toCharArray();

    private OpenBltWipeSrecGenerator() {
    }

    public static void main(String[] args) throws IOException {
        Map<String, String> options = parseArgs(args);
        generate(
            Paths.get(options.get("--output")),
            Paths.get(options.get("--manifest")),
            parseAddress(options.get("--start")),
            parseAddress(options.get("--end-exclusive")),
            options.get("--bundle-target"),
            options.get("--bootloader-target"),
            options.get("--mcu-family")
        );
    }

    static void generate(Path output, Path manifest, long start, long endExclusive,
                         String bundleTarget, String bootloaderTarget, String mcuFamily) throws IOException {
        if (!isSupportedProfile(bundleTarget, bootloaderTarget, mcuFamily, start, endExclusive)) {
            throw new IllegalArgumentException(String.format(
                "Unsupported wipe profile %s/%s/%s ending at 0x%08X",
                bundleTarget, bootloaderTarget, mcuFamily, endExclusive));
        }

        Path normalizedOutput = output.toAbsolutePath().normalize();
        Path normalizedManifest = manifest.toAbsolutePath().normalize();
        Path outputParent = normalizedOutput.getParent();
        Path manifestParent = normalizedManifest.getParent();
        if (outputParent == null || !outputParent.equals(manifestParent)
            || normalizedOutput.equals(normalizedManifest)) {
            throw new IllegalArgumentException("SREC and manifest must be in the same directory");
        }

        Files.createDirectories(outputParent);
        generateSrec(normalizedOutput, start, endExclusive);
        writeManifest(normalizedManifest, normalizedOutput.getFileName().toString(), start, endExclusive,
            bundleTarget, bootloaderTarget, mcuFamily);
    }

    static void generateSrec(Path output, long start, long endExclusive) throws IOException {
        if (start < 0 || start >= endExclusive || endExclusive > MAX_ADDRESS_EXCLUSIVE) {
            throw new IllegalArgumentException(String.format(
                "Invalid SREC range 0x%08X-0x%08X", start, endExclusive));
        }

        long recordCount = (endExclusive - start + RECORD_DATA_SIZE - 1) / RECORD_DATA_SIZE;
        if (recordCount > 0xFFFF) {
            throw new IllegalArgumentException("SREC contains too many data records for an S5 record");
        }

        byte[] fill = new byte[RECORD_DATA_SIZE];
        Arrays.fill(fill, (byte) 0xFF);
        try (BufferedWriter writer = Files.newBufferedWriter(output, StandardCharsets.US_ASCII)) {
            writeRecord(writer, '0', 0, 2, HEADER, HEADER.length);
            for (long address = start; address < endExclusive; address += RECORD_DATA_SIZE) {
                int size = (int) Math.min(RECORD_DATA_SIZE, endExclusive - address);
                writeRecord(writer, '3', address, 4, fill, size);
            }
            writeRecord(writer, '5', recordCount, 2, EMPTY_DATA, 0);
            writeRecord(writer, '7', start, 4, EMPTY_DATA, 0);
        }
    }

    private static void writeManifest(Path manifest, String srecName, long start, long endExclusive,
                                      String bundleTarget, String bootloaderTarget, String mcuFamily)
        throws IOException {
        try (BufferedWriter writer = Files.newBufferedWriter(manifest, StandardCharsets.US_ASCII)) {
            writer.write("format=1\n");
            writer.write("bundleTarget=" + bundleTarget + "\n");
            writer.write("bootloaderTarget=" + bootloaderTarget + "\n");
            writer.write("mcuFamily=" + mcuFamily + "\n");
            writer.write(String.format("startAddress=0x%08X\n", start));
            writer.write(String.format("endAddressExclusive=0x%08X\n", endExclusive));
            writer.write("srec=" + srecName + "\n");
        }
    }

    private static void writeRecord(BufferedWriter writer, char type, long address, int addressSize,
                                    byte[] data, int dataLength) throws IOException {
        long maxAddress = addressSize == 4 ? 0xFFFFFFFFL : (1L << (addressSize * 8)) - 1;
        if (address < 0 || address > maxAddress || dataLength < 0 || dataLength > data.length) {
            throw new IllegalArgumentException("Invalid S-record data");
        }

        int count = addressSize + dataLength + 1;
        int checksum = count;
        StringBuilder line = new StringBuilder(4 + addressSize * 2 + dataLength * 2 + 2);
        line.append('S').append(type);
        appendByte(line, count);
        for (int shift = (addressSize - 1) * 8; shift >= 0; shift -= 8) {
            int value = (int) (address >> shift) & 0xFF;
            appendByte(line, value);
            checksum += value;
        }
        for (int i = 0; i < dataLength; i++) {
            int value = data[i] & 0xFF;
            appendByte(line, value);
            checksum += value;
        }
        appendByte(line, (~checksum) & 0xFF);
        line.append('\n');
        writer.write(line.toString());
    }

    private static void appendByte(StringBuilder output, int value) {
        // String.format("%02X", value) is too slow for generating a whole file, so we are using a custom implementation
        output.append(HEX[(value >> 4) & 0xF]).append(HEX[value & 0xF]);
    }

    private static boolean isSupportedProfile(String bundleTarget, String bootloaderTarget, String mcuFamily,
                                              long start, long endExclusive) {
        if (start != APPLICATION_START) {
            return false;
        }
        return (endExclusive == 0x08100000L
                && "uaefi".equals(bundleTarget)
                && "uaefi".equals(bootloaderTarget)
                && "ARCH_STM32F4".equals(mcuFamily))
            || (endExclusive == 0x08200000L
                && "stm32f429_nucleo".equals(bundleTarget)
                && "stm32f429_nucleo".equals(bootloaderTarget)
                && "ARCH_STM32F4".equals(mcuFamily))
            || (endExclusive == 0x08200000L
                && "stm32f767_nucleo".equals(bundleTarget)
                && "stm32f767_nucleo".equals(bootloaderTarget)
                && "ARCH_STM32F7".equals(mcuFamily));
    }

    private static long parseAddress(String value) {
        if (value.startsWith("0x") || value.startsWith("0X")) {
            return Long.parseUnsignedLong(value.substring(2), 16);
        }
        return Long.parseLong(value);
    }

    private static Map<String, String> parseArgs(String[] args) {
        if (args.length % 2 != 0) {
            throw usage();
        }

        Map<String, String> options = new HashMap<>();
        for (int i = 0; i < args.length; i += 2) {
            String name = args[i];
            if (!isKnownOption(name) || options.put(name, args[i + 1]) != null) {
                throw usage();
            }
        }
        String[] required = {"--output", "--manifest", "--start", "--end-exclusive",
            "--bundle-target", "--bootloader-target", "--mcu-family"};
        for (String name : required) {
            if (!options.containsKey(name) || options.get(name).isEmpty()) {
                throw usage();
            }
        }
        return options;
    }

    private static boolean isKnownOption(String name) {
        return "--output".equals(name) || "--manifest".equals(name) || "--start".equals(name)
            || "--end-exclusive".equals(name) || "--bundle-target".equals(name)
            || "--bootloader-target".equals(name) || "--mcu-family".equals(name);
    }

    private static IllegalArgumentException usage() {
        return new IllegalArgumentException("Usage: --output <srec> --manifest <properties> --start <address> "
            + "--end-exclusive <address> --bundle-target <target> --bootloader-target <target> "
            + "--mcu-family <family>");
    }
}
