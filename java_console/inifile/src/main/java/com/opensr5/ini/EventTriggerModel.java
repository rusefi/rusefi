package com.opensr5.ini;

/**
 * Represents a single {@code triggeredPageRefresh} entry from the {@code [EventTriggers]}
 * INI section. When the expression transitions from false (0) to true (non-zero), the
 * specified page should be re-read from the ECU.
 */
public class EventTriggerModel {
    private final int page;
    private final String expression;

    public EventTriggerModel(int page, String expression) {
        this.page = page;
        this.expression = expression;
    }

    /** TunerStudio page number (1-based). */
    public int getPage() {
        return page;
    }

    /** The expression string, e.g. {@code { triggerPageRefreshFlag }}. */
    public String getExpression() {
        return expression;
    }
}
