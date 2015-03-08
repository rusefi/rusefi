package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.ui.StatusWindow;
import jssc.SerialPort;
import jssc.SerialPortException;

import javax.swing.*;
import java.io.EOFException;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

/**
 * (c) Andrey Belomutskiy 2013-2015
 * 3/7/2015
 */
public class UploadChanges {
    public static final Logger logger = createUiLogger();
    private static final Executor EXEC = Executors.newSingleThreadExecutor();

    public static void main(String[] args) throws SerialPortException, InvocationTargetException, InterruptedException {
        if (args.length != 1) {
            System.out.println("Exactly one parameter expected");
            return;
        }

        final String port = args[0];
        SwingUtilities.invokeAndWait(new Runnable() {
            @Override
            public void run() {
                try {
                    showUi(port);
                } catch (SerialPortException | IOException | InterruptedException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    private static void showUi(String port) throws SerialPortException, IOException, InterruptedException {
        SerialPort serialPort;

        serialPort = new SerialPort(port);
        boolean opened = serialPort.openPort();
        if (!opened) {
            logger.error("failed to open " + port);
        }
        PortHolder.setupPort(serialPort, 38400);
        logger.info("Looks good");

        final ConfigurationImage ci1 = ConfigurationImage.readFromFile("rus_saved.bin", logger);

        final ConfigurationImage ci2 = ConfigurationImage.readFromFile("rusefi_configuration.bin", logger);

        final BinaryProtocol bp = new BinaryProtocol(logger, serialPort);
        bp.setController(ci1);

        scheduleBurn(ci2, bp);
    }

    public static void scheduleBurn(final ConfigurationImage newVersion, final BinaryProtocol bp) {
        EXEC.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    bp.burnChanges(newVersion, logger);
                } catch (InterruptedException | EOFException | SerialPortException e) {
                    logger.error("Error: " + e);
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    private static Logger createUiLogger() {
        final StatusWindow wnd = new StatusWindow();
        wnd.showFrame("rusEfi bin upload");

        return new Logger() {
            @Override
            public void trace(final String msg) {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        System.out.println(msg);
                        wnd.appendMsg(msg);
                    }
                });
            }

            @Override
            public void info(final String msg) {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        System.out.println(msg);
                        wnd.appendMsg(msg);
                    }
                });
            }

            @Override
            public void error(final String msg) {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        System.out.println(msg);
                        wnd.appendMsg(msg);
                    }
                });
            }
        };
    }

}
