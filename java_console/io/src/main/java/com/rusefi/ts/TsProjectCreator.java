package com.rusefi.ts;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.core.FileUtil;
import com.rusefi.maintenance.CalibrationsInfo;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;
import java.util.UUID;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Creates a TunerStudio project on disk from a live ECU's calibrations:
 *  - {@code ~/TunerStudioProjects/<projectName>/} directory scaffold
 *  - Copies the ini as {@code projectCfg/mainController.ini}
 *  - Writes {@code CurrentTune.msq} from the configuration image
 *  - Writes {@code projectCfg/project.properties} (+ .bkup)
 *  - Writes a generated {@code dashboard/dashboard.dash}
 *  - Updates {@code ~/.efiAnalytics/tsUser.properties} so TS auto-opens the project
 * <p>
 * See /com/rusefi/ts/ts_project_format.md for the full on-disk contract.
 */
public final class TsProjectCreator {

    private static final Logging log = getLogging(TsProjectCreator.class);

    private TsProjectCreator() {}

    /**
     * Create the project only if no existing one matches the current ECU. Match priority:
     * <ol>
     *   <li>Same target directory already exists (fast path, also catches our previous runs).</li>
     *   <li>Another project under {@code ~/TunerStudioProjects/} has the same {@code ecuUid} —
     *       same physical ECU regardless of firmware version. This is the case the user cared
     *       about: firmware upgrades change the signature but not the hardware, and we don't want
     *       a fresh project per firmware update.</li>
     *   <li>Another project has the same {@code firmwareDescription} (same signature).</li>
     *   <li>Another project has the same {@code CommSettingMSCommDriver.RS232 Serial InterfaceCom Port}
     *       (same USB slot / tty).</li>
     * </ol>
     *
     * @return true if a project was created, false if an existing one was reused.
     */
    public static boolean createIfMissing(String projectName, String port, CalibrationsInfo calibrations) throws IOException {
        String projectPath = computeProjectPath(projectName);
        if (new File(projectPath + File.separator + "projectCfg" + File.separator + "project.properties").exists()) {
            log.info("Project " + projectName + " already exists by name, skipping creation");
            return false;
        }
        File existing = findMatchingProject(projectsDir(), calibrations, port);
        if (existing != null) {
            log.info("Found existing TS project for this ECU at " + existing.getAbsolutePath() + ", skipping creation");
            return false;
        }
        create(projectName, port, calibrations);
        return true;
    }

    /**
     * Unconditionally create (or overwrite) the project. Used by the sandbox; prefer
     * {@link #createIfMissing} from real console code to preserve user customizations.
     */
    public static void create(String projectName, String port, CalibrationsInfo calibrations) throws IOException {
        String signature = sanitizeSignature(calibrations.getImage().getMeta().getEcuSignature());
        String normalizedPort = normalizeSerialPort(port);

        String projectsDir = System.getProperty("user.home") + File.separator + "TunerStudioProjects";
        String projectPath = projectsDir + File.separator + projectName;
        File projectCfg = new File(projectPath + File.separator + "projectCfg");
        if (!projectCfg.mkdirs() && !projectCfg.isDirectory()) {
            throw new IOException("Could not create " + projectCfg);
        }
        for (String dir : new String[]{"dashboard", "DataLogs", "inc", "TuneView", "restorePoints"}) {
            new File(projectPath + File.separator + dir).mkdirs();
        }

        FileUtil.copyFile(calibrations.getIniFile().getIniFilePath(),
            projectPath + File.separator + TsHelper.MAIN_CONTROLLER_PATH);
        try {
            calibrations.generateMsq().writeXmlFile(projectPath + File.separator + TsHelper.CURRENT_TUNE_MSQ);
        } catch (jakarta.xml.bind.JAXBException e) {
            throw new IOException("Failed to write MSQ", e);
        }
        DashboardFactory.writeDashboard(
            projectPath + File.separator + "dashboard" + File.separator + "dashboard.dash",
            signature, calibrations);

        writeProjectProperties(projectPath, projectName, normalizedPort, signature, ecuUidFrom(calibrations));
        writeTsUserProperties(projectPath, projectsDir, signature);
    }

    static String computeProjectPath(String projectName) {
        return projectsDir() + File.separator + projectName;
    }

    private static String projectsDir() {
        return System.getProperty("user.home") + File.separator + "TunerStudioProjects";
    }

    /**
     * Scan all immediate subdirs of {@code ~/TunerStudioProjects/} looking for a
     * {@code projectCfg/project.properties} whose {@code ecuUid}, {@code firmwareDescription},
     * or serial-port setting matches the ECU we're about to create a project for.
     * Returns the first match, or null.
     */
    private static File findMatchingProject(String projectsDir, CalibrationsInfo calibrations, String port) {
        File dir = new File(projectsDir);
        if (!dir.isDirectory()) return null;
        File[] children = dir.listFiles(File::isDirectory);
        if (children == null) return null;

        String ecuUid = ecuUidFrom(calibrations).toString();
        String signature = sanitizeSignature(calibrations.getImage().getMeta().getEcuSignature());
        String normalizedPort = normalizeSerialPort(port);

        for (File child : children) {
            File propsFile = new File(child, "projectCfg" + File.separator + "project.properties");
            if (!propsFile.isFile()) continue;
            Properties p = new Properties();
            try (FileInputStream fis = new FileInputStream(propsFile)) {
                p.load(fis);
            } catch (IOException e) {
                log.warn("Skipping unreadable project " + child.getName() + ": " + e.getMessage());
                continue;
            }
            if (ecuUid.equals(p.getProperty("ecuUid"))) {
                log.info("Existing project " + child.getName() + " matches by ecuUid");
                return child;
            }
            if (signature.equals(p.getProperty("firmwareDescription"))) {
                log.info("Existing project " + child.getName() + " matches by firmwareDescription");
                return child;
            }
            if (normalizedPort.equals(p.getProperty("CommSettingMSCommDriver.RS232 Serial InterfaceCom Port"))) {
                log.info("Existing project " + child.getName() + " matches by serial port");
                return child;
            }
        }
        return null;
    }

