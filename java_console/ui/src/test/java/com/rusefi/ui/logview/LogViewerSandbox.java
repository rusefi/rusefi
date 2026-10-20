package com.rusefi.ui.logview;

import com.rusefi.logview.BinaryLogReader;
import com.rusefi.logview.CsvLogReader;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

/** Opens the bundled time-series log in a standalone viewer. */
public final class LogViewerSandbox {
    private LogViewerSandbox() {
    }

    public static void main(String[] args) throws IOException {
        if (args.length > 1) {
            throw new IllegalArgumentException("Usage: LogViewerSandbox [resource path]");
        }
        String path = args.length == 0 ? "/logview/test-log-binary-1.mlg" : args[0];
        CsvLogReader log = load(path);

        SwingUtilities.invokeLater(() -> {
            JFrame frame = new JFrame("rusEFI Log Viewer " + path);
            frame.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
            frame.setContentPane(new LogViewerPanel(log));
            frame.pack();
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);
        });
    }

    static CsvLogReader load(String path) throws IOException {
        try (InputStream stream = resource(path)) {
            return path.toLowerCase(java.util.Locale.ROOT).endsWith(".mlg")
                    ? BinaryLogReader.read(stream)
                    : CsvLogReader.read(new InputStreamReader(stream, StandardCharsets.ISO_8859_1));
        }
    }

    private static InputStream resource(String path) throws IOException {
        InputStream stream = LogViewerSandbox.class.getResourceAsStream(path);
        if (stream == null) {
            throw new IOException("Missing test resource: " + path);
        }
        return stream;
    }
}
