package com.opensr5.ini;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Predicate;

/**
 * Pure-Java FAT12 reader for the small read-only filesystem image the ECU serves
 * over the TunerStudio protocol (see {@link com.rusefi.binaryprotocol.FsImageReader}).
 * <p>
 * Ported from the rusEFI lvgl dashboard's app/fat12_reader.cpp so the console can
 * extract the embedded .ini (or rusefi.ini.7z) without any external tool. The image
 * is produced by firmware/hw_layer/mass_storage/create_ini_image.sh: 512-byte
 * sectors, two FATs, a fixed root directory, long-filename entries.
 */
public final class Fat12Reader {
    private Fat12Reader() {
    }

    public static final class FileInfo {
        public final String name;
        public final int size;
        public final int firstCluster;

        FileInfo(String name, int size, int firstCluster) {
            this.name = name;
            this.size = size;
            this.firstCluster = firstCluster;
        }
    }

    /** @return the first file whose info matches {@code pred}, or null. */
    public static byte[] readFirstMatching(byte[] image, Predicate<FileInfo> pred) {
        Layout layout = parseLayout(image);
        if (layout == null) {
            return null;
        }
        List<Integer> lfnIndices = new ArrayList<>();
        for (int i = 0; i < layout.rootEntryCount; i++) {
            int entry = layout.rootDirOffset + i * 32;
            int first = u8(image, entry);
            if (first == 0x00) {
                break;
            }
            if (first == 0xE5) {
                continue;
            }
            int attr = u8(image, entry + 11);
            if (attr == 0x0F) {
                if ((first & 0x40) != 0) {
                    lfnIndices.clear();
                }
                lfnIndices.add(i);
                continue;
            }
            if ((attr & 0x08) == 0) {
                int fileSize = le32(image, entry + 28);
                if (fileSize > 0) {
                    String name;
                    if (!lfnIndices.isEmpty()) {
                        name = lfnToString(image, layout.rootDirOffset + lfnIndices.get(0) * 32, lfnIndices.size());
                        lfnIndices.clear();
                    } else {
                        name = shortNameToString(image, entry);
                    }
                    FileInfo info = new FileInfo(name, fileSize, le16(image, entry + 26));
                    if (pred.test(info)) {
                        return readClusterChain(image, layout, info.firstCluster, info.size);
                    }
                }
            } else {
                lfnIndices.clear();
            }
        }
        return null;
    }

    /** @return the named file's bytes (case-insensitive exact match), or null. */
    public static byte[] readFile(byte[] image, String filename) {
        return readFirstMatching(image, info -> info.name.equalsIgnoreCase(filename));
    }

    private static final class Layout {
        int rootDirOffset;
        int fatOffset;
        int dataStartSector;
        int rootEntryCount;
        int sectorsPerCluster;
    }

    private static Layout parseLayout(byte[] image) {
        if (image == null || image.length < 36) {
            return null;
        }
        if (le16(image, 11) != 512) { // bytes per sector
            return null;
        }
        Layout layout = new Layout();
        layout.sectorsPerCluster = u8(image, 13);
        int reservedSectors = le16(image, 14);
        int numFats = u8(image, 16);
        layout.rootEntryCount = le16(image, 17);
        int sectorsPerFat = le16(image, 22);

        int rootDirSectors = ((layout.rootEntryCount * 32) + 511) / 512;
        int rootDirStart = reservedSectors + (numFats * sectorsPerFat);
        layout.dataStartSector = rootDirStart + rootDirSectors;
        layout.rootDirOffset = rootDirStart * 512;
        layout.fatOffset = reservedSectors * 512;
        if (layout.sectorsPerCluster <= 0 || layout.rootDirOffset + layout.rootEntryCount * 32 > image.length) {
            return null;
        }
        return layout;
    }

    private static int fat12NextCluster(byte[] image, int fatOffset, int cluster) {
        int offset = fatOffset + cluster + (cluster / 2);
        if (cluster % 2 == 0) {
            return (u8(image, offset) | (u8(image, offset + 1) << 8)) & 0xFFF;
        }
        return (u8(image, offset) >> 4) | (u8(image, offset + 1) << 4);
    }

    private static byte[] readClusterChain(byte[] image, Layout layout, int firstCluster, int fileSize) {
        byte[] out = new byte[fileSize];
        int cluster = firstCluster;
        int bytesRead = 0;
        int clusterSize = layout.sectorsPerCluster * 512;
        while (cluster < 0xFF8 && bytesRead < fileSize) {
            int dataOffset = (layout.dataStartSector + (cluster - 2) * layout.sectorsPerCluster) * 512;
            int copySize = Math.min(clusterSize, fileSize - bytesRead);
            if (dataOffset < 0 || dataOffset + copySize > image.length) {
                return null;
            }
            System.arraycopy(image, dataOffset, out, bytesRead, copySize);
            bytesRead += copySize;
            if (cluster < 0xFF0) {
                cluster = fat12NextCluster(image, layout.fatOffset, cluster);
            } else {
                break;
            }
        }
        return bytesRead == fileSize ? out : null;
    }

    private static String lfnToString(byte[] image, int firstLfnOffset, int count) {
        StringBuilder sb = new StringBuilder(count * 13);
        for (int i = count - 1; i >= 0; i--) {
            int lfn = firstLfnOffset + i * 32;
            int[] names = new int[13];
            for (int j = 0; j < 5; j++) {
                names[j] = le16(image, lfn + 1 + j * 2);
            }
            for (int j = 0; j < 6; j++) {
                names[5 + j] = le16(image, lfn + 14 + j * 2);
            }
            for (int j = 0; j < 2; j++) {
                names[11 + j] = le16(image, lfn + 28 + j * 2);
            }
            for (int c : names) {
                if (c == 0 || c == 0xFFFF) {
                    return sb.toString();
                }
                sb.append((char) (c & 0xFF));
            }
        }
        return sb.toString();
    }

    private static String shortNameToString(byte[] image, int entry) {
        char[] name = new char[8];
        int dot = 0;
        for (; dot < 8; dot++) {
            char c = (char) u8(image, entry + dot);
            if (c == ' ') {
                break;
            }
            name[dot] = c;
        }
        StringBuilder sb = new StringBuilder();
        sb.append(name, 0, dot);
        StringBuilder ext = new StringBuilder();
        for (int i = 0; i < 3; i++) {
            char c = (char) u8(image, entry + 8 + i);
            if (c == ' ') {
                break;
            }
            ext.append(c);
        }
        if (ext.length() > 0) {
            sb.append('.').append(ext);
        }
        return sb.toString();
    }

    private static int u8(byte[] b, int o) {
        return b[o] & 0xFF;
    }

    private static int le16(byte[] b, int o) {
        return u8(b, o) | (u8(b, o + 1) << 8);
    }

    private static int le32(byte[] b, int o) {
        return u8(b, o) | (u8(b, o + 1) << 8) | (u8(b, o + 2) << 16) | (u8(b, o + 3) << 24);
    }
}
