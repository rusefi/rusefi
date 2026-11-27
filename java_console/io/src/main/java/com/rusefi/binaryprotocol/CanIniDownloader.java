package com.rusefi.binaryprotocol;

import com.opensr5.ini.Fat12Reader;
import org.apache.commons.compress.archivers.sevenz.SevenZArchiveEntry;
import org.apache.commons.compress.archivers.sevenz.SevenZFile;
import org.apache.commons.compress.utils.SeekableInMemoryByteChannel;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.GZIPInputStream;

/**
 * Pulls the TunerStudio .ini out of the filesystem image the ECU serves over the
 * protocol (issue #10349). Used as a fallback when the console cannot locate a
 * matching .ini through the usual paths - works over any transport, including CAN.
 * <p>
 * Pure-Java, no external tools (unlike the lvgl dashboard which shells out to 7z):
 * {@link FsImageReader} reads the raw image, then this optionally gunzips it,
 * parses the FAT12 with {@link Fat12Reader}, and LZMA2-decompresses the inner
 * rusefi.ini.7z with Apache Commons Compress. See
 * firmware/hw_layer/mass_storage/create_ini_image.sh for the image format.
 */
public final class CanIniDownloader {
    private CanIniDownloader() {
    }

    /**
     * Must be invoked on the {@link com.rusefi.io.LinkManager} communication thread.
     *
     * @return the decompressed .ini bytes
     * @throws IOException on any protocol, filesystem or decompression failure
     */
    public static byte[] downloadIni(BinaryProtocol bp) throws IOException {
        return downloadIni(bp, FsImageReader.ProgressListener.NONE);
    }

    public static byte[] downloadIni(BinaryProtocol bp, FsImageReader.ProgressListener progress) throws IOException {
        byte[] image = FsImageReader.readImage(bp, progress);
        return extractIni(image);
    }

    /** Extracts the .ini from a raw (possibly gzip-wrapped) filesystem image. */
    public static byte[] extractIni(byte[] image) throws IOException {
        byte[] fat = maybeGunzip(image);

        byte[] iniOrArchive = Fat12Reader.readFirstMatching(fat, info -> {
            String lower = info.name.toLowerCase();
            return lower.endsWith(".ini") || lower.endsWith(".ini.7z");
        });
        if (iniOrArchive == null) {
            throw new IOException("No .ini or .ini.7z found in the ECU filesystem image");
        }

        if (isSevenZ(iniOrArchive)) {
            return un7z(iniOrArchive);
        }
        return iniOrArchive; // plain .ini
    }

    private static boolean isSevenZ(byte[] data) {
        // 7z signature: 37 7A BC AF 27 1C
        return data.length >= 6 && (data[0] & 0xff) == 0x37 && (data[1] & 0xff) == 0x7A
                && (data[2] & 0xff) == 0xBC && (data[3] & 0xff) == 0xAF
                && (data[4] & 0xff) == 0x27 && (data[5] & 0xff) == 0x1C;
    }

    private static byte[] un7z(byte[] archive) throws IOException {
        try (SevenZFile sevenZ = new SevenZFile(new SeekableInMemoryByteChannel(archive))) {
            SevenZArchiveEntry entry = sevenZ.getNextEntry();
            while (entry != null) {
                if (!entry.isDirectory() && entry.getName().toLowerCase().endsWith(".ini")) {
                    ByteArrayOutputStream out = new ByteArrayOutputStream((int) Math.max(entry.getSize(), 1024));
                    byte[] buffer = new byte[32 * 1024];
                    int read;
                    while ((read = sevenZ.read(buffer)) != -1) {
                        out.write(buffer, 0, read);
                    }
                    return out.toByteArray();
                }
                entry = sevenZ.getNextEntry();
            }
        }
        throw new IOException("No .ini inside the rusefi.ini.7z archive");
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
            return out.toByteArray();
        }
    }
}