    /**
     * ECU signature comes out of the binary protocol zero-padded; a trailing null in
     * project.properties makes TunerStudio declare the file "corrupt, no configuration options found".
     */
    static String sanitizeSignature(String raw) {
        if (raw == null) return "";
        return raw.replaceAll("\\p{Cntrl}", "").trim();
    }

    /** On Linux PortDetector returns a bare tty name ("ttyACM0"); TS wants the full device path. */
    static String normalizeSerialPort(String port) {
        if (port == null) return "";
        if (port.startsWith("tty") && !port.contains("/")) {
            return "/dev/" + port;
        }
        return port;
    }

    /**
     * Deterministic UUID from the STM32 factory UID exposed via device_uid1..3.
     * Same ECU always yields the same project UUID; falls back to random if the ini
     * doesn't expose the field (older firmware).
     */
    static UUID ecuUidFrom(CalibrationsInfo calibrations) {
        IniFileModel ini = calibrations.getIniFile();
        IniField uid1 = ini.getAllIniFields().get("device_uid1");
        if (uid1 == null) {
            return UUID.randomUUID();
        }
        ConfigurationImage image = calibrations.getImage().getConfigurationImage();
        byte[] uidBytes = new byte[12];
        assert image != null;
        System.arraycopy(image.getContent(), uid1.getOffset(), uidBytes, 0, 12);
        return UUID.nameUUIDFromBytes(uidBytes);
    }

    private static void writeProjectProperties(String projectPath, String projectName, String port,
                                               String signature, UUID ecuUid) throws IOException {
        Properties p = new Properties();
        p.setProperty("projectName", projectName);
        p.setProperty("projectDescription", "");
        p.setProperty("ecuConfigFile", "mainController.ini");
        p.setProperty("ecuUid", ecuUid.toString());
        p.setProperty("ecuSettings", "");
        p.setProperty("firmwareDescription", signature);
        p.setProperty("lastDisplayedTuneFile", "");
        p.setProperty("dashBoardFile", "/dashboard//dashboard.dash");
        p.setProperty("useCommonDashboardDir", "false");
        p.setProperty("useCommonTuneViewDir", "false");
        p.setProperty("canId", "-1");
        p.setProperty("selectedComDriver", "Multi Interface MegaSquirt Driver");
        p.setProperty("CommSettingCom Port", "COM1");
        p.setProperty("CommSettingBaud Rate", "115200");
        p.setProperty("CommSettingMSCommDriver.controllerInterfaceId", "RS232 Serial Interface");
        p.setProperty("CommSettingMSCommDriver.RS232 Serial InterfaceCom Port", port);
        p.setProperty("CommSettingMSCommDriver.RS232 Serial InterfaceBaud Rate", "115200");
        p.setProperty("CommSettingMSCommDriver.RS232 Serial InterfaceBluetooth Port", "false");
        p.setProperty("CommSettingMSCommDriver.RS232 Serial Interface2nd Com Port", "");

        String projectPropertiesPath = projectPath + File.separator + "projectCfg" + File.separator + "project.properties";
        try (FileOutputStream fos = new FileOutputStream(projectPropertiesPath)) {
            p.store(fos, "rusEFI");
        }
        // TS keeps a .bkup copy beside it; create it up-front so the project doesn't look half-written.
        FileUtil.copyFile(projectPropertiesPath, projectPropertiesPath + ".bkup");
    }

    private static void writeTsUserProperties(String projectPath, String projectsDir, String signature) throws IOException {
        Properties p = loadTsPropertiesOrEmpty();
        p.setProperty("lastProjectPath", projectPath);
        p.setProperty("lastConnectedFirmwareSignature", signature);
        p.setProperty("projectsDir", projectsDir);
        p.setProperty("recentlyOpenedProject_0", projectPath);
        p.setProperty("loadLastProjectOnStart2", "true");
        new File(TsHelper.TS_ROOT).mkdirs();
        try (FileOutputStream fos = new FileOutputStream(TsHelper.TS_USER_FILE)) {
            p.store(fos, "rusEFI");
        }
    }

    private static Properties loadTsPropertiesOrEmpty() throws IOException {
        Properties properties = new Properties();
        File file = new File(TsHelper.TS_USER_FILE);
        if (file.exists()) {
            try (FileInputStream fis = new FileInputStream(file)) {
                properties.load(fis);
            }
        }
        return properties;
    }
}
