package com.rusefi.ui;

import com.rusefi.AvailableHardware;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.ProductionConnectivity;
import com.rusefi.io.LinkManager;
import javax.swing.*;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

/**
 * Manual Swing sandbox showing {@link SlcanTab} in a standalone frame, no console needed.
 * Requires a rusEFI board exposing the CAN sniffer SLCAN channel as a secondary USB CDC port.
 *
 * console variant
 */
public class SlcanUiSandbox {
    public static void main(String[] args) {
        ConnectivityContext connectivityContext = ProductionConnectivity.CONTEXT;
        AvailableHardware hardware = connectivityContext.getCurrentHardware();

        if (hardware.isEmpty()) {
            System.out.println("Scanning for hardware...");
            CountDownLatch latch = new CountDownLatch(1);
            connectivityContext.getPortScanner().addListener(currentHardware -> {
                if (!currentHardware.isEmpty()) {
                    latch.countDown();
                }
            });
            try {
                latch.await(10, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            hardware = connectivityContext.getCurrentHardware();
        }

        if (hardware.isEmpty()) {
            System.err.println("No hardware found.");
            return;
        }

        System.out.println("Found hardware: " + hardware);
        List<PortResult> ecuPorts = hardware.getKnownPorts();
        if (ecuPorts.isEmpty()) {
            System.err.println("No ECU ports found.");
            return;
        }

        PortResult ecuPort = ecuPorts.get(0);
        System.out.println("Connecting to ECU on " + ecuPort.port + "...");

        UIContext uiContext = new UIContext(connectivityContext.getConnectedEcuTarget());
        LinkManager linkManager = uiContext.getLinkManager();

        linkManager.startAndConnect(ecuPort.port, new com.rusefi.io.ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                if (isConnected) {
                    System.out.println("Connected to ECU!");
                } else {
                    System.err.println("Connection status: not connected");
                }
            }

            @Override
            public void onConnectionEstablished() {
                System.out.println("Connection established (signature fetched)!");
            }

            @Override
            public void onConnectionFailed(String s) {
                System.err.println("Connection failed: " + s);
            }
        });

        SwingUtilities.invokeLater(() -> {
            JFrame frame = new JFrame("SLCAN Sandbox - " + ecuPort.port);
            frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
            frame.getContentPane().add(new SlcanTab(uiContext).getContent());
            frame.setSize(900, 600);
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);
        });
    }
}
