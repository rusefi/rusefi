package com.rusefi;

import com.opensr5.ConfigurationImage;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/7/2015
 */
public class UploadChanges {
    //public static final Logger logger = createUiLogger();

    public static void main(String[] args) throws InvocationTargetException, InterruptedException {

        StatusWindow wnd = new StatusWindow();

        wnd.getFrame().setDefaultCloseOperation(WindowConstants.HIDE_ON_CLOSE);
        wnd.getFrame().setTitle("rusEfi bin upload");
        wnd.getFrameHelper().initFrame(wnd.getContent(), false);

        JPanel bottomPanel = new JPanel(new FlowLayout());
//        bottomPanel.add(RecentCommands.createButton(new AtomicBoolean(), Integration.CMD_WRITECONFIG));
        wnd.getContent().add(bottomPanel, BorderLayout.SOUTH);

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
                } catch (IOException | InterruptedException e) {
                    throw new IllegalStateException(e);
                }
            }
        });
    }

    private static void showUi(String port) throws IOException, InterruptedException {
/*
        SerialPort serialPort;

        serialPort = new SerialPort(port);
        boolean opened = serialPort.openPort();
        if (!opened) {
            logger.error("failed to open " + port);
        }
        SerialIoStreamJSSC.setupPort(serialPort, 38400);
        logger.info("Configuration looks good!");

        final ConfigurationImage ci1 = ConfigurationImageFile.readFromFile("rus_saved.bin");

        final ConfigurationImage ci2 = ConfigurationImageFile.readFromFile("rusefi_configuration.bin");

        final BinaryProtocol bp = BinaryProtocolHolder.getInstance().create(logger, new SerialIoStreamJSSC(serialPort, logger));
        bp.setController(ci1);

        scheduleUpload(ci2);

 */
    }

    /*
    public static void scheduleUpload(UIContext uiContext, final ConfigurationImage newVersion) {
        scheduleUpload(uiContext, newVersion, null);
    }
*/
    public static void scheduleUpload(UIContext uiContext, final ConfigurationImage newVersion, final Runnable afterUpload) {
        if (1 == 1)
            throw new UnsupportedOperationException("disabled");
        JFrame frame = null;//wnd.getFrame();
        frame.setVisible(true);
        uiContext.getLinkManager().execute(new Runnable() {
            @Override
            public void run() {
                uiContext.getLinkManager().getCurrentStreamState().uploadChanges(newVersion);
                if (afterUpload != null)
                    afterUpload.run();
            }

            @Override
            public String toString() {
                return "Runnable for burn";
            }
        });
    }

/*
    private static Logger createUiLogger() {
        return new Logger() {
            @Override
            public void trace(final String msg) {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
//                        System.out.println(msg);
//                        wnd.append(msg);
                    }
                });
            }

            @Override
            public void info(final String msg) {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        System.out.println(msg);
                        wnd.append(msg);
                    }
                });
            }

            @Override
            public void error(final String msg) {
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        System.out.println(msg);
                        wnd.append(msg);
                    }
                });
            }
        };
    }
*/
}
