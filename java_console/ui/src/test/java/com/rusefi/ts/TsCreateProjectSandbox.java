package com.rusefi.ts;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.FrontPageModel;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniValue;
import com.opensr5.ini.field.IniField;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.core.FileUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.tune.xml.Msq;

import java.io.*;
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

        // TS projects always have these sibling dirs.
        for (String dir : new String[]{"dashboard", "DataLogs", "inc", "TuneView", "restorePoints"}) {
            new File(projectPath + File.separator + dir).mkdirs();
        }

        FileUtil.copyFile(sourceIniPath, projectPath + File.separator + TsHelper.MAIN_CONTROLLER_PATH);
        calibrations.generateMsq().writeXmlFile(projectPath + File.separator + TsHelper.CURRENT_TUNE_MSQ);
        writeMinimalDashboard(
            projectPath + File.separator + "dashboard" + File.separator + "dashboard.dash",
            signature, calibrations);

        Properties projectProperties = new Properties();
        projectProperties.setProperty("projectName", projectName);
        projectProperties.setProperty("projectDescription", "");
        projectProperties.setProperty("ecuConfigFile", "mainController.ini");
        projectProperties.setProperty("ecuUid", ecuUidFrom(calibrations).toString());
        projectProperties.setProperty("ecuSettings", "");
        projectProperties.setProperty("firmwareDescription", signature);
        projectProperties.setProperty("lastDisplayedTuneFile", "");
        // Points at the minimal <dsh/> stub we write below. TS accepts the well-formed-but-empty
        // cluster and regenerates its real default dashboard on first save.
        projectProperties.setProperty("dashBoardFile", "/dashboard//dashboard.dash");
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

    private static void writeMinimalDashboard(String path, String signature, CalibrationsInfo calibrations) throws IOException {
        // Template is the "skeleton" — font/bezel/needle image blobs + <gaugeCluster> wrapper +
        // one gauge and one indicator block we borrow as prototypes. We strip those existing
        // dashComps and regenerate them from the ini's [FrontPage] + [GaugeConfigurations], so
        // the ini is the single source of truth — edit the ini and the dashboard updates.
        try (java.io.InputStream in = TsCreateProjectSandbox.class
            .getResourceAsStream("/com/rusefi/ts/dashboard_template.dash")) {
            if (in == null) {
                throw new IOException("dashboard_template.dash missing from classpath");
            }

            byte[] template = new byte[in.available()];
            DataInputStream dataInputStream = new DataInputStream(in);
            dataInputStream.readFully(template);

            String xml = new String(template, java.nio.charset.StandardCharsets.UTF_8)
                .replaceFirst("firmwareSignature=\"[^\"]*\"",
                    "firmwareSignature=\"" + java.util.regex.Matcher.quoteReplacement(signature) + "\"")
                .replaceFirst("writeDate=\"[^\"]*\"",
                    "writeDate=\"" + java.util.regex.Matcher.quoteReplacement(new java.util.Date().toString()) + "\"");

            xml = rebuildDashComps(xml, calibrations);

            try (FileOutputStream fos = new FileOutputStream(path)) {
                fos.write(xml.getBytes(java.nio.charset.StandardCharsets.UTF_8));
            }
        }
    }

    private static final java.util.regex.Pattern GAUGE_BLOCK = java.util.regex.Pattern.compile(
        "(?s)[ \\t]*<dashComp type=\"Gauge\">.*?</dashComp>\\s*\\n");
    private static final java.util.regex.Pattern INDICATOR_BLOCK = java.util.regex.Pattern.compile(
        "(?s)[ \\t]*<dashComp type=\"Indicator\">.*?</dashComp>\\s*\\n");

    // Layout constants — shared between gauge and indicator builders to keep them aligned.
    private static final double INDICATOR_WIDTH = 0.0803;
    private static final double INDICATOR_HEIGHT = 0.028;
    private static final double INDICATOR_BOTTOM_Y = 0.9675;
    private static final int INDICATORS_PER_ROW =
        Math.max(1, (int) Math.floor(1.0 / INDICATOR_WIDTH));
    private static final double GAUGE_START_Y = 0.005;
    private static final double GAUGE_STRIP_GAP = 0.005; // padding between bottom gauge row and top indicator row

    private static String rebuildDashComps(String xml, CalibrationsInfo calibrations) {
        IniFileModel ini = calibrations.getIniFile();
        ConfigurationImage image = calibrations.getImage().getConfigurationImage();
        FrontPageModel frontPage = ini.getFrontPage();
        if (frontPage == null) {
            return xml;
        }

        // Pull the first gauge and first indicator block as prototypes — we inherit all the styling
        // defaults (bezel, needle, colors, sweep, font) by cloning and overriding ini-derived fields.
        java.util.regex.Matcher gm = GAUGE_BLOCK.matcher(xml);
        String gaugePrototype = gm.find() ? gm.group() : null;
        java.util.regex.Matcher im = INDICATOR_BLOCK.matcher(xml);
        String indicatorPrototype = im.find() ? im.group() : null;
        if (gaugePrototype == null || indicatorPrototype == null) {
            return xml;
        }

        // Strip every existing dashComp so we re-emit only what the ini specifies.
        xml = GAUGE_BLOCK.matcher(xml).replaceAll("");
        xml = INDICATOR_BLOCK.matcher(xml).replaceAll("");

        java.util.List<IndicatorModel> indicators = frontPage.getIndicators();
        int indicatorRows = Math.max(1, (int) Math.ceil(indicators.size() / (double) INDICATORS_PER_ROW));

        StringBuilder generated = new StringBuilder();
        java.util.List<String> gaugeNames = frontPage.getGaugeNames();
        for (int i = 0; i < gaugeNames.size(); i++) {
            GaugeModel g = ini.getGauge(gaugeNames.get(i));
            if (g != null) {
                generated.append(buildGauge(gaugePrototype, i, g, ini, image, indicatorRows));
            }
        }
        for (int i = 0; i < indicators.size(); i++) {
            generated.append(buildIndicator(indicatorPrototype, i, indicators.size(), indicators.get(i)));
        }

        return xml.replace("</gaugeCluster>", generated + "    </gaugeCluster>");
    }

    private static String buildGauge(String prototype, int index, GaugeModel g, IniFileModel ini, ConfigurationImage image, int indicatorRows) {
        // 4x2 grid sized to fit above the indicator strip at the bottom of the cluster.
        double indicatorStripTop = INDICATOR_BOTTOM_Y - (indicatorRows - 1) * INDICATOR_HEIGHT;
        double gaugeRelH = (indicatorStripTop - GAUGE_STRIP_GAP - GAUGE_START_Y) / 2.0;
        double relW = 0.247;
        double relX = 0.0015 + (index % 4) * 0.25;
        double relY = GAUGE_START_Y + (index / 4) * gaugeRelH;
        double relH = gaugeRelH;

        String block = prototype;
        block = setText(block, "Title", "String", xmlEscape(g.getTitle()));
        block = setText(block, "Units", "String", xmlEscape(g.getUnits()));
        block = setText(block, "OutputChannel", "String", xmlEscape(g.getChannel()));
        block = setText(block, "MaxVP", "ValueProvider", xmlEscape(stripBraces(g.getHighValueValue().getRawString())));
        block = setText(block, "MinVP", "ValueProvider", xmlEscape(stripBraces(g.getLowValueValue().getRawString())));
        block = setText(block, "HighCriticalVP", "ValueProvider", xmlEscape(stripBraces(g.getHighDangerValueValue().getRawString())));
        block = setText(block, "HighWarningVP", "ValueProvider", xmlEscape(stripBraces(g.getHighWarningValueValue().getRawString())));
        block = setText(block, "LowCriticalVP", "ValueProvider", xmlEscape(stripBraces(g.getLowDangerValueValue().getRawString())));
        block = setText(block, "LowWarningVP", "ValueProvider", xmlEscape(stripBraces(g.getLowWarningValueValue().getRawString())));
        block = setNumeric(block, "Max", g.getHighValueValue(), ini, image);
        block = setNumeric(block, "DefaultMax", g.getHighValueValue(), ini, image);
        block = setNumeric(block, "Min", g.getLowValueValue(), ini, image);
        block = setNumeric(block, "DefaultMin", g.getLowValueValue(), ini, image);
        block = setNumeric(block, "HighCritical", g.getHighDangerValueValue(), ini, image);
        block = setNumeric(block, "HighWarning", g.getHighWarningValueValue(), ini, image);
        block = setNumeric(block, "LowCritical", g.getLowDangerValueValue(), ini, image);
        block = setNumeric(block, "LowWarning", g.getLowWarningValueValue(), ini, image);
        block = setInt(block, "ValueDigits", g.getValueDecimalPlaces());
        block = setInt(block, "LabelDigits", g.getLabelDecimalPlaces());
        block = setDouble(block, "RelativeX", relX);
        block = setDouble(block, "RelativeY", relY);
        block = setDouble(block, "RelativeWidth", relW);
        block = setDouble(block, "RelativeHeight", relH);
        // leftovers from the template that TS rewrites on first save
        block = block.replaceAll("<EcuConfigurationName type=\"String\">[^<]*</EcuConfigurationName>",
            "<EcuConfigurationName type=\"String\"/>");
        block = block.replace("<LongClickAction type=\"String\">null</LongClickAction>",
            "<LongClickAction type=\"String\"/>");
        block = block.replace("<ShortClickAction type=\"String\">null</ShortClickAction>",
            "<ShortClickAction type=\"String\"/>");
        return block;
    }

    private static String buildIndicator(String prototype, int index, int total, IndicatorModel ind) {
        // Match TS's default indicator metrics. Rows fill top-down with the last row anchored
        // at the bottom of the cluster.
        int totalRows = Math.max(1, (int) Math.ceil((double) total / INDICATORS_PER_ROW));
        int row = index / INDICATORS_PER_ROW;
        int col = index % INDICATORS_PER_ROW;
        double relY = INDICATOR_BOTTOM_Y - (totalRows - 1 - row) * INDICATOR_HEIGHT;
        double relX = col * INDICATOR_WIDTH;
        double relW = INDICATOR_WIDTH;
        double relH = INDICATOR_HEIGHT;

        String block = prototype;
        block = setText(block, "OffText", "String", xmlEscape(formatIndicatorLabel(ind.getOffLabel())));
        block = setText(block, "OnText", "String", xmlEscape(formatIndicatorLabel(ind.getOnLabel())));
        // TS encodes comparison expressions as <name><OP><value>_OC for use as a ValueProvider
        // channel. A raw `x != 0` won't resolve; `xNOTEQ0_OC` will.
        block = setText(block, "OutputChannel", "String", xmlEscape(encodeIndicatorChannel(ind.getExpression())));
        block = setColor(block, "OffBackgroundColor", ind.getOffBg(), 0xFFFFFFFF);
        block = setColor(block, "OffTextColor", ind.getOffFg(), 0xFF000000);
        block = setColor(block, "OnBackgroundColor", ind.getOnBg(), 0xFF00FF00);
        block = setColor(block, "OnTextColor", ind.getOnFg(), 0xFF000000);
        block = setDouble(block, "RelativeX", relX);
        block = setDouble(block, "RelativeY", relY);
        block = setDouble(block, "RelativeWidth", relW);
        block = setDouble(block, "RelativeHeight", relH);
        // Indicators (unlike gauges) keep LongClickAction/ShortClickAction as literal "null" —
        // don't normalize. Only clear the stale EcuConfigurationName value from the template.
        block = block.replaceAll("<EcuConfigurationName type=\"String\">[^<]*</EcuConfigurationName>",
            "<EcuConfigurationName type=\"String\"/>");
        return block;
    }

    /**
     * TS writes indicator OutputChannels as ValueProvider names. Simple variable references pass
     * through; comparison expressions get encoded: "x != 0" → "xNOTEQ0_OC", "x == 0" → "xEQ0_OC",
     * "x >= 0" → "xGTE0_OC", etc. All whitespace is stripped, then the operator is substituted.
     */
    private static String encodeIndicatorChannel(String raw) {
        if (raw == null) return "";
        String s = stripBraces(raw).replaceAll("\\s+", "");
        if (s.isEmpty()) return "";
        // 2-char operators first, so ">=" is not mis-matched as ">".
        String[][] ops = {
            {"!=", "NOTEQ"},
            {"==", "EQ"},
            {">=", "GTE"},
            {"<=", "LTE"},
            {">",  "GT"},
            {"<",  "LT"},
        };
        for (String[] op : ops) {
            if (s.contains(op[0])) {
                return s.replace(op[0], op[1]) + "_OC";
            }
        }
        return s;
    }

    private static String setText(String block, String tag, String type, String value) {
        return block.replaceFirst(
            "<" + tag + " type=\"" + type + "\">[^<]*</" + tag + ">",
            java.util.regex.Matcher.quoteReplacement(
                "<" + tag + " type=\"" + type + "\">" + value + "</" + tag + ">"));
    }

    private static String setDouble(String block, String tag, double value) {
        return block.replaceFirst(
            "<" + tag + " type=\"double\">[^<]*</" + tag + ">",
            "<" + tag + " type=\"double\">" + value + "</" + tag + ">");
    }

    private static String setInt(String block, String tag, int value) {
        return block.replaceFirst(
            "<" + tag + " type=\"int\">[^<]*</" + tag + ">",
            "<" + tag + " type=\"int\">" + value + "</" + tag + ">");
    }

    private static String setNumeric(String block, String tag, IniValue v, IniFileModel ini, ConfigurationImage image) {
        Double resolved;
        if (v.isNumeric()) {
            resolved = v.getNumericValue();
        } else if (v.isExpression()) {
            resolved = ExpressionEvaluator.evaluateNumericExpression(v.getRawString(), ini, image);
        } else {
            resolved = null;
        }
        double value = (resolved == null || Double.isNaN(resolved)) ? 0.0 : resolved;
        return setDouble(block, tag, value);
    }

    private static String setColor(String block, String tag, String colorName, int fallbackArgb) {
        int argb = colorNameToArgb(colorName, fallbackArgb);
        int a = (argb >> 24) & 0xFF;
        int r = (argb >> 16) & 0xFF;
        int gr = (argb >> 8) & 0xFF;
        int b = argb & 0xFF;
        String replacement = "<" + tag + " alpha=\"" + a + "\" blue=\"" + b + "\" green=\"" + gr
            + "\" red=\"" + r + "\" type=\"Color\">" + argb + "</" + tag + ">";
        return block.replaceFirst(
            "<" + tag + "[^>]*type=\"Color\">[^<]*</" + tag + ">",
            java.util.regex.Matcher.quoteReplacement(replacement));
    }

    private static int colorNameToArgb(String name, int fallback) {
        if (name == null) return fallback;
        switch (name.trim().toLowerCase(java.util.Locale.ROOT)) {
            case "white":   return 0xFFFFFFFF;
            case "black":   return 0xFF000000;
            case "red":     return 0xFFFF0000;
            case "green":   return 0xFF00FF00;
            case "blue":    return 0xFF0000FF;
            case "yellow":  return 0xFFFFFF00;
            case "cyan":    return 0xFF00FFFF;
            case "magenta": return 0xFFFF00FF;
            case "orange":  return 0xFFFFA500;
            case "gray":
            case "grey":    return 0xFF808080;
            default:        return fallback;
        }
    }

    private static String xmlEscape(String s) {
        if (s == null) return "";
        return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")
            .replace("\"", "&quot;");
    }

    private static String stripBraces(String s) {
        if (s == null) return "";
        s = s.trim();
        if (s.startsWith("{") && s.endsWith("}")) {
            s = s.substring(1, s.length() - 1).trim();
        }
        return s;
    }

    /**
     * Indicator labels may be plain strings ("Fan off") or wrapped expressions
     * ({ Fuel cut: bitStringValue(foo, bar) }). TS renders the latter with spaces around
     * parens and commas, so we mirror that formatting. Plain strings pass through unchanged.
     */
    private static String formatIndicatorLabel(String raw) {
        if (raw == null) return "";
        String s = raw;
        // Strip literal braces without trimming inner whitespace — TS keeps the leading space.
        if (s.length() >= 2 && s.charAt(0) == '{' && s.charAt(s.length() - 1) == '}') {
            s = s.substring(1, s.length() - 1);
            s = s.replace("(", "( ")
                 .replace(")", " )")
                 .replace(",", " , ");
        }
        return s;
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
