package com.opensr5.ini;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Model for plain text help entries from [SettingContextHelp] section
 * help = referenceName, "Title"
 *   text = "line 1"
 *   text = "line 2"
 *   webHelp = "url" (optional)
 */
public class ContextHelpModel {
    private final String referenceName;
    private final String title;
    private final List<String> textLines;
    private final String webHelp;

    public ContextHelpModel(String referenceName, String title, List<String> textLines, String webHelp) {
        this.referenceName = referenceName;
        this.title = title;
        this.textLines = Collections.unmodifiableList(new ArrayList<>(textLines));
        this.webHelp = webHelp;
    }

    public String getReferenceName() {
        return referenceName;
    }

    public String getTitle() {
        return title;
    }

    public List<String> getTextLines() {
        return textLines;
    }

    public String getWebHelp() {
        return webHelp;
    }

    @Override
    public String toString() {
        return "ContextHelpModel{" +
                "referenceName='" + referenceName + '\'' +
                ", title='" + title + '\'' +
                ", textLines=" + textLines.size() +
                ", webHelp='" + webHelp + '\'' +
                '}';
    }
}
