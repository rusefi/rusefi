package com.rusefi.tune_manifest;

import org.jetbrains.annotations.NotNull;
import org.json.simple.JSONObject;

public class TuneModel {
    private final String notes;
    private final int size;
    private final String url;
    private final String mapId;
    private final String fileName;
    private boolean error;

    public TuneModel(String notes, String url, int size, String mapId, String fileName) {
        this.notes = notes;
        this.size = size;
        this.url = url;
        this.mapId = mapId;
        this.fileName = fileName;
    }

    static TuneModel parse(JSONObject object) {
        String mapId = (String) object.get("MapID");
        String notes = (String) object.get("Notes");
        String url = (String) object.get("Url");
        int size = Integer.valueOf((String)object.get("EngineSize"));
        String fileName = (String) object.get("FileName");
        return new TuneModel(notes, url, size, mapId, fileName);
    }

    public @NotNull String getSaferLocalFileName() {
        return TuneManifestHelper.trimUrlToLocalName(getUrl());
    }

    public String getNotes() {
        return notes;
    }

    public String getFileName() {
        return fileName;
    }

    public int getSize() {
        return size;
    }

    public String getMapId() {
        return mapId;
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
