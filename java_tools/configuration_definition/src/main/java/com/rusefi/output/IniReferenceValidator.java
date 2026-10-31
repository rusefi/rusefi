package com.rusefi.output;

import java.nio.file.Path;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/** Symbol references in the generated ECU definition (not an expression evaluator).
 * See EFI Analytics ECU Definition Files, sections 5, 12-15 and 22-23.
 */
final class IniReferenceValidator {
    private static final Pattern WORD = Pattern.compile("[A-Za-z_][A-Za-z_0-9]*(?:\\.[A-Za-z_][A-Za-z_0-9]*)?");
    private static final Pattern NUMBER = Pattern.compile("(?:0[xX][0-9a-fA-F]+|0[bB][01]+|(?:[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+)(?:[eE][+-]?[0-9]+)?)");
    private static final Pattern STRING_FUNCTION = Pattern.compile("\\b(?:bitStringValue|stringValue)\\s*\\(");
    private static final Pattern QUOTED_STRING_FUNCTION = Pattern.compile("\\$(?:bitStringValue|stringValue)\\s*\\(");
    private static final Pattern DIMENSION_EXPRESSION = Pattern.compile("\\{[^{}]*}");
    private static final Set<String> VALUE_TYPES = set("scalar", "array", "bits", "string", "continuousChannelValue", "channelValueOnConnect");
    private static final Set<String> UI_SECTIONS = set("userdefined", "uidialogs");
    private static final Set<String> COMPONENTS = set("dialog", "indicatorpanel", "readoutpanel", "livegraph", "help");
    // Application-owned values: timeNow is TS's clock; true/false are literals.
    private static final Set<String> BUILTIN_VALUES = set("timeNow", "true", "false");
    private static final Set<String> STANDARD_PANELS = set("std_injection", "std_realtime", "std_accel", "std_ms3Rtc",
            "std_ms3SdConsole", "std_ms2gentherm", "std_ms2geno2", "std_constants", "std_warmup", "std_port_edit", "std_trigwiz");

    private final Path file;
    private final List<Row> rows = new ArrayList<>();
    private final Set<String> constants = set();
    private final Set<String> channels = set();
    private final Set<String> gauges = set();
    private final Set<String> macros = set();
    private final Map<String, Row> components = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<Integer, Set<String>> loggerFields = new HashMap<>();
    private final Set<String> errors = new LinkedHashSet<>();

    IniReferenceValidator(Path file, List<String> lines) {
        this.file = file;
        String section = "";
        int parentLine = 0;
        int logger = 0;
        for (int i = 0; i < lines.size(); i++) {
            String line = GeneratedIniValidator.withoutComment(lines.get(i)).trim();
            if (line.startsWith("#define ")) {
                Matcher name = WORD.matcher(line.substring(8));
                if (name.find()) {
                    macros.add(name.group());
                }
            }
            if (line.isEmpty() || line.startsWith("#")) {
                continue;
            }
            if (line.startsWith("[") && line.endsWith("]")) {
                section = line.substring(1, line.length() - 1).toLowerCase(Locale.ROOT);
                parentLine = 0;
                continue;
            }
            int equals = line.indexOf('=');
            if (equals < 0) {
                continue;
            }
            String key = line.substring(0, equals).trim();
            String keyword = key.toLowerCase(Locale.ROOT);
            if (UI_SECTIONS.contains(section) && COMPONENTS.contains(keyword)) {
                parentLine = i + 1;
            }
            if (section.equals("loggerdefinition") && keyword.equals("loggerdef")) {
                logger++;
                loggerFields.put(logger, set());
            }
            List<String> args = split(line.substring(equals + 1));
            if (!keyword.equals("settingoption")) {
                args = splitWords(args);
            }
            Row row = new Row(section, key, args, i + 1, parentLine, logger);
            rows.add(row);
            collect(row);
        }
    }

    private static Set<String> set(String... values) {
        Set<String> result = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
        Collections.addAll(result, values);
        return result;
    }

