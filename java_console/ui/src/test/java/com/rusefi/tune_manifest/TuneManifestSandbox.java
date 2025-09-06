package com.rusefi.tune_manifest;

import com.rusefi.core.net.PropertiesHolder;
import org.json.simple.parser.ParseException;

import java.io.IOException;
import java.util.List;

public class TuneManifestSandbox {
    public static void main(String[] args) throws IOException, ParseException {
        String baseUrl = PropertiesHolder.getProperty("tunes_manifest");

        String localManifest = TuneManifestHelper.downloadFile(baseUrl, baseUrl + TuneManifestHelper.MANIFEST_FILE_NAME, TuneManifestHelper.MANIFEST_FILE_NAME);


        List<TuneModel> tunes = TuneManifestHelper.parseManifest(localManifest);

        for (TuneModel t : tunes) {
            System.out.println(t.toString());

            String localTuneName = TuneManifestHelper.trimUrlToLocalName(t.getUrl());

            TuneManifestHelper.downloadFile(baseUrl, t.getUrl(), localTuneName);
        }
    }
}
