package com.rusefi.autodetect;

import com.rusefi.UiProperties;
import com.rusefi.autoupdate.Autoupdate;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.io.BoardCompatibility;
import com.rusefi.core.io.BundleInfo;
import com.rusefi.core.io.BundleInfoStrategy;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.net.PropertiesHolder;

import java.io.IOException;

/**
 * Manual sandbox: full "scan and update" cycle against a real ECU.
 * <p>
 * Scans serial ports for a rusEFI ECU, extracts the board name from its signature, and if that
 * board is on the {@code board_compatibility} allowlist (shared_io.properties) downloads the
 * matching bundle from the build server, unzips the firmware .srec, reboots the ECU into the
 * OpenBLT bootloader and flashes the fresh firmware over serial.
 * <p>
 * The reusable flow steps live in {@link ScanAndUpdateFlow} (ui testFixtures).
 *
 * see PortDetectorSandbox
 * see AutoupdateSandbox
 * see BltSwitchSandbox
 */
public class ScanAndUpdateBoardSandbox {

    public static void main(String[] args) throws IOException, InterruptedException {
        // the sandbox does its own compatibility gate and stages its own .srec, so skip the
        // universal-bundle logic inside sendBootloaderRebootCommand (it would download a second
        // bundle and unpack firmware into ".." before rebooting), same as BltSwitchSandbox
        ConnectionAndMeta.getProperties().setProperty(UiProperties.SKIP_ECU_TYPE_DETECTION, "true");

        SerialAutoChecker.AutoDetectResult autoDetectResult = ScanAndUpdateFlow.detectEcuOrExit();
        String ecuPort = autoDetectResult.getSerialPort();

        RusEfiSignature signature = SignatureHelper.parse(autoDetectResult.getSignature());
        if (signature == null) {
            System.out.println("Failed to parse signature [" + autoDetectResult.getSignature() + "]");
            System.exit(-1);
        }
        String boardName = signature.getBundleTarget();
        System.out.println("Board name " + boardName);

        if (!BoardCompatibility.matchesCompatibility(boardName)) {
            System.out.println("Board [" + boardName + "] is not on the board_compatibility list ["
                + BoardCompatibility.getBoardCompatibility() + "], not updating");
            return;
        }

        BundleInfo bundleInfo = signature.asBundleInfo();
        // same branch-aware URL as ensureFirmwareForTarget, without the interactive branch dialog
        String baseUrl = BundleInfoStrategy.getDownloadUrl(bundleInfo, PropertiesHolder.getBaseUrl(), BundleInfo::getBranchName);
        System.out.println("Downloading bundle for " + bundleInfo.getTarget() + " from " + baseUrl);
        String zipFileName = Autoupdate.downloadZipForTarget(bundleInfo, baseUrl,
            percentage -> {}, System.out::println);
        if (zipFileName == null) {
            System.out.println("Bundle for " + bundleInfo.getTarget() + " is not available on the build server");
            System.exit(-1);
        }
        System.out.println("Bundle at " + zipFileName);

        String srecFile = ScanAndUpdateFlow.unzipSrec(zipFileName);
        System.out.println("Firmware image " + srecFile);

        boolean success = ScanAndUpdateFlow.rebootAndFlash(ecuPort, boardName, srecFile);
        System.out.println(success ? "Firmware update SUCCESS" : "Firmware update FAILED");
        System.exit(success ? 0 : -1);
    }
}