    private void collect(Row row) {
        String type = row.arg(0);
        if ((row.section.equals("constants") || row.section.equals("pcvariables")) && VALUE_TYPES.contains(type)) {
            constants.add(row.key);
        }
        if (row.section.equals("outputchannels") && (VALUE_TYPES.contains(type) || type.startsWith("{"))) {
            channels.add(row.key);
        }
        if (row.section.equals("gaugeconfigurations") && !row.keyword.equals("gaugecategory") && row.args.size() > 1) {
            gauges.add(row.key);
        }
        if ((UI_SECTIONS.contains(row.section) && COMPONENTS.contains(row.keyword))
                || (row.section.equals("curveeditor") && row.keyword.equals("curve"))
                || (row.section.equals("tableeditor") && row.keyword.equals("table"))) {
            components.putIfAbsent(type, row);
            if (row.keyword.equals("table")) {
                components.putIfAbsent(row.arg(1), row); // the table's 3D map alias
            }
        }
        if (row.section.equals("loggerdefinition") && (row.keyword.equals("recordfield") || row.keyword.equals("calcfield"))) {
            loggerFields.computeIfAbsent(row.logger, ignored -> set()).add(type);
        }
    }

    void validate() {
        for (Row row : rows) {
            references(row);
            for (int i = 0; i < row.args.size(); i++) {
                String arg = row.arg(i);
                if (isText(row, i) || arg.startsWith("\"")) {
                    // Newer TS versions also allow $stringValue(...) inside quotes.
                    stringFunctions(row, arg);
                } else if (arg.startsWith("{")) {
                    expression(row, arg);
                } else if (arg.startsWith("[")) {
                    Matcher dimension = DIMENSION_EXPRESSION.matcher(arg);
                    while (dimension.find()) {
                        expression(row, dimension.group());
                    }
                }
                if (arg.startsWith("$") && WORD.matcher(arg.substring(1)).matches() && !macros.contains(arg.substring(1))) {
                    error(row, "Undefined macro [" + arg.substring(1) + "]");
                }
            }
        }
        if (!errors.isEmpty()) {
            throw new IllegalStateException(String.join("\n", errors));
        }
    }

    private void references(Row r) {
        if ((UI_SECTIONS.contains(r.section) || r.section.equals("frontpage"))
                && (r.keyword.equals("indicator") || r.keyword.equals("indicatortemplate"))) {
            data(r, r.arg(0));
        }
        if (r.section.equals("settingcontexthelp")) {
            constant(r, r.key);
        } else if (r.section.equals("constantsextensions") || r.section.equals("constantextensions")) {
            constant(r, r.arg(0));
        } else if (r.section.equals("gaugeconfigurations")) {
            if (gauges.contains(r.key)) {
                data(r, r.arg(0));
            }
        } else if (r.section.equals("datalog") && r.keyword.equals("entry")) {
            data(r, r.arg(0));
        } else if (r.section.equals("pcvariables") && set("continuousChannelValue", "channelValueOnConnect").contains(r.arg(0))) {
            data(r, r.arg(1));
        } else if (r.section.equals("curveeditor") || r.section.equals("tableeditor")) {
            if (set("xbins", "ybins", "zbins").contains(r.keyword)) {
                constant(r, r.arg(0));
                if (!r.arg(1).isEmpty() && !r.arg(1).equalsIgnoreCase("readOnly")) {
                    data(r, r.arg(1));
                }
            } else if (r.keyword.equals("gauge")) {
                gauge(r, r.arg(0));
            } else if (r.keyword.equals("showxydataplot")) {
                data(r, r.arg(1));
                data(r, r.arg(2));
            }
        } else if (UI_SECTIONS.contains(r.section)) {
            switch (r.keyword) {
                case "field": case "checkbox": case "radio": case "canclientidselector":
                    if (r.keyword.equals("field") && !r.arg(0).startsWith("\"") && !r.arg(0).startsWith("{")
                            && (r.arg(1).isEmpty() || r.arg(1).startsWith("{"))) {
                        constant(r, r.arg(0));
                    } else if (!r.arg(1).isEmpty() && !r.arg(1).startsWith("{")) {
                        constant(r, r.arg(1));
                    }
                    break;
                case "runtimevalue": data(r, r.arg(1)); break;
                case "userpassword": constant(r, r.arg(0)); break;
                case "channelselector": constant(r, r.arg(1)); constant(r, r.arg(2)); break;
                case "graphline": data(r, r.arg(0)); break;
                case "gauge": gauge(r, r.arg(0)); break;
                case "readout":
                    if (!gauges.contains(r.arg(0))) {
                        data(r, r.arg(0));
                    }
                    break;
                case "panel": panel(r, r.arg(0), true); break;
                case "settingoption":
                    for (int i = 1; i < r.args.size(); i++) {
                        String assignment = r.arg(i);
                        int equals = assignment.indexOf('=');
                        if (equals >= 0) {
                            constant(r, assignment.substring(0, equals).trim());
                            expression(r, assignment.substring(equals + 1));
                        }
                    }
                    break;
                default: break;
            }
        } else if (r.section.equals("menu")) {
            if (set("submenu", "groupchildmenu", "menudialog").contains(r.keyword)
                    && !set("main", "std_separator").contains(r.arg(0))) {
                panel(r, r.arg(0), false);
            }
        } else if (r.section.equals("keyactions") && r.keyword.equals("showpanel")) {
            panel(r, r.arg(1), false);
        } else if ((r.section.equals("frontpage") || r.section.equals("tuning")) && r.keyword.matches("gauge[0-9]+")) {
            gauge(r, r.arg(0));
        } else if (r.section.equals("veanalyze") || r.section.equals("wueanalyze")) {
            if (r.keyword.equals("veanalyzemap") || r.keyword.equals("wueanalyzemap")) {
                int panels = r.keyword.equals("veanalyzemap") ? 2 : 3;
                for (int i = 0; i < panels; i++) {
                    panel(r, r.arg(i), false);
                }
                for (int i = panels; i < r.args.size(); i++) {
                    data(r, r.arg(i));
                }
            } else if (r.keyword.equals("lambdatargettables")) {
                for (String target : r.args) {
                    if (!target.isEmpty()) {
                        panel(r, target, false);
                    }
                }
            } else if (r.keyword.equals("filter") && r.args.size() > 2) {
                data(r, r.arg(2));
            }
        }
    }

