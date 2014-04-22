package com.rusefi;

import com.irnems.Launcher;
import com.rusefi.io.tcp.TcpConnector;
import jssc.SerialPortList;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * This frame is used on startup to select the port we would be using
 *
 * @author Andrey Belomutskiy
 *         2/14/14
 */
public class PortLookupFrame {
    public static void chooseSerialPort() {
        java.util.List<String> ports = new ArrayList<String>();
        ports.addAll(Arrays.asList(SerialPortList.getPortNames()));
        ports.addAll(TcpConnector.getAvailablePorts());


        if (ports.size() == 0) {
            JOptionPane.showMessageDialog(null, "No suitable ports found");
            System.exit(-1);
        }

        final JFrame frame = new JFrame("Serial port selection");

        JPanel panel = new JPanel(new FlowLayout());

        for (final String port : ports) {
            JButton button = new JButton("Use " + port);

            button.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    frame.dispose();
                    new Launcher(port);
                }
            });

            panel.add(button);
        }

        frame.add(panel);
        frame.pack();
        frame.setVisible(true);
    }
}
