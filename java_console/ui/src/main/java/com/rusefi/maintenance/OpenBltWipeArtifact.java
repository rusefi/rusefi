package com.rusefi.maintenance;

import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.core.FindFileHelper;
import com.rusefi.core.io.BundleUtil;
import com.rusefi.libopenblt.file.SrecParser;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.List;
import java.util.Properties;

public class OpenBltWipeArtifact {
    private static final long APPLICATION_START = 0x08008000L;
    private static final long FLASH_1MB_END_EXCLUSIVE = 0x08100000L;
    private static final long FLASH_2MB_END_EXCLUSIVE = 0x08200000L;

    private final String bundleTarget;
    private final String bootloaderTarget;
    private final String mcuFamily;
    private final long endAddressExclusive;
    private final List<SrecParser.SRecord> segments;
    private final String reportedStationId;

    private OpenBltWipeArtifact(String bundleTarget, String bootloaderTarget, String mcuFamily,
                                long endAddressExclusive, List<SrecParser.SRecord> segments,
                                String reportedStationId) {
        this.bundleTarget = bundleTarget;
        this.bootloaderTarget = bootloaderTarget;
        this.mcuFamily = mcuFamily;
        this.endAddressExclusive = endAddressExclusive;
        this.segments = segments;
        this.reportedStationId = reportedStationId;
    }

    public static OpenBltWipeArtifact loadAndValidate(PortResult port) throws IOException {
        Profile profile = loadProfileFor(port);

        SrecParser parser = new SrecParser();
        parser.parse(new File(profile.srecName));
        if (parser.hasEmptyDataRecords()) {
            throw new IOException("Emergency wipe SREC contains an empty data record");
        }
        List<SrecParser.SRecord> segments = parser.getSegments();
        validateSegments(segments, profile.startAddress, profile.endAddressExclusive);

        return new OpenBltWipeArtifact(profile.bundleTarget, profile.bootloaderTarget, profile.mcuFamily,
            profile.endAddressExclusive, segments, profile.reportedStationId);
    }

    public static boolean isAvailableFor(PortResult port) {
        if (!FindFileHelper.hasOpenBltWipeArtifact()) {
            return false;
        }
        try {
            loadProfileFor(port);
            return true;
        } catch (IOException e) {
            return false;
        }
    }

    private static Profile loadProfileFor(PortResult port) throws IOException {
        if (port == null || port.type != SerialPortType.OpenBlt) {
            throw new IOException("Emergency wipe requires a positively detected OpenBLT port");
        }

        String manifestName = FindFileHelper.findOpenBltWipeManifest();
        if (manifestName == null) {
            throw new IOException("This bundle does not contain an emergency wipe profile");
        }

        Properties profile = new Properties();
        try (FileReader reader = new FileReader(manifestName)) {
            profile.load(reader);
        }

        if (!"1".equals(required(profile, "format"))) {
            throw new IOException("Unsupported emergency wipe profile format");
        }
        String bundleTarget = required(profile, "bundleTarget");
        String bootloaderTarget = required(profile, "bootloaderTarget");
        String mcuFamily = required(profile, "mcuFamily");
        if (!"ARCH_STM32F4".equals(mcuFamily) && !"ARCH_STM32F7".equals(mcuFamily)) {
            throw new IOException("Emergency wipe is not enabled for " + mcuFamily);
        }

        long startAddress = parseAddress(required(profile, "startAddress"));
        long endAddressExclusive = parseAddress(required(profile, "endAddressExclusive"));
        if (!isSupportedProfile(bundleTarget, bootloaderTarget, mcuFamily, startAddress, endAddressExclusive)) {
            throw new IOException(String.format("Unsupported emergency wipe range 0x%08X-0x%08X",
                startAddress, endAddressExclusive));
        }

        String reportedTarget = port.bootloaderInfo == null ? null : port.bootloaderInfo.board;
        if (reportedTarget != null) {
            if (!reportedTarget.equalsIgnoreCase(bootloaderTarget)) {
                throw new IOException("OpenBLT target " + reportedTarget
                    + " does not match wipe target " + bootloaderTarget);
            }
        } else {
            String localBundleTarget = BundleUtil.getBundleTarget();
            if (localBundleTarget == null || !localBundleTarget.equalsIgnoreCase(bundleTarget)) {
                throw new IOException("Legacy OpenBLT wipe requires the matching " + bundleTarget + " bundle");
            }
        }

        String srecName = FindFileHelper.findOpenBltWipeSrec(required(profile, "srec"));
        String reportedStationId = reportedTarget == null ? null : port.bootloaderInfo.raw;
        return new Profile(bundleTarget, bootloaderTarget, mcuFamily, startAddress, endAddressExclusive,
            srecName, reportedStationId);
    }