    private void constant(Row r, String name) {
        int index = name.indexOf('[');
        if (index > 0) {
            constant(r, name.substring(0, index));
            expression(r, name.substring(index));
            return;
        }
        if (!constants.contains(name)) {
            error(r, "Undefined constant [" + name + "]");
        }
    }

    private void data(Row r, String name) {
        if (name.startsWith("{") || name.contains("[")) {
            expression(r, name);
        } else if (!channels.contains(name) && !constants.contains(name) && !BUILTIN_VALUES.contains(name)) {
            error(r, "Undefined data point [" + name + "]");
        }
    }

    private void gauge(Row r, String name) {
        if (!gauges.contains(name)) {
            error(r, "Undefined gauge [" + name + "]");
        }
    }

    private void panel(Row r, String name, boolean ordered) {
        // afrTSCustom is the application's editable target table, not an ECU channel.
        if (STANDARD_PANELS.contains(name) || ((r.section.equals("veanalyze") || r.section.equals("wueanalyze"))
                && name.equals("afrTSCustom"))) {
            return;
        }
        Row definition = components.get(name);
        if (definition == null) {
            error(r, "Undefined panel [" + name + "]");
        } else if (ordered && UI_SECTIONS.contains(definition.section) && definition.line >= r.parentLine) {
            error(r, "Panel [" + name + "] defined at line " + definition.line
                    + " must be defined before its parent at line " + r.parentLine);
        }
    }

    private void expression(Row row, String expression) {
        // Consume strings and numbers as tokens so e.g. 1e3, 0xFF and quoted labels
        // cannot create imaginary references. Function names are not data points.
        for (int i = 0; i < expression.length();) {
            char c = expression.charAt(i);
            if (c == '"' || c == '\'') {
                i = quotedEnd(expression, i);
                continue;
            }
            Matcher number = NUMBER.matcher(expression).region(i, expression.length());
            if (number.lookingAt()) {
                i = number.end();
                continue;
            }
            Matcher word = WORD.matcher(expression).region(i, expression.length());
            if (word.lookingAt()) {
                String name = word.group();
                i = word.end();
                int next = i;
                while (next < expression.length() && Character.isWhitespace(expression.charAt(next))) {
                    next++;
                }
                if (next < expression.length() && expression.charAt(next) == '(') {
                    continue;
                }
                if (name.startsWith("array.")) {
                    constant(row, name.substring(6));
                } else if (!(row.section.equals("loggerdefinition")
                        && loggerFields.getOrDefault(row.logger, Collections.emptySet()).contains(name))) {
                    data(row, name);
                }
            } else {
                i++;
            }
        }
    }

