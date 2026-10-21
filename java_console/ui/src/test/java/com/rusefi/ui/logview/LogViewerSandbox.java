package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;
import com.rusefi.logview.LogFiles;
import com.rusefi.ui.basic.UiHelper;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;

/**
 * Standalone log viewer. Without arguments it shows the bundled binary log; the argument can be
 * a log file on disk or a bundled resource path such as /logview/test-log-1.msl.
 */
public final class LogViewerSandbox {
    private static final String TITLE = "rusEFI Log Viewer";

    private LogViewerSandbox() {
    }

    public static void main(String[] args) throws IOException {
        if (args.length > 1) {
            throw new IllegalArgumentException("Usage: LogViewerSandbox [log file or resource path]");
        }
        String path = args.length == 0 ? "/logview/test-log-binary-1.mlg" : args[0];
        File file = new File(path);
        CsvLogReader log = file.isFile() ? LogFiles.read(file) : load(path);

        SwingUtilities.invokeLater(() -> {
            // the console look: FlatLaf with the rusEFI theme files [tag:console_theme]
            UiHelper.configureLaf();
            JFrame frame = new JFrame(TITLE);
            LogViewerHost host = new LogViewerHost();
            host.setTitleListener(name -> frame.setTitle(TITLE + " - " + name));
            host.showLog(log, file.isFile() ? file.getName() : fileName(path));
            frame.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
            frame.setContentPane(host);
            frame.pack();
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);
        });
    }

    private static String fileName(String path) {
        return path.substring(path.lastIndexOf('/') + 1);
    }

    static CsvLogReader load(String path) throws IOException {
        try (InputStream stream = resource(path)) {
            return LogFiles.read(stream, path);
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