    private static boolean isSupportedProfile(String bundleTarget, String bootloaderTarget, String mcuFamily,
                                              long startAddress, long endAddressExclusive) {
        return startAddress == APPLICATION_START
            && ((endAddressExclusive == FLASH_1MB_END_EXCLUSIVE
                    && "uaefi".equals(bundleTarget)
                    && "uaefi".equals(bootloaderTarget)
                    && "ARCH_STM32F4".equals(mcuFamily))
                || (endAddressExclusive == FLASH_2MB_END_EXCLUSIVE
                    && "stm32f429_nucleo".equals(bundleTarget)
                    && "stm32f429_nucleo".equals(bootloaderTarget)
                    && "ARCH_STM32F4".equals(mcuFamily))
                || (endAddressExclusive == FLASH_2MB_END_EXCLUSIVE
                    && "stm32f767_nucleo".equals(bundleTarget)
                    && "stm32f767_nucleo".equals(bootloaderTarget)
                    && "ARCH_STM32F7".equals(mcuFamily)));
    }

    private static String required(Properties profile, String name) throws IOException {
        String value = profile.getProperty(name);
        if (value == null || value.trim().isEmpty()) {
            throw new IOException("Emergency wipe profile is missing " + name);
        }
        return value.trim();
    }

    private static long parseAddress(String value) throws IOException {
        try {
            String digits = value.startsWith("0x") || value.startsWith("0X") ? value.substring(2) : value;
            return Long.parseUnsignedLong(digits, 16);
        } catch (NumberFormatException e) {
            throw new IOException("Invalid emergency wipe address: " + value, e);
        }
    }

    private static void validateSegments(List<SrecParser.SRecord> segments, long startAddress,
                                         long endAddressExclusive) throws IOException {
        if (segments.size() != 1) {
            throw new IOException("Emergency wipe SREC must contain one contiguous data range");
        }

        SrecParser.SRecord segment = segments.get(0);
        if (Integer.toUnsignedLong(segment.address) != startAddress || segment.endAddress() != endAddressExclusive) {
            throw new IOException(String.format("Emergency wipe SREC range is 0x%08X-0x%08X, expected 0x%08X-0x%08X",
                Integer.toUnsignedLong(segment.address), segment.endAddress(), startAddress, endAddressExclusive));
        }
        for (byte value : segment.data) {
            if (value != (byte) 0xFF) {
                throw new IOException("Emergency wipe SREC contains data other than 0xFF");
            }
        }
    }

    List<SrecParser.SRecord> getSegments() {
        return segments;
    }

    String getReportedStationId() {
        return reportedStationId;
    }

    public String getBundleTarget() {
        return bundleTarget;
    }

    public String getBootloaderTarget() {
        return bootloaderTarget;
    }

    public String getMcuFamily() {
        return mcuFamily;
    }

    public int getFlashSizeKiB() {
        return (int) ((endAddressExclusive - 0x08000000L) / 1024);
    }

    public String confirmationMessage() {
        return "ECU: " + bundleTarget + "\n\n"
            + "This will permanently erase the ECU firmware and all internal settings, including the current tune.\n\n"
            + "The ECU will remain in recovery mode and cannot run the engine until firmware is installed again.\n"
            + "Next, run Manual OpenBLT Update. The previous tune will not be restored automatically.\n\n"
            + "SD card and external storage will not be erased.";
    }

    private static class Profile {
        final String bundleTarget;
        final String bootloaderTarget;
        final String mcuFamily;
        final long startAddress;
        final long endAddressExclusive;
        final String srecName;
        final String reportedStationId;

        Profile(String bundleTarget, String bootloaderTarget, String mcuFamily, long startAddress,
                long endAddressExclusive, String srecName, String reportedStationId) {
            this.bundleTarget = bundleTarget;
            this.bootloaderTarget = bootloaderTarget;
            this.mcuFamily = mcuFamily;
            this.startAddress = startAddress;
            this.endAddressExclusive = endAddressExclusive;
            this.srecName = srecName;
            this.reportedStationId = reportedStationId;
        }
    }
}
