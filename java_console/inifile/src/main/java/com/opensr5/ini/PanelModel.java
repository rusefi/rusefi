package com.opensr5.ini;

/**
 * Represents a panel reference within a dialog.
 * Panels allow dialogs to include other dialogs, table editors, curve editors, etc.
 *
 * Syntax: panel = panelName, [placement], {enableExpression}, {visibleExpression}
 *
 */
public class PanelModel {
    private final String panelName;
    private final String placement;
    private final String enableExpression;
    private final String visibleExpression;

    public PanelModel(String panelName, String placement, String enableExpression, String visibleExpression) {
        this.panelName = panelName;
        this.placement = placement;
        if (placement != null) {
            if (!isExpression(placement)) {
                if (!isValidPlacement(placement)) {
                    throw new IllegalArgumentException("Unexpected placement: " + placement);
                }
            }
        }
        this.enableExpression = enableExpression;
        this.visibleExpression = visibleExpression;
    }

    public static boolean isExpression(String string) {
        return string.trim().startsWith("{") && string.endsWith("}");
    }

    public static boolean isValidPlacement(String placement) {
        return "west".equalsIgnoreCase(placement)
            || "north".equalsIgnoreCase(placement)
            || "south".equalsIgnoreCase(placement)
            || "center".equalsIgnoreCase(placement)
            || "east".equalsIgnoreCase(placement);
    }

    public String getPanelName() {
        return panelName;
    }

    public String getPlacement() {
        return placement;
    }

    public String getEnableExpression() {
        return enableExpression;
    }

    public String getVisibleExpression() {
        return visibleExpression;
    }

    /**
     * Resolves the dialog referenced by this panel.
     * Panel names reference dialogs by their key (not UI name).
     *
     * @param model the INI file model containing all dialogs
     * @return the DialogModel referenced by this panel, or null if not found
     */
    public DialogModel resolveDialog(IniFileModel model) {
        if (panelName == null) {
            return null;
        }
        return model.getDialogs().get(panelName);
    }

    @Override
    public String toString() {
        return "PanelModel{" +
                "panelName='" + panelName + '\'' +
                ", placement='" + placement + '\'' +
                ", enableExpression='" + enableExpression + '\'' +
                ", visibleExpression='" + visibleExpression + '\'' +
                '}';
    }
}
