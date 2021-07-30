package com.rusefi.ui.util;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

public class URLLabel extends JLabel {
    private String text;

    public URLLabel(String text, URI uri) {
        setup(text, uri);
    }

    public URLLabel(String uri) {
        this(uri, uri);
    }

    public URLLabel(String text, String uri) {
        setup(text, createUri(uri));
    }

    public static URI createUri(String uri) {
        URI oURI;
        try {
            oURI = new URI(uri);
        } catch (URISyntaxException e) {
            // converts to runtime exception for ease of use
            // if you cannot be sure at compile time that your
            // uri is valid, construct your uri manually and
            // use the other constructor.
            throw new RuntimeException(e);
        }
        return oURI;
    }

    public static void open(String url) {
        try {
            open(new URI(url));
        } catch (URISyntaxException e) {
            throw new IllegalStateException(e);
        }
    }

    public void setup(String t, final URI u) {
        setCursor(new Cursor(Cursor.HAND_CURSOR));
        text = t;
        setText(text);
        setToolTipText(u.toString());
        addUrlAction(this, u);
        addMouseListener(new MouseAdapter() {

            public void mouseEntered(MouseEvent e) {
                setText(text, false);
            }

            public void mouseExited(MouseEvent e) {
                setText(text, true);
            }
        });
    }

    public static void addUrlAction(JComponent component, final URI u) {
        component.addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                open(u);
            }
        });
    }

    @Override
    public void setText(String text) {
        setText(text, true);
    }

    public void setText(String text, boolean ul) {
        String link = ul ? "<u>" + text + "</u>" : text;
        super.setText("<html><span style=\"color: #000099;\">" +
                link + "</span></html>");
        this.text = text;
    }

    public static void open(URI uri) {
        if (Desktop.isDesktopSupported()) {
            Desktop desktop = Desktop.getDesktop();
            try {
                desktop.browse(uri);
            } catch (IOException e) {
                JOptionPane.showMessageDialog(null,
                        "Failed to launch the link, " +
                                "your computer is likely misconfigured.",
                        "Cannot Launch Link", JOptionPane.WARNING_MESSAGE
                );
            }
        } else {
            JOptionPane.showMessageDialog(null,
                    "Java is not able to launch links on your computer.",
                    "Cannot Launch Link", JOptionPane.WARNING_MESSAGE);
        }
    }
}