package com.opensr5.ini;

import java.util.ArrayList;
import java.util.List;

/**
 * TODO: so much dead code?! do we even need this class at all? see IniFileModelImpl
 *
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/1/2017
 */
public class IniFileMetaInfoImpl implements IniFileMetaInfo {
/*
    public static final int DEFAULT_BLOCKING_FACTOR = 16000;
 */
    private final int nPages;
    private final String signature;
    private final List<String> pageReadCommands;
/*
    private final List<String> crc32CheckCommands;
    private int totalSize;
*/
    private final List<Integer> pageSizes = new ArrayList<>();
    // TS wire page identifiers (e.g. 0x0400 for the Lua page), positionally aligned with the
    // page= blocks.  May be empty for older inis without a pageIdentifier line.
    private final List<Integer> pageIdentifiers = new ArrayList<>();
    private final int ochBlockSize;

    /**
     * read maximum chunk size
     */
/*
    private final int blockingFactor; // todo: this is probably an optional property, come up with some default
*/
    public IniFileMetaInfoImpl(RawIniFile file) {

        try {
            nPages = file.getSimpleIntegerProperty("nPages", 1);
            ochBlockSize = file.getSimpleIntegerProperty("ochBlockSize");
/*
            blockingFactor = file.getSimpleIntegerProperty("blockingFactor", DEFAULT_BLOCKING_FACTOR);
*/
            signature = file.getValues("signature").get(0);

            List<String> individualPageSizes = file.getValues("pageSize");

            if (individualPageSizes.size() != nPages)
                throw new IllegalStateException("Unexpected individual sizes: " + individualPageSizes);

            for (String value : individualPageSizes) {
                int size = Integer.parseInt(value);
                pageSizes.add(size);
/*
                totalSize += size;
*/
            }

            pageReadCommands = file.getValues("pageReadCommand");
/*
            crc32CheckCommands = file.getValues("crc32CheckCommand");
 */
            // pageIdentifier is optional: older inis (and any single-page controller .ini) may
            // omit it.  When present it gives the real TS wire page id for each page block.
            try {
                for (String value : file.getValues("pageIdentifier")) {
                    pageIdentifiers.add(parsePageIdentifier(value));
                }
            } catch (MandatoryLineMissing ignored) {
                // leave pageIdentifiers empty -> getPageIdentifier() falls back to the ordinal
            }
        } catch (MandatoryLineMissing e) {
            throw new RuntimeException("While reading " + file.msg, e);
        }
    }

    @Override
    public int getnPages() {
        return nPages;
    }

    @Override
    public int getOchBlockSize() {
        return ochBlockSize;
    }

    /*
    @Override
    public int getBlockingFactor() {
        return blockingFactor;
    }

    @Override
    public int getTotalSize() {
        return totalSize;
    }
*/

    @Override
    public String getSignature() {
        return signature;
    }

    @Override
    public String getPageReadCommand(int pageIndex) {
        return pageReadCommands.get(pageIndex);
    }

    /*
    @Override
    public String getCrc32CheckCommand(int pageIndex) {
        return crc32CheckCommands.get(pageIndex);
    }
*/
    @Override
    public int getPageSize(int pageIndex) {
        return pageSizes.get(pageIndex);
    }

    @Override
    public int getPageIdentifier(int pageIndex) {
        if (pageIndex >= 0 && pageIndex < pageIdentifiers.size()) {
            return pageIdentifiers.get(pageIndex);
        }
        // Fallback for inis without a pageIdentifier line - preserves the legacy ordinal
        // behavior, which is correct for the main page (ordinal 0 == identifier 0x0000).
        return pageIndex;
    }

    /**
     * Parse a single {@code pageIdentifier} token into its 16-bit wire value.  The generator
     * emits each identifier little-endian as two {@code \xNN} byte escapes, e.g. the Lua page
     * {@code "\x00\x04"} decodes to 0x0400.  (Quotes are already stripped by the tokenizer.)
     */
    public static int parsePageIdentifier(String token) {
        String t = token.trim();
        if (t.contains("\\x")) {
            int value = 0;
            int shift = 0;
            for (String part : t.split("\\\\x")) {
                if (part.length() < 2) {
                    continue;
                }
                int b = Integer.parseInt(part.substring(0, 2), 16);
                value |= (b & 0xff) << shift; // first byte is the low byte (little-endian)
                shift += 8;
            }
            return value;
        }
        // Plain decimal/hex fallback in case a future .ini stops using \x escapes.
        return Integer.decode(t);
    }
}
