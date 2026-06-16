package com.rusefi.tune_manifest;

import com.rusefi.ui.basic.TuneManagementTab;
import org.json.simple.parser.ParseException;

import java.io.IOException;
import java.util.List;

/**
 * Manual harness that downloads the online tunes manifest.
 */
public class TuneManifestSandbox {
    public static void main(String[] args) throws IOException, ParseException {
        String baseUrl = TuneManagementTab.getTunesManifestUrl();

        TuneManifestHelper.downloadAllTunes(baseUrl, new TuneManifestHelper.Callback() {
            @Override
            public void onDownloaded(List<TuneModel> tunes) {

            }
        });
    }


}
