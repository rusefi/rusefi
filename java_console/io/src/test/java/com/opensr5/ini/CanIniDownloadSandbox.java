package com.opensr5.ini;

import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.binaryprotocol.FsImageReader;
import com.rusefi.core.SignatureHelper;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.can.PCanHelper;
import com.rusefi.io.can.PCanIoStream;
import com.rusefi.io.can.SLCANConnector;
import com.rusefi.io.can.SocketCANIoStream;
import com.rusefi.io.can.slcan.SlcanPortScanner;
import com.rusefi.ini.reader.IniFileReaderUtil;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import java.util.zip.GZIPInputStream;

/**
 * Manual harness: downloads the .ini embedded in the ECU's mass-storage filesystem image over CAN
 * (TS READ32 of {@link FsImageReader#TS_PAGE_FS_IMAGE_DATA}) and places it into both the rusEFI
 * console .ini cache ({@code ~/.rusEFI/ini_database}) and TunerStudio's ecuDef cache.
 * <p>
 * The image is a FAT filesystem, optionally gzip-wrapped (create_ini_image_compressed.sh), holding
 * either the plain .ini or rusefi.ini.7z (create_ini_image.sh). FAT and 7z extraction use the
 * {@code 7z} command line tool, which must be on PATH (pixi provides it).
 * <p>
 * Usage: {@code CanIniDownloadSandbox [auto | PCAN | SLCAN:<serial port> | SocketCAN] [image-file]}
 * <br>default transport is auto, see {@link #autoDetect()}; with an image-file argument the CAN download is skipped and that
 * previously saved image (raw or gzip) is processed instead.
 *
 * @see PrimeTunerStudioCacheSandbox
 */
public class CanIniDownloadSandbox {
    private static final int SLCAN_BITRATE_500K = 6;
    private static final String AUTO = "auto";

    public static void main(String[] args) throws Exception {
        String port = args.length > 0 ? args[0] : AUTO;
        Path workDir = Files.createTempDirectory("can_ini_download");
        System.out.println("Working folder " + workDir);

        String ecuSignature;
        byte[] image;
        if (args.length > 1) {
            ecuSignature = null;
            image = Files.readAllBytes(new File(args[1]).toPath());
            System.out.println("Using saved image " + args[1] + ", " + image.length + " bytes");
        } else {
            LinkManager linkManager = new LinkManager();
            IoStream stream = AUTO.equalsIgnoreCase(port) ? autoDetect() : openCanStream(port);
            try {
                ecuSignature = SerialAutoChecker.checkResponse(stream, null);
                System.out.println("ECU signature [" + ecuSignature + "]");
                if (ecuSignature == null) {
                    throw new IllegalStateException("No rusEFI HELLO response via " + port);
                }
                BinaryProtocol bp = new BinaryProtocol(linkManager, stream);
                long start = System.currentTimeMillis();
                image = (byte[]) linkManager.COMMUNICATION_EXECUTOR
                        .submit(() -> FsImageReader.readImage(bp))
                        .get(5, TimeUnit.MINUTES);
                System.out.println("Got CRC-validated FS image, " + image.length + " bytes in "
                        + (System.currentTimeMillis() - start) + "ms");
            } finally {
                stream.close();
                linkManager.close();
            }
            Files.write(workDir.resolve("fs_image.bin"), image);
        }

        File iniFile = extractIni(maybeGunzip(image), workDir);
        String iniSignature = IniLocator.readIniSignature(iniFile);
        System.out.println("Extracted " + iniFile + " " + iniFile.length() + " bytes, signature [" + iniSignature + "]");
        if (iniSignature == null) {
            throw new IllegalStateException("No signature in " + iniFile);
        }
        if (ecuSignature != null && !ecuSignature.equals(iniSignature)) {
            throw new IllegalStateException("ECU signature [" + ecuSignature + "] does not match image .ini [" + iniSignature + "]");
        }

        String consoleCacheFile = SignatureHelper.importIntoCache(iniSignature, iniFile);
        if (consoleCacheFile == null) {
            System.out.println("Console cache: signature not parsable, skipped");
        } else {
            System.out.println("Console cache: " + consoleCacheFile);
        }

        IniFileModel iniFileModel = IniFileReaderUtil.readIniFile(iniFile.getAbsolutePath());
        PrimeTunerStudioCache.prime(iniFileModel, iniFile.getAbsolutePath());
        String tsCacheFile = PrimeTunerStudioCache.findInTunerStudioCache(iniSignature);
        System.out.println("TunerStudio cache: " + (tsCacheFile == null ? "not available" : tsCacheFile));

        System.exit(0);
    }

