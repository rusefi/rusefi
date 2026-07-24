package com.rusefi.binaryprotocol.test;

import com.opensr5.ConfigurationImage;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IoHelper;
import com.rusefi.io.LinkManager;
import com.rusefi.tune.xml.Msq;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

/**
 * End-to-end "send a tune -> burn -> read back page CRC -> compare" harness against a REAL ECU
 * over serial. This reproduces the round-trip that TunerStudio performs (and that fails on a
 * cross-firmware tune, see {@code docs/issue.md}): it is the write+burn+CRC step that neither the
 * hardware {@code f4discovery/BurnCommandTest} (writes+burns but never re-reads the CRC) nor the
 * protocol {@code SandboxCommon.verifyCrcNoPending} (reads CRC but never writes) exercises on its own.
 *
 * <p>Usage: pass the path to a TunerStudio {@code .msq} tune file as the single argument.
 * <pre>SerialWriteTuneBurnCrcSandbox path/to/tune.msq</pre>
 *
 * <p>What it does, on the communication thread:
 * <ol>
 *     <li>Baseline: compute {@link IoHelper#getCrc32} of the just-read image and compare it to the
 *         controller's CRC ({@link BinaryProtocol#getCrcFromController}). These must already match -
 *         that is the invariant the whole config cache relies on.</li>
 *     <li>Apply the constants from the {@code .msq} onto a copy of the ECU's current image
 *         ({@link Msq#applyOnto}) - fields absent from the firmware are skipped, so the image size
 *         is preserved.</li>
 *     <li>{@link BinaryProtocol#uploadChanges} - patch the changed bytes into ECU RAM, burn to
 *         flash, and update the local baseline.</li>
 *     <li>After-burn: recompute the local CRC and read the controller CRC again; assert they match.</li>
 * </ol>
 *
 * Not part of the automated suite - launch {@code main()} by hand against connected hardware.
 *
 * @see SerialSandbox
 * @see SandboxCommon
 */
public class SerialWriteTuneBurnCrcSandbox {
    public static void main(String[] args) throws Exception {
        if (args.length != 1) {
            System.err.println("Usage: SerialWriteTuneBurnCrcSandbox <path-to-tune.msq>");
            System.exit(1);
        }
        String tuneFile = args[0];

        // Read (and validate) the tune up front so a bad path fails before we touch hardware.
        Msq tune = Msq.readTune(tuneFile);
        System.out.println("Loaded tune " + tuneFile + " signature '" + tune.getVersionInfo().getSignature() + "'");

        SerialAutoChecker.AutoDetectResult autoDetectResult = PortDetector.autoDetectSerial(callbackContext -> null);
        String port = autoDetectResult.getSerialPort();
        if (port == null) {
            throw new IllegalStateException("No ECU detected on any serial port");
        }
        System.out.println("Serial detected on " + port + " signature '" + autoDetectResult.getSignature() + "'");

        LinkManager linkManager = new LinkManager()
            .setNeedPullText(false)
            .setNeedPullLiveData(false);

        try {
            linkManager.connect(port, false).await(60, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException("Not connected in time");
        }

        CountDownLatch latch = new CountDownLatch(1);
        linkManager.execute(() -> {
            try {
                runWriteBurnCrc(linkManager.getBinaryProtocol(), tune);
            } catch (RuntimeException e) {
                System.out.println("FAILED: " + e);
                e.printStackTrace();
            } finally {
                latch.countDown();
            }
        });
        latch.await(2, TimeUnit.MINUTES);
        System.out.println("Done, closing connection");
        linkManager.close();
    }

    private static void runWriteBurnCrc(BinaryProtocol bp, Msq tune) {
        // image read during connect - this is our best knowledge of what is in the controller
        ConfigurationImage current = bp.getControllerConfiguration();

        if (!assertCrcMatches(bp, current, "BASELINE")) {
            // If the baseline already disagrees the local image and the ECU are out of sync -
            // proceeding would only confuse the after-burn comparison.
            throw new IllegalStateException("Baseline CRC mismatch - local image does not match connected ECU, aborting");
        }

        String tuneSignature = tune.getVersionInfo().getSignature();
        if (tuneSignature != null && !tuneSignature.equals(bp.signature)) {
            // Not fatal: applyOnto skips fields the firmware does not know about. But this is exactly
            // the cross-firmware situation from docs/issue.md, so make it loud.
            System.out.println("WARNING: tune signature '" + tuneSignature
                + "' does not match ECU signature '" + bp.signature + "' - cross-firmware load");
        }

        // apply the .msq constants onto a copy of the ECU's current image (preserves size,
        // skips fields absent from this firmware)
        ConfigurationImage modified = tune.applyOnto(current, bp.getIniFile());

        // write changed bytes to ECU RAM, burn to flash, and adopt 'modified' as the new baseline
        bp.uploadChanges(modified);
        System.out.println("uploadChanges (write + burn) complete");

        if (assertCrcMatches(bp, modified, "AFTER-BURN")) {
            System.out.println("SUCCESS: controller CRC matches the written tune after burn");
        } else {
            throw new IllegalStateException("AFTER-BURN CRC mismatch - the burned tune does not match what we sent");
        }
    }

    /**
     * @return true if the controller-reported CRC32 equals the CRC32 of the given local image
     */
    private static boolean assertCrcMatches(BinaryProtocol bp, ConfigurationImage image, String stage) {
        int localCrc = IoHelper.getCrc32(image.getContent());
        int controllerCrc = bp.getCrcFromController(image.getSize());
        boolean match = localCrc == controllerCrc;
        System.out.printf("%s CRC: local=0x%08X controller=0x%08X -> %s%n",
            stage, localCrc, controllerCrc, match ? "MATCH" : "MISMATCH");
        return match;
    }
}
