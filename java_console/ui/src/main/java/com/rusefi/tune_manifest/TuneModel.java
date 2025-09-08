package com.rusefi.tune_manifest;

import org.jetbrains.annotations.NotNull;
import org.json.simple.JSONObject;

public class TuneModel {
    private final String notes;
    private final String url;
    private boolean error;

    public TuneModel(String notes, String url) {
        this.notes = notes;
        this.url = url;
    }

    static TuneModel parse(JSONObject object) {
        String notes = (String) object.get("Notes");
        String url = (String) object.get("Url");
        return new TuneModel(notes, url);
    }

    public @NotNull String getSaferLocalFileName() {
        return TuneManifestHelper.trimUrlToLocalName(getUrl());
    }

    public String getNotes() {
        return notes;
    }

    public String getUrl() {
        return url;
    }

    @Override
    public String toString() {
        return "TuneModel{" +
            "notes='" + notes + '\'' +
            ", url='" + url + '\'' +
            '}';
    }

    public void setError(boolean error) {
        this.error = error;
    }

    public boolean isError() {
        return error;
    }
}
