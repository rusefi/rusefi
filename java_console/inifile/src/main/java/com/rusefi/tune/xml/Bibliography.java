package com.rusefi.tune.xml;

import javax.xml.bind.annotation.XmlAttribute;

public class Bibliography {
    private String tuneComment = null;

    @XmlAttribute
    public String getAuthor() {
        return "rusEFI";
    }

    @XmlAttribute
    public String getTuneComment() {
        return tuneComment;
    }

    public void setTuneComment(String tuneComment) {
        this.tuneComment = tuneComment;
    }

    @XmlAttribute
    public String getWriteDate() {
        return "date";
    }
}
