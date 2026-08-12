package com.rusefi.autodetect;

import com.fazecast.jSerialComm.SerialPort;
import com.rusefi.core.FileUtil;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.io.IoStream;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.maintenance.ProgramSelector;
import com.rusefi.updater.OpenbltDetectorStrategy;

import java.io.File;
import java.io.IOException;
import java.util.concurrent.atomic.AtomicReference;

/**
 * Reusable steps of the manual "scan and update" cycle: detect an ECU on serial, stage a
 * firmware .srec from a bundle zip, reboot into the OpenBLT bootloader and flash.
 * <p>
 * Shared between the generic {@code ScanAndUpdateBoardSandbox} (ui tests) and board-specific
 * sandboxes in custom-board repositories (e.g. fw-iws {@code HarleyUpdateSandbox}).
 */
public class ScanAndUpdateFlow {
    public static final int OPENBLT_PORT_WAIT_SECONDS = 90;

    /**
     * Scans serial ports for a rusEFI ECU; exits the JVM if none found.
     */
    public static SerialAutoChecker.AutoDetectResult detectEcuOrExit() {
        SerialAutoChecker.AutoDetectResult autoDetectResult = PortDetector.autoDetectSerial(null);
        if (autoDetectResult.getSerialPort() == null) {
            System.out.println("No ECU detected on any serial port");
            System.exit(-1);
        }
        System.out.println("Port detected " + autoDetectResult.getSerialPort() + ", "
            + autoDetectResult.getSignature());
        return autoDetectResult;
    }

    /**
     * Cached download of {@code baseUrl + fileName} into the local {@code temp} folder, with the
     * same freshness rule as Autoupdate.downloadZipForTarget (size + last-modified match).
     *
     * @return local zip path
     */
    public static String downloadZip(String baseUrl, String fileName) throws IOException {
        File outDir = new File("temp");
        outDir.mkdirs();
        File localZip = new File(outDir, fileName);
        ConnectionAndMeta connectionAndMeta = new ConnectionAndMeta(fileName).invoke(baseUrl);
        if (localZip.length() == connectionAndMeta.getCompleteFileSize()
            && localZip.lastModified() == connectionAndMeta.getLastModified()) {
            System.out.println("Using cached " + localZip.getPath());
            return localZip.getPath();
        }
        System.out.println("Downloading " + baseUrl + fileName
            + " (" + (connectionAndMeta.getCompleteFileSize() / 1024) + " KB)...");
        ConnectionAndMeta.downloadFile(localZip.getPath(), connectionAndMeta, percentage -> {});
        System.out.println("Download complete " + localZip.getPath());
        return localZip.getPath();
    }

    /**
     * Extracts the firmware .srec from a bundle zip into the local {@code temp} folder;
     * exits the JVM if the zip has no .srec.
     */
    public static String unzipSrec(String zipFileName) throws IOException {
        File outDir = new File("temp");
        // FileUtil.unzip does not create the destination directory (production callers unzip into existing ones)
        outDir.mkdirs();
        AtomicReference<String> srecName = new AtomicReference<>();
        FileUtil.unzip(zipFileName, outDir, zipEntry -> {
            boolean isSrec = zipEntry.getName().endsWith(".srec");
            if (isSrec) {
                srecName.set(zipEntry.getName());
            }
            return isSrec;
        });
        if (srecName.get() == null) {
            System.out.println("No .srec found in " + zipFileName);
            System.exit(-1);
        }
        return new File(outDir, srecName.get()).getAbsolutePath();
    }

    /**
     * Reboots the ECU into the OpenBLT bootloader, waits for the bootloader port to appear and
     * flashes the given .srec over serial.
     */
    public static boolean rebootAndFlash(String ecuPort, String boardName, String srecFile) throws InterruptedException {
        System.out.println("Rebooting ECU on " + ecuPort + " to OpenBLT...");
        ProgramSelector.rebootToOpenblt(null, ecuPort, UpdateOperationCallbacks.LOGGER);

        String openBltPort = waitForOpenBltPort();
        if (openBltPort == null) {
            System.out.println("No OpenBLT port appeared within " + OPENBLT_PORT_WAIT_SECONDS + " seconds");
            return false;
        }
        System.out.println("OpenBLT bootloader on " + openBltPort + ", flashing...");

        ConnectedEcuTarget connectedEcuTarget = new ConnectedEcuTarget();
        connectedEcuTarget.set(boardName);
        return ProgramSelector.flashOpenbltSerial(null, openBltPort,
            UpdateOperationCallbacks.LOGGER, connectedEcuTarget, srecFile);
    }

    /**
     * Same direct-probe approach as ProgramSelector.waitForNewOpenBltPortAppeared: scan all system
     * serial ports and XCP-probe each one until the bootloader answers, since OpenBLT may enumerate
     * on a different port than the original ECU port.
     */
    public static String waitForOpenBltPort() throws InterruptedException {
        long start = System.currentTimeMillis();
        long deadline = start + OPENBLT_PORT_WAIT_SECONDS * 1_000L;
        long lastStatus = 0;
        while (System.currentTimeMillis() < deadline) {
            for (SerialPort sp : SerialPort.getCommPorts()) {
                String portName = sp.getSystemPortName();
                long now = System.currentTimeMillis();
                if (now - lastStatus >= 3_000) {
                    System.out.println("Waiting for OpenBLT port, " + (now - start) / 1_000
                        + "s elapsed, probing " + portName + "...");
                    lastStatus = now;
                }
                try (IoStream stream = BufferedSerialIoStream.openPort(portName)) {
                    if (OpenbltDetectorStrategy.isPortOpenblt(stream)) {
                        return portName;
                    }
                } catch (Exception e) {
                    // port busy or device still transitioning - keep polling
                }
            }
            Thread.sleep(1000);
        }
        return null;
    }
}
