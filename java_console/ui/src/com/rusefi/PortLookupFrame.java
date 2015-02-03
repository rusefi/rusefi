package com.rusefi;

import com.irnems.Launcher;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.ui.UiUtils;
import com.rusefi.ui.widgets.URLLabel;
import jssc.SerialPortList;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * This frame is used on startup to select the port we would be using
 *
 * @author Andrey Belomutskiy
 *         <p/>
 *         2/14/14
 */
public class PortLookupFrame {
    // todo: figure out a better way to work with absolute path
    public static final String APPICON_PNG = "../../appicon.png";
    private static final String LINK_TEXT = "rusEfi (c) 2012-2015";
    private static final String URI = "http://rusefi.com/?java_console";

    private final JFrame frame;
    private boolean isProceeding;

    public PortLookupFrame() {
        frame = new JFrame(Launcher.CONSOLE_VERSION + ": Serial port selection");
        frame.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosed(WindowEvent ev) {
                if (!isProceeding)
                    System.exit(0);
            }
        });
        setAppIcon(frame);
    }

    public static void setAppIcon(JFrame frame) {
        ImageIcon icon = loadIcon(APPICON_PNG);
        if (icon != null)
            frame.setIconImage(icon.getImage());
    }

    public void chooseSerialPort() {
        List<String> ports = new ArrayList<>();
        ports.addAll(Arrays.asList(SerialPortList.getPortNames()));
        ports.addAll(TcpConnector.getAvailablePorts());

        JPanel content = new JPanel(new BorderLayout());

        final JPanel upperPanel = new JPanel(new FlowLayout());

        if (!ports.isEmpty())
            addPortSelection(ports, frame, upperPanel);

        final JButton buttonLogViewer = new JButton();
        buttonLogViewer.setText("Use " + LinkManager.LOG_VIEWER);
        buttonLogViewer.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                disposeFrameAndProceed();
                new Launcher(LinkManager.LOG_VIEWER);
            }
        });

        upperPanel.add(buttonLogViewer);

        JPanel centerPanel = new JPanel(new FlowLayout());
        centerPanel.add(SimulatorHelper.createSimulatorComponent(this));


        JPanel lowerPanel = new JPanel(new FlowLayout());
        lowerPanel.add(new URLLabel(LINK_TEXT, URI));
        content.add(upperPanel, BorderLayout.NORTH);
        content.add(centerPanel, BorderLayout.CENTER);
        content.add(lowerPanel, BorderLayout.SOUTH);


        frame.add(content);
        frame.pack();
        frame.setVisible(true);
        UiUtils.centerWindow(frame);
    }

    public void disposeFrameAndProceed() {
        isProceeding = true;
        frame.dispose();
    }

    private void addPortSelection(List<String> ports, final JFrame frame, JPanel panel) {
        final JComboBox<String> comboPorts = new JComboBox<>();
        for (final String port : ports)
            comboPorts.addItem(port);
        panel.add(comboPorts);

        final JButton connect = new JButton("Connect");
        panel.add(connect);
        connect.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                disposeFrameAndProceed();
                new Launcher(comboPorts.getSelectedItem().toString());
            }
        });
    }

    public static ImageIcon loadIcon(String strPath) {
        URL imgURL = PortLookupFrame.class.getResource(strPath);
        if (imgURL != null)
            return new ImageIcon(imgURL);
        else
            return null;
    }
}