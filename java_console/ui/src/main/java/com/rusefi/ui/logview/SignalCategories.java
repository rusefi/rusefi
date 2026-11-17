package com.rusefi.ui.logview;

import com.opensr5.ini.DatalogEntry;
import com.opensr5.ini.GaugeCategoryModel;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.logview.CsvLogReader;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.regex.Pattern;

/**
 * Sorts a log's signals into a few groups for the signal list. Logs carry no usable categories (ECU logs leave
 * the field empty, console recordings write "Others"), so the groups come from the ECU's .ini when one is
 * available: log label to channel through [Datalog], channel to the gauge categories that show it. Those ~35
 * developer-facing categories are merged into friendly groups; a channel in several categories takes the
 * first group in {@link Group} order. Signals the
 * .ini cannot place, or every signal when there is no .ini, are placed by name rules, else in Other.
 */
final class SignalCategories {
    /** Friendly groups in display order, which is also the preference order between .ini categories. */
    enum Group {
        SENSORS("Sensors"),
        FUEL("Fuel"),
        IGNITION("Ignition"),
        AIR("Air"),
        THROTTLE("Throttle"),
        IDLE("Idle"),
        BOOST("Boost"),
        KNOCK("Knock"),
        VVT("VVT"),
        TRIGGER("Trigger & sync"),
        TRANSMISSION("Transmission"),
        LUA("Lua"),
        CAN("CAN"),
        ECU("ECU status"),
        STATUS("Status & errors"),
        RAW("Raw & debug"),
        OTHER("Other");

        final String title;

        Group(String title) {
            this.title = title;
        }
    }

    /** The signals most logs are opened for, by .ini channel and by log label. */
    private static final String[][] COMMON = {
            {"RPMValue", "RPM"},
            {"TPSValue", "TPS"},
            {"MAPValue", "MAP"},
            {"coolant", "CLT"},
            {"intake", "Intake Air IAT", "IAT"},
            {"lambdaValue", "Lambda"},
            {"AFRValue", "Air/Fuel Ratio", "AFR"},
            {"VBatt", "VBatt"},
            {"correctedIgnitionAdvance", "Ignition: Timing: ignition"},
            {"injectorDutyCycle", "Fuel: injector duty cycle"},
            {"vehicleSpeedKph", "Vehicle Speed"},
    };

    /**
     * .ini gauge category name (lower case) prefixes, first match wins. Overview pages that gather other
     * subsystems' inputs say nothing about a signal and are left out: "AE / Transient fueling" shows RPM, CLT,
     * TPS and MAP, which would otherwise all land in Fuel.
     */
    private static final Object[][] INI_GROUPS = {
            {"ae /", null},
            {"sensors - raw", Group.RAW},
            {"debug", Group.RAW},
            {"test bench", Group.RAW},
            {"sensors", Group.SENSORS},
            {"fuel", Group.FUEL},
            {"acceleration enrichment", Group.FUEL},
            {"gdi", Group.FUEL},
            {"ignition", Group.IGNITION},
            {"timing", Group.IGNITION},
            {"throttle", Group.THROTTLE},
            {"etb", Group.THROTTLE},
            {"idle", Group.IDLE},
            {"boost", Group.BOOST},
            {"knock", Group.KNOCK},
            {"vvt", Group.VVT},
            {"trigger", Group.TRIGGER},
            {"sync", Group.TRIGGER},
            {"transmission", Group.TRANSMISSION},
            {"lua", Group.LUA},
            {"can", Group.CAN},
            {"ecu status", Group.ECU},
            {"alternator", Group.ECU},
    };

    /** Name rules for signals the .ini cannot place, first match wins. */
    private static final Object[][] RULES = {
            {"^(error|warning)\\b|fault|failure|cutreason|errorcode|blinker|^stopengine", Group.STATUS},
            // on/off flags named like isO2HeaterOn, hasFaultReportFile, dfcoActive (case-sensitive)
            {"(?-i)^(is|has|need|allow)[A-Z]|Ready$|Triggered$|Active$", Group.STATUS},
            {"^raw|adc\\b|adc$|^debug|resistance$", Group.RAW},
            // sensor words anywhere in the name: Rear Lambda, Front Cyl Air/Fuel Ratio, Instant MAP, Batt V
            {"^(rpm|clt|iat|intake air|tps\\d?|map|maf|vign|fuel level|egt|aux ?temp|wb\\d|ego|o2)\\b"
                    + "|lambda|air/fuel|\\bafr\\b|baro|batt|\\bmap\\b|map(fast)?$|\\boil\\b|temp(f)?$|imu|wheel|speed|kph|wbo",
                    Group.SENSORS},
            {"etb|throttle|split|pedal|\\bpps\\b|grip|^dc\\b", Group.THROTTLE},
            {"^(fuel|inj|stft|ltft|dfco|flex|gdi|hpfp|accel|ae)\\b|fuel|inject|\\bve\\b|^ve\\d|vetable|currentve"
                    + "|afrtable|stft|ltft|tcharge|wall|maxextra|maxinjected|accumulatedvalue", Group.FUEL},
            {"^(ign|ignition|timing|spark|dwell)\\b|coil|ignitiontable", Group.IGNITION},
            {"^air\\b|airmass|cylinder|charge temp", Group.AIR},
            {"idle|targetrpm", Group.IDLE},
            {"boost|turbo|wastegate", Group.BOOST},
            {"knock", Group.KNOCK},
            {"vvt|cam\\b", Group.VVT},
            {"trigger|^trg|sync|tooth|^drpm", Group.TRIGGER},
            {"gear|^tcu|transmission|clutch|shift", Group.TRANSMISSION},
            {"^lua", Group.LUA},
            {"^can\\b", Group.CAN},
            {"^ecu\\b|uptime|cpu|vref|internal|main relay|^sd\\b|^sd:|usb|tuning|burn|rtc|hardware events"
                    + "|runtime|ontime|running_time|timesince|revolutioncounter|distance|scheduling|outputrequest",
                    Group.ECU},
    };

