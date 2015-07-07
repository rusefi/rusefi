package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConfigurationImageFile;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.PortHolder;
import com.rusefi.ui.StatusWindow;
import jssc.SerialPort;
import jssc.SerialPortException;

import javax.swing.*;
import java.io.EOFException;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;

/**
 * (c) Andrey Belomutskiy 2013-2015
 * 3/7/2015
 */
public class UploadChanges {
    public static final Logger logger = createUiLogger();

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

        final ConfigurationImage ci1 = ConfigurationImageFile.readFromFile("rus_saved.bin");

        final ConfigurationImage ci2 = ConfigurationImageFile.readFromFile("rusefi_configuration.bin");

        final BinaryProtocol bp = new BinaryProtocol(logger, serialPort);
        bp.setController(ci1);

        scheduleBurn(ci2, bp);
    }

    public static void scheduleBurn(final ConfigurationImage newVersion, final BinaryProtocol bp) {
        LinkManager.COMMUNICATION_EXECUTOR.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    BinaryProtocol.instance.burnChanges(newVersion, logger);
                } catch (InterruptedException | EOFException | SerialPortException e) {
                    logger.error("Error: " + e);
                    throw new IllegalStateException(e);
                }
            }

            @Override
            public String toString() {
                return "Runnable for burn";
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
//                        System.out.println(msg);
//                        wnd.appendMsg(msg);
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
