package com.rusefi.ts;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.FrontPageModel;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniValue;
import com.rusefi.maintenance.CalibrationsInfo;

import java.io.DataInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Generates a TunerStudio dashboard.dash from a template + the ini's
 * [FrontPage] + [GaugeConfigurations]. Template carries the required
 * font/bezel/needle image blobs; dashComps are regenerated per-call
 * so ini edits propagate straight into the output.
 *
 * See /com/rusefi/ts/dashboard_format.md for the XML schema notes.
 */
public final class DashboardFactory {

    private static final String TEMPLATE_RESOURCE = "/com/rusefi/ts/dashboard_template.dash";

    private static final Pattern GAUGE_BLOCK = Pattern.compile(
        "(?s)[ \\t]*<dashComp type=\"Gauge\">.*?</dashComp>\\s*\\n");
    private static final Pattern INDICATOR_BLOCK = Pattern.compile(
        "(?s)[ \\t]*<dashComp type=\"Indicator\">.*?</dashComp>\\s*\\n");

    // Layout constants — shared between gauge and indicator builders to keep them aligned.
    private static final double INDICATOR_WIDTH = 0.0803;
    private static final double INDICATOR_HEIGHT = 0.028;
    private static final double INDICATOR_BOTTOM_Y = 0.9675;
    private static final int INDICATORS_PER_ROW =
        Math.max(1, (int) Math.floor(1.0 / INDICATOR_WIDTH));
    private static final double GAUGE_START_Y = 0.005;
    private static final double GAUGE_STRIP_GAP = 0.005;

    private DashboardFactory() {}

    /**
     * Write a generated dashboard.dash to the given path. Sanitized signature goes into
     * {@code <versionInfo firmwareSignature>}; gauges + indicators are built from the live
     * ini and config image inside {@code calibrations}.
     */
    public static void writeDashboard(String path, String signature, CalibrationsInfo calibrations) throws IOException {
        try (InputStream in = DashboardFactory.class.getResourceAsStream(TEMPLATE_RESOURCE)) {
            if (in == null) {
                throw new IOException("dashboard_template.dash missing from classpath");
            }

            byte[] template = new byte[in.available()];
            DataInputStream dataInputStream = new DataInputStream(in);
            dataInputStream.readFully(template);

            String xml = new String(template, StandardCharsets.UTF_8)
                .replaceFirst("firmwareSignature=\"[^\"]*\"",
                    "firmwareSignature=\"" + Matcher.quoteReplacement(signature) + "\"")
                .replaceFirst("writeDate=\"[^\"]*\"",
                    "writeDate=\"" + Matcher.quoteReplacement(new Date().toString()) + "\"");

            xml = rebuildDashComps(xml, calibrations);

            try (FileOutputStream fos = new FileOutputStream(path)) {
                fos.write(xml.getBytes(StandardCharsets.UTF_8));
            }
        }
    }

    private static String rebuildDashComps(String xml, CalibrationsInfo calibrations) {
        IniFileModel ini = calibrations.getIniFile();
        ConfigurationImage image = calibrations.getImage().getConfigurationImage();
        FrontPageModel frontPage = ini.getFrontPage();
        if (frontPage == null) {
            return xml;
        }

        // Pull the first gauge and first indicator block as prototypes — we inherit all the styling
        // defaults (bezel, needle, colors, sweep, font) by cloning and overriding ini-derived fields.
        Matcher gm = GAUGE_BLOCK.matcher(xml);
        String gaugePrototype = gm.find() ? gm.group() : null;
        Matcher im = INDICATOR_BLOCK.matcher(xml);
        String indicatorPrototype = im.find() ? im.group() : null;
        if (gaugePrototype == null || indicatorPrototype == null) {
            return xml;
        }

        // Strip every existing dashComp so we re-emit only what the ini specifies.
        xml = GAUGE_BLOCK.matcher(xml).replaceAll("");
        xml = INDICATOR_BLOCK.matcher(xml).replaceAll("");

        List<IndicatorModel> indicators = frontPage.getIndicators();
        int indicatorRows = Math.max(1, (int) Math.ceil(indicators.size() / (double) INDICATORS_PER_ROW));

        StringBuilder generated = new StringBuilder();
        List<String> gaugeNames = frontPage.getGaugeNames();
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
            Matcher.quoteReplacement(
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
            Matcher.quoteReplacement(replacement));
    }

    private static int colorNameToArgb(String name, int fallback) {
        if (name == null) return fallback;
        switch (name.trim().toLowerCase(Locale.ROOT)) {
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
}
