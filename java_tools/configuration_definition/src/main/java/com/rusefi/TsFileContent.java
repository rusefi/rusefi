package com.rusefi;

/**
 * 1/31/2015
 */
public class TsFileContent {
    private final String prefix;
    private final String postfix;

    public TsFileContent(String prefix, String postfix) {
        this.prefix = prefix;
        this.postfix = postfix;
    }

    public String getPrefix() {
        return prefix;
    }

    public String getPostfix() {
        return postfix;
    }
}