    private static final List<Map.Entry<Pattern, Group>> COMPILED_RULES = new ArrayList<>();

    static {
        for (Object[] rule : RULES) {
            COMPILED_RULES.add(new AbstractMap.SimpleImmutableEntry<>(
                    Pattern.compile((String) rule[0], Pattern.CASE_INSENSITIVE), (Group) rule[1]));
        }
    }

    private final Group[] groups;
    private final List<Integer> common = new ArrayList<>();
    private final List<String> defaultCommonLabels = new ArrayList<>();

    private SignalCategories(Group[] groups) {
        this.groups = groups;
    }

    /** @param ini the ECU's .ini, or null to use the name rules only */
    static SignalCategories of(CsvLogReader log, IniFileModel ini) {
        Map<String, String> channelOfLabel = new HashMap<>();
        if (ini != null) {
            for (DatalogEntry entry : ini.getDatalogEntries()) {
                channelOfLabel.put(entry.getLabel(), entry.getChannel());
            }
        }
        Map<String, Group> groupOfChannel = groupsByChannel(ini);
        Group[] groups = new Group[log.fieldCount()];
        String[] channels = new String[log.fieldCount()];
        for (int field = 1; field < log.fieldCount(); field++) {
            String label = log.name(field);
            channels[field] = channelOfLabel.getOrDefault(label, label);
            Group fromIni = groupOfChannel.get(channels[field]);
            groups[field] = fromIni != null ? fromIni : ruleGroup(label);
        }
        SignalCategories categories = new SignalCategories(groups);
        for (String[] signal : COMMON) {
            String label = signal[1];
            for (int field = 1; field < log.fieldCount(); field++) {
                if (matches(signal, channels[field], log.name(field)) && !categories.common.contains(field)) {
                    categories.common.add(field);
                    label = log.name(field);
                    break;
                }
            }
            categories.defaultCommonLabels.add(label);
        }
        return categories;
    }

    /** The group of each .ini channel shown by a gauge category; empty without an .ini. */
    static Map<String, Group> groupsByChannel(IniFileModel ini) {
        Map<String, Group> groupOfChannel = new HashMap<>();
        Map<String, GaugeCategoryModel> categories = ini == null ? null : ini.getGaugeCategories();
        if (categories == null) {
            return groupOfChannel;
        }
        for (GaugeCategoryModel category : categories.values()) {
            Group group = iniGroup(category.getName());
            if (group == null) {
                continue;
            }
            for (GaugeModel gauge : category.getGauges()) {
                groupOfChannel.merge(gauge.getChannel(), group,
                        (old, candidate) -> candidate.ordinal() < old.ordinal() ? candidate : old);
            }
        }
        return groupOfChannel;
    }

    private static boolean matches(String[] signal, String channel, String label) {
        if (signal[0].equals(channel)) {
            return true;
        }
        for (int i = 1; i < signal.length; i++) {
            if (signal[i].equalsIgnoreCase(label)) {
                return true;
            }
        }
        return false;
    }

    /** The friendly group of a .ini gauge category, or null for one without a sensible group. */
    static Group iniGroup(String category) {
        String name = category.toLowerCase(Locale.ROOT).trim();
        for (Object[] mapping : INI_GROUPS) {
            if (name.startsWith((String) mapping[0])) {
                return (Group) mapping[1];
            }
        }
        return null;
    }

    static Group ruleGroup(String label) {
        for (Map.Entry<Pattern, Group> rule : COMPILED_RULES) {
            if (rule.getKey().matcher(label).find()) {
                return rule.getValue();
            }
        }
        return Group.OTHER;
    }

    Group group(int field) {
        return groups[field];
    }

    /** The Common signals present in the log, in the default order. */
    List<Integer> common() {
        return common;
    }

    /**
     * Labels of the default Common signals as this log names them, or the usual label of those it lacks: what
     * Common holds once edited, so signals of other logs are not dropped.
     */
    List<String> defaultCommonLabels() {
        return defaultCommonLabels;
    }
}
