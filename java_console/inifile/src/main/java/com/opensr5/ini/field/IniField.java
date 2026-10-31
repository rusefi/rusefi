package com.opensr5.ini.field;

import java.util.Objects;

public abstract class IniField {
    private final String name;
    // offset within binary page
    private final int offset;
    // TunerStudio protocol page identifier (0x0000 = main settings page, 0x0100 scatter,
    // 0x0200 LTFT, 0x0300 second tables, 0x0400 Lua - see firmware tunerstudio.h).  This is
    // the value sent on the wire in read/write/burn commands, NOT the ini's 1-based page
    // ordinal.  Set by IniFileReader.registerField from the ini's pageIdentifier list.
    private int pageIndex = 0;

    public IniField(String name, int offset) {
        this.name = name;
        this.offset = offset;
    }

    public int getPageIndex() {
        return pageIndex;
    }

    public void setPageIndex(int pageIndex) {
        this.pageIndex = pageIndex;
    }

    /**
     * Human-friendly 1-based TS page for user messages/logs. {@link #pageIndex} holds the TS wire
     * identifier whose high byte is the page ordinal (0x0400 -> Lua page 5).
     */
    public int getDisplayPage() {
        return toDisplayPage(pageIndex);
    }

    public static int toDisplayPage(int pageIndexOrIdentifier) {
        int ordinal = pageIndexOrIdentifier >= 0x100 ? (pageIndexOrIdentifier >> 8) : pageIndexOrIdentifier;
        return ordinal + 1;
    }

    public static double parseDouble(String s) {
        // todo: real implementation
        // TODO: replace with new ExpressionEvaluator
        s = s.replace("{", "").replace("}", "").trim();
        // If this is a complex expression with ternary operator, try to extract the true branch
        // this is related to the lambdaTable using the true branch as default on the fuel tests
        // [tag:lambdaTable]
        if (s.contains("?")) {
            int questionIndex = s.indexOf('?');
            int colonIndex = s.lastIndexOf(':');
            if (questionIndex >= 0 && colonIndex > questionIndex) {
                s = s.substring(questionIndex + 1, colonIndex).trim();
                // Recursively parse the true branch
                return parseDouble(s);
            }
            // If we can't extract a true branch, return default
            return 1.0;
        }
        int dividerIndex = s.indexOf('/');
        if (dividerIndex != -1) {
            return Double.parseDouble(s.substring(0, dividerIndex).trim()) / Double.parseDouble(s.substring(dividerIndex + 1).trim());
        } else {
            try {
                return Double.parseDouble(s);
            } catch (NumberFormatException e) {
                return 0;
            }
        }
    }

    public String getName() {
        return name;
    }

    public String getUnits() {
        return null;
    }

    public String getDigits() {
        return null;
    }

    public static int parseDigits(String digits) {
        try {
            return Integer.parseInt(digits);
        } catch (NumberFormatException e) {
            return 3;
        }
    }

    public int getOffset() {
        return offset;
    }

    public abstract int getSize();

    public abstract <T> T accept(IniFieldVisitor<T> visitor);

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        IniField iniField = (IniField) o;
        return getOffset() == iniField.getOffset() && Objects.equals(getName(), iniField.getName());
    }

    @Override
    public int hashCode() {
        return Objects.hash(getName(), getOffset());
    }

    @Override
    public String toString() {
        return "IniField{" +
            "name='" + name + '\'' +
            ", offset=" + offset +
            '}';
    }
}
