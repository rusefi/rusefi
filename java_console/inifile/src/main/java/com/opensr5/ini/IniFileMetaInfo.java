package com.opensr5.ini;

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

    /*
    String getCrc32CheckCommand(int pageIndex);
*/
    int getPageSize(int pageIndex);

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
