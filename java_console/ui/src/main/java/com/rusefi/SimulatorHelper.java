package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.core.ui.ErrorMessageHelper;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpConnector;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ThreadFactory;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.ui.util.UiUtils.setToolTip;

public class SimulatorHelper {
    private final static ThreadFactory THREAD_FACTORY = new NamedThreadFactory("SimulatorHelper");
    private static final String BINARY = "rusefi_simulator.exe";
    private static Process process;
    private static final Logging log = getLogging(SimulatorHelper.class);

    public static boolean isBinaryHere() {
        return new File(BINARY).exists();
    }

    public interface Listener {
        void onSimulatorLaunched(boolean isLaunchedOk);
    }
    /**
     * this code starts simulator for UI console
     * todo: unify with the code which starts simulator for auto tests?
     */
    private static void startSimulator(Listener simulatorListener) {
        LinkManager.isSimulationMode = true;

        launchSimulatorInADifferenceThread();

        THREAD_FACTORY.newThread(new Runnable() {
            @Override
            public void run() {
                // unfortunately at this point Windows might popup a dialog asking for a permission to listen to port

                boolean isPortOpened = false;
                for (int i = 0; i < 60 && !isPortOpened; i++) {
                    isPortOpened = TcpConnector.isTcpPortOpened();
                    try {
                        Thread.sleep(1000);
                        log.info("Process " + process);
                    } catch (InterruptedException e) {
                        throw new IllegalStateException("Unexpected", e);
                    }
                }
                if (!isPortOpened) {
                    simulatorListener.onSimulatorLaunched(/*isLaunchedOk*/false);
                } else {
                    log.info("Port " + TcpConnector.DEFAULT_PORT + " is alive");
                    simulatorListener.onSimulatorLaunched(/*isLaunchedOk*/true);
                }

            }
        }).start();
    }

    private static void launchSimulatorInADifferenceThread() {
        log.info("Executing simulator " + BINARY);
        THREAD_FACTORY.newThread(new Runnable() {
            @Override
            public void run() {
                try {
                    process = Runtime.getRuntime().exec(BINARY);
                    log.info("Executing simulator " + BINARY + "=" + process);
                    SimulatorExecHelper.dumpProcessOutput(process, new CountDownLatch(1));
                } catch (IOException e) {
                    log.warn("Error " + e, e);
                    throw new IllegalStateException(e);
                }
            }
        }).start();
    }

    public static JComponent createSimulatorComponent(final StartupFrame portSelector) {
        if (!SimulatorHelper.isBinaryHere())
            return new JLabel(SimulatorHelper.BINARY + " not found");

        if (TcpConnector.isTcpPortOpened())
            return new JLabel("Port " + TcpConnector.DEFAULT_PORT + " already busy. Simulator running?");

        JButton simulatorButton = new JButton("Start Virtual Simulator");
        simulatorButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                simulatorButton.setText("Launching...");
                simulatorButton.setEnabled(false);
                startSimulator(isLaunchedOk -> SwingUtilities.invokeLater(() -> {
                    if (isLaunchedOk) {
                        portSelector.disposeFrameAndProceed();
                        new ConsoleUI("" + TcpConnector.DEFAULT_PORT, SerialPortType.Unknown);
                    } else {
                        ErrorMessageHelper.showErrorDialog("Error starting simulator", "Error");
                        simulatorButton.setText("Failed");
                    }
                }));
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
