package com.rusefi.ts;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.core.FileUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.tune.xml.Msq;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Optional;
import java.util.Properties;
import java.util.UUID;

public class TsCreateProjectSandbox {
    public static void main(String[] args) throws Exception {
        // scan for ECU
        SerialAutoChecker.AutoDetectResult detect = PortDetector.autoDetectSerial(null);
        if (detect.getSerialPort() == null || detect.getSignature() == null) {
            System.err.println("No ECU found");
            System.exit(1);
        }
        // ECU signature comes out of the binary protocol zero-padded; a trailing null in
        // project.properties makes TunerStudio declare the file "corrupt, no configuration options found".
        String signature = detect.getSignature().replaceAll("\\p{Cntrl}", "").trim();
        // On Linux PortDetector returns a bare tty name ("ttyACM0"); TS wants the full device path.
        String port = normalizeSerialPort(detect.getSerialPort());
        System.out.println("Detected ECU at " + port + " signature=" + signature);

        // read calibrations + ini straight off the ECU
        Optional<CalibrationsInfo> maybeCalibrations =
            CalibrationsHelper.readCurrentCalibrationsWithoutSuspendingPortScanner(
                port, UpdateOperationCallbacks.LOGGER);
        if (!maybeCalibrations.isPresent()) {
            System.err.println("Failed to read calibrations from " + port);
            System.exit(2);
        }
        CalibrationsInfo calibrations = maybeCalibrations.get();
        String sourceIniPath = calibrations.getIniFile().getIniFilePath();

        // create project directory using serial port we have located
        String projectsDir = System.getProperty("user.home") + File.separator + "TunerStudioProjects";
        String projectName = "rusefi_" + signature.replaceAll("[^a-zA-Z0-9]", "_");
        String projectPath = projectsDir + File.separator + projectName;
        File projectCfg = new File(projectPath + File.separator + "projectCfg");
        if (!projectCfg.mkdirs() && !projectCfg.isDirectory()) {
            System.err.println("Could not create " + projectCfg);
            System.exit(3);
        }

        // TS projects always have these sibling dirs; empty dashboard/ is fine — TS generates dashboard.dash on first open.
        for (String dir : new String[] {"dashboard", "DataLogs", "inc", "TuneView", "restorePoints"}) {
            new File(projectPath + File.separator + dir).mkdirs();
        }

        FileUtil.copyFile(sourceIniPath, projectPath + File.separator + TsHelper.MAIN_CONTROLLER_PATH);
        calibrations.generateMsq().writeXmlFile(projectPath + File.separator + TsHelper.CURRENT_TUNE_MSQ);

        Properties projectProperties = new Properties();
        projectProperties.setProperty("projectName", projectName);
        projectProperties.setProperty("projectDescription", "");
        projectProperties.setProperty("ecuConfigFile", "mainController.ini");
        projectProperties.setProperty("ecuUid", ecuUidFrom(calibrations).toString());
        projectProperties.setProperty("ecuSettings", "");
        projectProperties.setProperty("firmwareDescription", signature);
        projectProperties.setProperty("lastDisplayedTuneFile", "");
        // Deliberately NOT setting dashBoardFile — TS tries to load that path literally. Without a
        // pre-generated dashboard.dash (TS writes its own ~500KB one including embedded fonts),
        // pointing at a missing file throws FileNotFoundException and TS shows "error loading
        // default gauge cluster". Omitting the key makes TS fall back to its built-in default.
        projectProperties.setProperty("useCommonDashboardDir", "false");
        projectProperties.setProperty("useCommonTuneViewDir", "false");
        projectProperties.setProperty("canId", "-1");
        projectProperties.setProperty("selectedComDriver", "Multi Interface MegaSquirt Driver");
        projectProperties.setProperty("CommSettingCom Port", "COM1");
        projectProperties.setProperty("CommSettingBaud Rate", "115200");
        projectProperties.setProperty("CommSettingMSCommDriver.controllerInterfaceId", "RS232 Serial Interface");
        projectProperties.setProperty("CommSettingMSCommDriver.RS232 Serial InterfaceCom Port", port);
        projectProperties.setProperty("CommSettingMSCommDriver.RS232 Serial InterfaceBaud Rate", "115200");
        projectProperties.setProperty("CommSettingMSCommDriver.RS232 Serial InterfaceBluetooth Port", "false");
        projectProperties.setProperty("CommSettingMSCommDriver.RS232 Serial Interface2nd Com Port", "");
        String projectPropertiesPath = projectPath + File.separator + "projectCfg" + File.separator + "project.properties";
        try (FileOutputStream fos = new FileOutputStream(projectPropertiesPath)) {
            projectProperties.store(fos, "rusEFI sandbox");
        }
        // TS keeps a .bkup copy beside it; create the same content so the project doesn't look half-written.
        FileUtil.copyFile(projectPropertiesPath, projectPropertiesPath + ".bkup");

        // make new project the "last project" — TS opens it on next launch
        Properties tsProperties = loadTsPropertiesOrEmpty();
        tsProperties.setProperty("lastProjectPath", projectPath);
        tsProperties.setProperty("lastConnectedFirmwareSignature", signature);
        tsProperties.setProperty("projectsDir", projectsDir);
        tsProperties.setProperty("recentlyOpenedProject_0", projectPath);
        tsProperties.setProperty("loadLastProjectOnStart2", "true");
        new File(TsHelper.TS_ROOT).mkdirs();
        try (FileOutputStream fos = new FileOutputStream(TsHelper.TS_USER_FILE)) {
            tsProperties.store(fos, "rusEFI sandbox");
        }

        System.out.println("Created TS project at " + projectPath);
    }

    private static String normalizeSerialPort(String port) {
        if (port.startsWith("tty") && !port.contains("/")) {
            return "/dev/" + port;
        }
        return port;
    }

    private static UUID ecuUidFrom(CalibrationsInfo calibrations) {
        IniFileModel ini = calibrations.getIniFile();
        IniField uid1 = ini.getAllIniFields().get("device_uid1");
        if (uid1 == null) {
            return UUID.randomUUID();
        }
        ConfigurationImage image = calibrations.getImage().getConfigurationImage();
        byte[] uidBytes = new byte[12];
        System.arraycopy(image.getContent(), uid1.getOffset(), uidBytes, 0, 12);
        return UUID.nameUUIDFromBytes(uidBytes);
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
