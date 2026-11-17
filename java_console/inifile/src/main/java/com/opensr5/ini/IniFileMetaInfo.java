package com.opensr5.ini;

import java.nio.ByteOrder;

public interface IniFileMetaInfo {
/*
    int getTotalSize();
*/
    int getnPages();

/*
    int getBlockingFactor();
*/
    int getOchBlockSize();

    String getSignature();

    String getPageReadCommand(int pageIndex);

    default String getBurnCommand(int pageIndex) {
        return pageIndex == 0 ? "B" : "";
    }

    /*
    String getCrc32CheckCommand(int pageIndex);
*/
    int getPageSize(int pageIndex);

    /**
     * Wire/image byte order declared by the ini's {@code endianness} line.  Governs multi-byte
     * scalars in config pages and output channels AND the offset/count arguments of TS
     * read/write/burn commands.  rusEFI (and an omitted line) means little-endian; a PowerPC
     * controller declares {@code endianness = big}.  See {@link IniFileModel#isLittleEndian()}.
     */
    default ByteOrder getByteOrder() {
        return ByteOrder.LITTLE_ENDIAN;
    }

    /**
     * The TunerStudio wire page identifier for the given 0-based page (e.g. 0x0400 for the
     * Lua page), as declared by the ini's {@code pageIdentifier} list.  This is the value sent
     * on the wire in read/write/burn commands - it is NOT the page ordinal.  Default falls back
     * to the ordinal for inis that predate / omit the {@code pageIdentifier} line.
     */
    default int getPageIdentifier(int pageIndex) {
        return pageIndex;
    }
}
