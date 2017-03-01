package com.opensr5.io;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * (c) Andrey Belomutskiy
 * 3/1/2017
 */
public class IniFileMetaInfo {
    public static final int DEFAULT_BLOCKING_FACTOR = 16000;
    private final int nPages;

    private int totalSize;
    private final List<Integer> pageSizes = new ArrayList<>();

    /**
     * read maximum chunk size
     */
    private final int blockingFactor; // todo: this is probably an optional propery, come up with some default

    public IniFileMetaInfo(RawIniFile file) {

        nPages = file.getSimpleIntegerProperty("nPages");

        blockingFactor = file.getSimpleIntegerProperty("blockingFactor", DEFAULT_BLOCKING_FACTOR);


        RawIniFile.Line pageSize = file.getMandatoryLine("pageSize");

        List<String> individualPageSizes = Arrays.asList(pageSize.getTokens()).subList(1, pageSize.getTokens().length);

        if (individualPageSizes.size() != nPages)
            throw new IllegalStateException("Unexpected individual sizes: " + individualPageSizes);

        for (String value : individualPageSizes) {
            int size = Integer.parseInt(value);
            pageSizes.add(size);
            totalSize += size;
        }
    }

    public int getnPages() {
        return nPages;
    }

    public int getBlockingFactor() {
        return blockingFactor;
    }

    public int getTotalSize() {
        return totalSize;
    }
}
