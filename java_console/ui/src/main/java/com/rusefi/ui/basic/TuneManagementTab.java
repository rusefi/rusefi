package com.rusefi.ui.basic;

import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.tune_manifest.TuneManifestHelper;
import com.rusefi.tune_manifest.TuneModel;
import org.json.simple.parser.ParseException;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.util.List;

public class TuneManagementTab {
    private final JPanel content = new JPanel(new VerticalFlowLayout());
    private final JLabel status = new JLabel("Downloading tunes...");

    public TuneManagementTab(Component importTuneButton) {
        String tunesManifestUrl = getTunesManifestUrl();
        if (tunesManifestUrl != null) {
            content.add(status);
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        TuneManifestHelper.downloadAllTunes(tunesManifestUrl, new TuneManifestHelper.Callback() {
                            @Override
                            public void onDownloaded(List<TuneModel> tunes) {
                                SwingUtilities.invokeLater(() -> status.setText(tunes.size() + " tunes downloaded!"));
                            }
                        });
                    } catch (IOException | ParseException e) {
                        SwingUtilities.invokeLater(() -> status.setText("Error " + e));
                    }
                }
            }).start();
        }

        content.add(importTuneButton);
    }

    public Component getContent() {
        return content;
    }

    public static String getTunesManifestUrl() {
        return PropertiesHolder.getProperty("tunes_manifest");
    }
}
