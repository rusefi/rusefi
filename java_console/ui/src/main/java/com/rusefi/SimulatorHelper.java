package com.rusefi;

import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.util.concurrent.ThreadFactory;

import static com.rusefi.ui.util.UiUtils.setToolTip;

public class SimulatorHelper {
    private final static ThreadFactory THREAD_FACTORY = new NamedThreadFactory("SimulatorHelper");
    private static final String SIMULATOR_BINARY_PATH = "./";
    private static Process process;

    /**
     * this code start sumulator for UI console
     * todo: unify with the code which starts simulator for auto tests?
     */
    private static void startSimulator(File binary) {
        LinkManager.isSimulationMode = true;

        FileLog.MAIN.logLine("Executing " + binary.getPath());
        THREAD_FACTORY.newThread(new Runnable() {
            @Override
            public void run() {
                try {
                    FileLog.SIMULATOR_CONSOLE.start();
                    process = Runtime.getRuntime().exec(binary.getPath());
                    FileLog.MAIN.logLine("Executing " + binary.getPath() + "=" + process);
                    SimulatorExecHelper.dumpProcessOutput(process);
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }
        }).start();

        // unfortunately at this point Windows might popup a dialog asking for a permission to listen to port

        boolean isPortOpened = false;
        for (int i = 0; i < 60 && !isPortOpened; i++) {
            isPortOpened = TcpConnector.isTcpPortOpened();
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                throw new IllegalStateException("Unexpected", e);
            }
        }
        if (!isPortOpened)
            throw new IllegalStateException("Port not opened?");
        FileLog.MAIN.logLine("Port " + TcpConnector.DEFAULT_PORT + " is alive");

        new ConsoleUI("" + TcpConnector.DEFAULT_PORT);
    }

    public static JComponent createSimulatorComponent(final StartupFrame portSelector) {
        File simulatorBinary;
        try {
            simulatorBinary = SimulatorExecHelper.getSimulatorBinary(SIMULATOR_BINARY_PATH);
        } catch (IllegalStateException e) {
            return new JLabel(e.getMessage());
        }

        if (TcpConnector.isTcpPortOpened()) {
            return new JLabel("Port " + TcpConnector.DEFAULT_PORT + " already busy. Simulator running?");
        }

        JButton simulatorButton = new JButton("Start Virtual Simulator");
        simulatorButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                portSelector.disposeFrameAndProceed();
                startSimulator(simulatorBinary);
            }
        });
        setToolTip(simulatorButton, "Connect to totally virtual simulator",
                "You do not need any hardware to try rusEfi",
                "This works via localhost: " + TcpConnector.DEFAULT_PORT + " TCP/IP port");

        return simulatorButton;
    }

    public static void onWindowClosed() {
        if (process != null)
            process.destroy();
    }
}
