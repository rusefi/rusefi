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
}
