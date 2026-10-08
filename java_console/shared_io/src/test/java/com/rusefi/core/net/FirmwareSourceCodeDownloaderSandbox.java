package com.rusefi.core.net;

import java.nio.file.Path;
import java.nio.file.Paths;

/** CLI: ./gradlew :shared_io:runFirmwareSourceCodeDownloaderSandbox [--args='/path/to/cache'] */
public final class FirmwareSourceCodeDownloaderSandbox {
    public static void main(String[] args) throws Exception {
        if (args.length > 1) {
            throw new IllegalArgumentException("Usage: FirmwareSourceCodeDownloaderSandbox [cache-directory]");
        }
        FirmwareSourceCodeDownloader downloader = args.length == 0 ? new FirmwareSourceCodeDownloader()
                : new FirmwareSourceCodeDownloader(Paths.get(args[0]));
        System.out.println("Preparing firmware source from " + FirmwareSourceCodeDownloader.DOWNLOAD_URL);
        Path directory = downloader.download(percent -> System.out.println("Firmware source: " + percent + "%"));
        System.out.println("Firmware source ready: " + directory);
    }
}