    /**
     * Same transport order as the console's EcuHardwareProbes (which lives in ecu_shared, above
     * this module): SocketCAN on Linux, PCAN where its native API loads (Windows PCANBasic.dll,
     * macOS MacCAN), then every serial port SlcanPortScanner recognizes as an SLCAN adapter.
     * The first transport on which a rusEFI ECU answers HELLO wins.
     */
    private static IoStream autoDetect() {
        List<String> candidates = new ArrayList<>();
        if (System.getProperty("os.name").toLowerCase().contains("linux")) {
            candidates.add(LinkManager.SOCKET_CAN);
        }
        if (PCanHelper.isNativeApiAvailable()) {
            candidates.add(LinkManager.PCAN);
        } else {
            System.out.println("PCAN: native API not available");
        }
        for (SlcanPortScanner.Result result : SlcanPortScanner.scanOnce(SlcanPortScanner.Probes.REAL)) {
            System.out.println("Serial " + result);
            if (result.type == SlcanPortScanner.Type.SLCAN) {
                candidates.add(LinkManager.SLCAN_PREFIX + result.port);
            }
        }
        for (String candidate : candidates) {
            IoStream stream;
            try {
                stream = openCanStream(candidate);
            } catch (Throwable e) {
                // LinkageError for a missing native library, IOException for an unavailable adapter
                System.out.println(candidate + ": " + e);
                continue;
            }
            String signature = SerialAutoChecker.checkResponse(stream, null);
            if (signature != null && signature.startsWith(Integration.PROTOCOL_SIGNATURE_PREFIX)) {
                System.out.println(candidate + ": rusEFI ECU [" + signature + "]");
                return stream;
            }
            System.out.println(candidate + ": adapter opened, no rusEFI ECU answered");
            stream.close();
        }
        throw new IllegalStateException("No rusEFI ECU found via any CAN adapter, tried " + candidates);
    }

    private static IoStream openCanStream(String port) throws IOException {
        IoStream stream;
        if (LinkManager.PCAN.equals(port)) {
            stream = PCanIoStream.createStream();
        } else if (LinkManager.isSlcanPort(port)) {
            stream = SLCANConnector.createStream(port.substring(LinkManager.SLCAN_PREFIX.length()), SLCAN_BITRATE_500K);
        } else if (LinkManager.SOCKET_CAN.equals(port)) {
            stream = SocketCANIoStream.createStream();
        } else {
            throw new IllegalArgumentException("Not a CAN port: " + port + ", expected " + AUTO + ", " + LinkManager.PCAN
                    + ", " + LinkManager.SLCAN_PREFIX + "<serial port> or " + LinkManager.SOCKET_CAN);
        }
        if (stream == null) {
            throw new IOException("Error opening " + port);
        }
        return stream;
    }

    private static byte[] maybeGunzip(byte[] image) throws IOException {
        if (image.length < 2 || (image[0] & 0xff) != 0x1f || (image[1] & 0xff) != 0x8b) {
            return image;
        }
        try (InputStream in = new GZIPInputStream(new ByteArrayInputStream(image))) {
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            byte[] buffer = new byte[32 * 1024];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            System.out.println("Gunzipped image to " + out.size() + " bytes");
            return out.toByteArray();
        }
    }

    private static File extractIni(byte[] fatImage, Path workDir) throws IOException, InterruptedException {
        Path imageFile = workDir.resolve("ramdisk.image");
        Files.write(imageFile, fatImage);
        Path fsDir = workDir.resolve("fs");
        sevenZipExtract(imageFile, fsDir);

        List<File> files = listFiles(fsDir);
        File ini = findBySuffix(files, ".ini");
        if (ini != null) {
            return ini;
        }
        File archive = findBySuffix(files, ".ini.7z");
        if (archive == null) {
            throw new IllegalStateException("Neither .ini nor .ini.7z in FS image: " + files);
        }
        Path iniDir = workDir.resolve("ini");
        sevenZipExtract(archive.toPath(), iniDir);
        ini = findBySuffix(listFiles(iniDir), ".ini");
        if (ini == null) {
            throw new IllegalStateException("No .ini inside " + archive);
        }
        return ini;
    }

    private static void sevenZipExtract(Path archive, Path outputDir) throws IOException, InterruptedException {
        Process process;
        try {
            process = new ProcessBuilder("7z", "x", "-y", "-o" + outputDir, archive.toString())
                    .redirectErrorStream(true)
                    .redirectOutput(workLog(outputDir))
                    .start();
        } catch (IOException e) {
            throw new IOException("Unable to run 7z, is it on PATH? " + e.getMessage(), e);
        }
        int exitCode = process.waitFor();
        if (exitCode != 0) {
            throw new IllegalStateException("7z failed with " + exitCode + " on " + archive + ", see " + workLog(outputDir));
        }
    }

    private static File workLog(Path outputDir) {
        return new File(outputDir + ".7z.log");
    }

    private static List<File> listFiles(Path dir) throws IOException {
        try (Stream<Path> paths = Files.walk(dir)) {
            return paths.filter(Files::isRegularFile)
                    .map(Path::toFile)
                    .sorted(Comparator.comparing(File::getName))
                    .collect(Collectors.toList());
        }
    }

    private static File findBySuffix(List<File> files, String suffix) {
        return files.stream()
                .filter(f -> f.getName().toLowerCase().endsWith(suffix))
                .findFirst()
                .orElse(null);
    }
}
