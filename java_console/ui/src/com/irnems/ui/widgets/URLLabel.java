package com.irnems.ui.widgets;


import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

public class URLLabel extends JLabel {

    private static final long serialVersionUID = 8273875024682878518L;
    private String text;
    private URI uri;

    public URLLabel(String text, URI uri) {
        setup(text, uri);
    }

    public URLLabel(String text, String uri) {
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
        setup(text, oURI);
    }

    public void setup(String t, URI u) {
        text = t;
        uri = u;
        setText(text);
        setToolTipText(uri.toString());
        addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                open(uri);
            }

            public void mouseEntered(MouseEvent e) {
                setText(text, false);
            }

            public void mouseExited(MouseEvent e) {
                setText(text, true);
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

    public String getRawText() {
        return text;
    }

    private static void open(URI uri) {
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