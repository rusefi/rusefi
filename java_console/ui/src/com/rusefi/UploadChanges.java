package com.rusefi;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.Pair;
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
        final StatusWindow wnd = new StatusWindow();
        wnd.showFrame("rusEfi bin upload");

        final Logger logger = new Logger() {
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

        EXEC.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    patch(ci1, ci2, bp, logger);
                } catch (InterruptedException | EOFException | SerialPortException e) {
                    logger.error("Error: " + e);
                    throw new IllegalStateException(e);
                }
            }
        });


    }

    private static void patch(ConfigurationImage ci1, ConfigurationImage ci2, BinaryProtocol bp, Logger logger) throws InterruptedException, EOFException, SerialPortException {
        int offset = 0;
        while (offset < ci1.getSize()) {
            Pair<Integer, Integer> range = ConfigurationImageDiff.findDifferences(ci1, ci2, offset);
            if (range == null)
                break;
            logger.info("Need to patch: " + range);
            bp.writeData(ci2.getContent(), range.first, range.second - range.first, logger);

            offset = range.second;
        }
        bp.burn();
    }
}