    private void stringFunctions(Row row, String text) {
        Matcher matcher = (text.startsWith("\"") ? QUOTED_STRING_FUNCTION : STRING_FUNCTION).matcher(text);
        while (matcher.find()) {
            int begin = matcher.end();
            int end = begin;
            int depth = 1;
            while (end < text.length() && depth != 0) {
                char c = text.charAt(end);
                if (c == '"' || c == '\'') {
                    end = quotedEnd(text, end);
                    continue;
                }
                if (c == '(') {
                    depth++;
                }
                if (c == ')') {
                    depth--;
                }
                end++;
            }
            String arguments = text.substring(begin, depth == 0 ? end - 1 : end);
            List<String> args = split(arguments);
            constant(row, args.get(0));
            expression(row, arguments);
        }
    }

    private static boolean isText(Row r, int index) {
        if (r.section.equals("constants") || r.section.equals("pcvariables") || r.section.equals("outputchannels")) {
            return false;
        }
        switch (r.keyword) {
            case "indicator": case "indicatortemplate": return index == 1 || index == 2;
            case "field": case "runtimevalue": case "checkbox": case "radio":
            case "text": case "help": case "webhelp": case "topichelp":
            case "commandbutton": case "settingselector": return index == 0;
            case "dialog": case "curve": case "livegraph": return index == 1;
            case "table": return index == 2;
            case "columnlabel": case "xylabels": case "linelabel": case "updownlabel": return true;
            case "entry": return index == 1;
            default: return (r.section.equals("gaugeconfigurations") || r.keyword.equals("readout")) && (index == 1 || index == 2);
        }
    }

    private void error(Row row, String message) {
        errors.add(file + ":" + row.line + ": " + message + " (" + row.key + ")");
    }

    // Preserve comma positions and quoted strings, unlike the runtime reader's
    // whitespace tokenizer: expressions/functions may themselves contain commas.
    private static List<String> split(String value) {
        List<String> result = new ArrayList<>();
        int start = 0;
        int depth = 0;
        for (int i = 0; i < value.length(); i++) {
            char c = value.charAt(i);
            if (c == '"' || c == '\'') {
                i = quotedEnd(value, i) - 1;
            } else if (c == '{' || c == '(' || c == '[') {
                depth++;
            } else if (c == '}' || c == ')' || c == ']') {
                depth--;
            } else if (c == ',' && depth == 0) {
                result.add(value.substring(start, i).trim());
                start = i + 1;
            }
        }
        result.add(value.substring(start).trim());
        return result;
    }

    private static int quotedEnd(String text, int start) {
        char quote = text.charAt(start);
        for (int i = start + 1; i < text.length(); i++) {
            if (text.charAt(i) == '\\') {
                i++;
            } else if (text.charAt(i) == quote) {
                return i + 1;
            }
        }
        return text.length();
    }

    private static List<String> splitWords(List<String> commaSeparated) {
        List<String> result = new ArrayList<>();
        for (String value : commaSeparated) {
            int start = 0;
            int depth = 0;
            for (int i = 0; i < value.length(); i++) {
                char c = value.charAt(i);
                if (c == '"' || c == '\'') {
                    i = quotedEnd(value, i) - 1;
                } else if (c == '{' || c == '(' || c == '[') {
                    depth++;
                } else if (c == '}' || c == ')' || c == ']') {
                    depth--;
                } else if (Character.isWhitespace(c) && depth == 0) {
                    if (i > start) {
                        result.add(value.substring(start, i));
                    }
                    start = i + 1;
                }
            }
            if (start < value.length() || value.isEmpty()) {
                result.add(value.substring(start));
            }
        }
        return result;
    }

    private static final class Row {
        final String section, key, keyword;
        final List<String> args;
        final int line, parentLine, logger;

        Row(String section, String key, List<String> args, int line, int parentLine, int logger) {
            this.section = section;
            this.key = key;
            this.keyword = key.toLowerCase(Locale.ROOT);
            this.args = args;
            this.line = line;
            this.parentLine = parentLine;
            this.logger = logger;
        }

        String arg(int index) {
            return index < args.size() ? args.get(index) : "";
        }
    }
}
