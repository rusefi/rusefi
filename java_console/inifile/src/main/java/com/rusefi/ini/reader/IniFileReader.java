package com.rusefi.ini.reader;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
import com.opensr5.ini.field.*;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 12/23/2015.
 */
public class IniFileReader {
    public IniFileModel getIniFileModel() {
        // Finish any pending context help entry before building the model
        finishContextHelp();

        return new ImmutableIniFileModel(metaInfo.getSignature(),
                getBlockingFactor(),
                defines,
                allIniFields,
                secondaryIniFields,
                allOutputChannels,
                protocolMeta,
                getMetaInfo(),
                getIniFilePath(),
                tooltips,
                fieldsInUiOrder,
                dialogs,
                gaugeCategories,
                allGauges,
                topicHelpMap,
                contextHelp,
                allTables);
    }
    private static final Logging log = Logging.getLogging(IniFileReader.class);
    public static final String RUSEFI_INI_PREFIX = "rusefi";
    public static final String RUSEFI_INI_SUFFIX = ".ini";
    public static final String INI_FILE_PATH = System.getProperty("ini_file_path", "..");
    private static final String SECTION_PAGE = "page";
    private static final String FIELD_TYPE_SCALAR = "scalar";
    public static final String FIELD_TYPE_STRING = "string";
    private static final String FIELD_TYPE_ARRAY = "array";
    private static final String FIELD_TYPE_BITS = "bits";

    private final Map<String, List<String>> defines = new TreeMap<>();

    private String dialogId;
    private String dialogUiName;
    private String dialogTopicHelp;
    private final Map<String, DialogModel> dialogs = new TreeMap<>();
    // this is only used while reading model - TODO extract reader
    private final List<DialogModel.Field> fieldsOfCurrentDialog = new ArrayList<>();
    private final List<DialogModel.Command> commandsOfCurrentDialog = new ArrayList<>();
    private final List<PanelModel> panelsOfCurrentDialog = new ArrayList<>();
    private final Map<String, IniField> allIniFields = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<String, IniField> secondaryIniFields = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<String, IniField> allOutputChannels = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    public final Map<String, DialogModel.Field> fieldsInUiOrder = new LinkedHashMap<>();

    private final Map</*field name*/String, String> tooltips = new TreeMap<>();
    private final Map<String, String> protocolMeta = new TreeMap<>();
    private boolean isConstantsSection;
    private boolean isOutputChannelsSection;
    private final IniFileMetaInfo metaInfo;
    private final String iniFilePath;

    private boolean isGaugeConfigurationsSection = false;
    private String currentGaugeCategory = null;
    private final Map<String, GaugeCategoryModel> gaugeCategories = new LinkedHashMap<>();
    private final List<GaugeModel> currentCategoryGauges = new ArrayList<>();
    private final Map<String, GaugeModel> allGauges = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<String, String> topicHelpMap = new TreeMap<>();

    private final Map<String, ContextHelpModel> contextHelp = new LinkedHashMap<>();
    private String currentHelpReferenceName;
    private String currentHelpTitle;
    private final List<String> currentHelpTextLines = new ArrayList<>();
    private String currentHelpWebHelp;

    private boolean isTableEditorSection = false;
    private final Map<String, TableModel> allTables = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final TableBuilder tableBuilder = new TableBuilder();

    private boolean isInSettingContextHelp = false;
    private boolean isInsidePageDefinition;
    private int blockingFactor;
    // useful when connecting remotely via TCP/IP, if CUSTOM_TS_BUFFER_SIZE is available
	// For proteus_f7 over TCP/IP recommended to set blockingFactorOverride=32000
	// java -jar -DblockingFactorOverride=32000 rusefi_console.jar host:port
    private static final Integer blockingFactorOverride = Integer.getInteger("blockingFactorOverride");

    static {
        if (blockingFactorOverride != null)
            log.info("blockingFactorOverride=" + blockingFactorOverride);
    }

    private int currentPageIndex;

    IniFileReader(@Nullable final IniFileMetaInfo metaInfo, final String iniFilePath) {
        this.metaInfo = metaInfo;
        this.iniFilePath = iniFilePath;
    }

    public int getBlockingFactor() {
        if (blockingFactorOverride != null)
            return blockingFactorOverride;
        return blockingFactor;
    }


    public IniFileMetaInfo getMetaInfo() {
        // pageSize lives inside!
        return Objects.requireNonNull(metaInfo, "metaInfo");
    }

    public String getIniFilePath() {
        return Objects.requireNonNull(iniFilePath, "iniFilePath");
    }

    void finishDialog() {
        if (fieldsOfCurrentDialog.isEmpty() && commandsOfCurrentDialog.isEmpty() && panelsOfCurrentDialog.isEmpty())
            return;
        if (dialogUiName == null)
            dialogUiName = dialogId;
        // Store dialogs by their key (dialogId), not by UI name, for easier panel resolution
        dialogs.put(dialogId, new DialogModel(dialogId, dialogUiName, fieldsOfCurrentDialog, commandsOfCurrentDialog, panelsOfCurrentDialog, dialogTopicHelp));

        dialogId = null;
        dialogTopicHelp = null;
        fieldsOfCurrentDialog.clear();
        commandsOfCurrentDialog.clear();
        panelsOfCurrentDialog.clear();
    }

    void handleLine(RawIniFile.Line line) {

        String rawText = line.getRawText();
        try {
            LinkedList<String> list = new LinkedList<>(Arrays.asList(line.getTokens()));
            if (!list.isEmpty() && list.get(0).equals("#define")) {
                defines.put(list.get(1), list.subList(2, list.size()));
                return;
            }

            if (!list.isEmpty() && list.get(0).equals(SECTION_PAGE)) {
                if (list.size() >= 2) {
                    currentPageIndex = Integer.parseInt(list.get(1));
                }
                isInsidePageDefinition = true;
                return;
            }

            // todo: use TSProjectConsumer constant
            if (isInSettingContextHelp) {
                // todo: use TSProjectConsumer constant
                if (rawText.contains("SettingContextHelpEnd")) {
                    finishContextHelp();
                    isInSettingContextHelp = false;
                }
                handleSettingContextHelpLine(list);
                return;
            }

            if (RawIniFile.Line.isCommentLine(rawText))
                return;

            if (RawIniFile.Line.isPreprocessorDirective(rawText))
                return;

            trim(list);

            if (list.isEmpty())
                return;

            String first = list.getFirst();

            if (first.equalsIgnoreCase("blockingFactor")) {
                blockingFactor = Integer.parseInt(list.get(1));
            }


            if (first.startsWith("[") && first.endsWith("]")) {
                log.info("Section " + first);

                boolean wasGaugeSection = isGaugeConfigurationsSection;
                boolean wasTableSection = isTableEditorSection;

                if (first.contains("[SettingContextHelp]")) {
                    isInsidePageDefinition = false;
                    isInSettingContextHelp = true;
                }
                isConstantsSection = first.equals("[Constants]");
                isOutputChannelsSection = first.equals("[OutputChannels]");
                isGaugeConfigurationsSection = first.equalsIgnoreCase("[GaugeConfigurations]");
                isTableEditorSection = first.equalsIgnoreCase("[TableEditor]");

                if (wasGaugeSection && !isGaugeConfigurationsSection) {
                    finishGaugeCategory();
                }
                if (wasTableSection && !isTableEditorSection) {
                    finishTable();
                }
            }

            if (isConstantsSection) {
                if (isInsidePageDefinition) {
                    if (list.size() > 1)
                        handleConstantFieldDefinition(list, line);
                    return;
                } else {
                    if (list.size() > 1) {
                        protocolMeta.put(list.get(0), list.get(1));
                    }
                }
            } else if (isOutputChannelsSection) {
                handleOutputChannelDefinition(list);
                return;
            } else if (isGaugeConfigurationsSection) {
                handleGaugeConfiguration(list);
                return;
            } else if (isTableEditorSection) {
                tableBuilder.handleLine(list, this::addField, this::finishTable);
                return;
            }


            switch (first) {
                case "field":
                    handleField(list);
                    break;
                case "commandButton":
                    handleCommand(list);
                    break;
                case "slider":
                    handleSlider(list);
                    break;
                case "dialog":
                    handleDialog(list);
                    break;
                case "panel":
                    handlePanel(list);
                    break;
                case "topicHelp":
                    handleTopicHelp(list);
                    break;
                case "help":
                    // Handle help entries anywhere (they can appear after dialogs)
                    handleHelpEntry(list);
                    break;
                case "text":
                    // Handle text lines for help entries
                    handleHelpText(list);
                    break;
                case "webHelp":
                    // Handle webHelp for help entries
                    handleHelpWebHelp(list);
                    break;
                case "xBins":
                case "yBins":
                    // Handle xBins/yBins for CurveEditor and other non-TableEditor sections
                    // In TableEditor section, these are handled by TableBuilder
                    handleBins(list);
                    break;
            }
        } catch (RuntimeException e) {
            throw new IllegalStateException("Failed to handle [" + rawText + "]: " + e, e);
        }
    }

    private void handleOutputChannelDefinition(LinkedList<String> list) {
        if (list.size() < 2)
            return;
        String name = list.get(0);
        String channelType = list.get(1);
        switch (channelType) {
            case FIELD_TYPE_SCALAR: {
                String scalarType = list.get(2);
                int offset = Integer.parseInt(list.get(3));
                // todo: reuse ScalarIniField#parse but would need changes?
                allOutputChannels.put(name, new ScalarIniField(name, offset, scalarType, null, 1, "0", 0));
            }
        }
    }

    private void addField(String key) {
        DialogModel.Field field = new DialogModel.Field(key, key);
        fieldsInUiOrder.put(key, field);
    }

    private void handleBins(LinkedList<String> list) {
        // Handle xBins/yBins for CurveEditor and other sections (not TableEditor)
        // Just register the constant name in fieldsInUiOrder
        if (list.size() >= 2) {
            list.removeFirst(); // remove "xBins" or "yBins"
            String binsConstant = list.removeFirst();
            addField(binsConstant);
        }
    }

    private void handleConstantFieldDefinition(LinkedList<String> list, RawIniFile.Line line) {
        switch (list.get(1)) {
            case FIELD_TYPE_SCALAR:
                registerField(ScalarIniField.parse(list));
                break;
            case FIELD_TYPE_STRING:
                registerField(StringIniField.parse(list));
                break;
            case FIELD_TYPE_ARRAY:
                registerField(ArrayIniField.parse(list));
                break;
            case FIELD_TYPE_BITS:
                registerField(EnumIniReaderHelper.parse(list, line, defines));
                break;
            default:
                throw new IllegalStateException("Unexpected " + list);
        }
    }

    private void registerField(IniField field) {
        if (currentPageIndex != 1) {
            log.info("Skipping field from secondary page: " + field);
            secondaryIniFields.put(field.getName(), field);
            return;
        }
        if (allIniFields.containsKey(field.getName()))
            return;
        allIniFields.put(field.getName(), field);
    }

    private void handleSlider(LinkedList<String> list) {
        list.removeFirst(); // "slider"

        String uiFieldName = list.isEmpty() ? "" : list.removeFirst();

        String key = list.isEmpty() ? null : list.removeFirst();

        registerUiField(key, uiFieldName);
        log.debug("IniFileModel: Slider label=[" + uiFieldName + "] : key=[" + key + "]");
    }

    private void handleCommand(LinkedList<String> list) {
        list.removeFirst(); // "commandButton"
        String uiName = list.removeFirst();
        String command = list.removeFirst();
        commandsOfCurrentDialog.add(new DialogModel.Command(uiName, command));
    }

    private void handleField(LinkedList<String> list) {
        list.removeFirst(); // "field"

        String uiFieldName = list.isEmpty() ? "" : list.removeFirst();

        String key = list.isEmpty() ? null : list.removeFirst();

        registerUiField(key, uiFieldName);
        log.debug("IniFileModel: Field label=[" + uiFieldName + "] : key=[" + key + "]");
    }

    private void registerUiField(String key, String uiFieldName) {
        DialogModel.Field field = new DialogModel.Field(key, uiFieldName);

        if (key != null) {
            fieldsOfCurrentDialog.add(field);
            // If the field hasn't been registered yet,
            //  or if it has been but without a UI name (name will be the same as the key)
            // This isn't necessarily more correct, but it's more likely to be correct in dialogs that are more user-visible
            if (! fieldsInUiOrder.containsKey(key) || fieldsInUiOrder.get(key).getUiName() == key) {
                fieldsInUiOrder.put(key, field);
            }
        }
    }

    private void handleDialog(LinkedList<String> list) {
        finishDialog();
        list.removeFirst(); // "dialog"
        //                    trim(list);
        String keyword = list.removeFirst();
//                    trim(list);
        String name = list.isEmpty() ? null : list.removeFirst();

        dialogId = keyword;
        dialogUiName = name;
        log.debug("IniFileModel: Dialog key=" + keyword + ": name=[" + name + "]");
    }

    private void handlePanel(LinkedList<String> list) {
        list.removeFirst(); // "panel"

        String panelName = list.isEmpty() ? null : list.removeFirst();
        String placement = list.isEmpty() ? null : list.removeFirst();
        String enableExpression = list.isEmpty() ? null : list.removeFirst();
        String visibleExpression = list.isEmpty() ? null : list.removeFirst();

        if (panelName != null) {
            PanelModel panel = new PanelModel(panelName, placement, enableExpression, visibleExpression);
            panelsOfCurrentDialog.add(panel);
            log.debug("IniFileModel: Panel name=[" + panelName + "] placement=[" + placement + "]");
        }
    }

    private void handleTopicHelp(LinkedList<String> list) {
        list.removeFirst(); // "topicHelp"
        if (!list.isEmpty()) {
            String topicHelpValue = list.removeFirst();
            dialogTopicHelp = topicHelpValue;

            // Store topicHelp in the map with the current dialog ID as key
            if (dialogId != null) {
                topicHelpMap.put(dialogId, topicHelpValue);
                log.debug("IniFileModel: topicHelp=[" + topicHelpValue + "] for dialog=[" + dialogId + "]");
            }
        }
    }

    private void trim(LinkedList<String> list) {
        while (!list.isEmpty() && list.getFirst().isEmpty())
            list.removeFirst();
    }

    private void handleGaugeConfiguration(LinkedList<String> list) {
        if (list.size() < 2)
            return;

        String gaugeName = list.get(0);

        if (gaugeName.equalsIgnoreCase("gaugeCategory")) {
            finishGaugeCategory();
            currentGaugeCategory = list.get(1);
            return;
        }

        // Gauge definition requires at minimum: name, channel, title, units, lowValue, highValue (6 fields)
        if (list.size() < 6) {
            log.warn("Skipping incomplete gauge definition: " + gaugeName + " (expected at least 6 fields, got " + list.size() + ")");
            return;
        }

        try {
            GaugeModel gauge = getGaugeModel(list, gaugeName);

            currentCategoryGauges.add(gauge);
            allGauges.put(gaugeName, gauge);

        } catch (NumberFormatException e) {
            log.warn("Failed to parse gauge: " + gaugeName + ": " + e.getMessage());
        } catch (IndexOutOfBoundsException e) {
            log.warn("Failed to parse gauge: " + gaugeName + ": " + e.getMessage());
        }
    }

    private @NotNull GaugeModel getGaugeModel(LinkedList<String> list, String gaugeName) {
        String channel = list.get(1);
        // Title and units: parse as string or expression (never numeric)
        IniValue title = IniValue.parseString(list.get(2));
        IniValue units = IniValue.parseString(list.get(3));
        // Numeric fields can be either numbers or expressions
        IniValue lowValue = IniValue.parseNumeric(list.get(4));
        IniValue highValue = IniValue.parseNumeric(list.get(5));

        // For optional fields, use the same value as lowValue/highValue if not specified
        IniValue lowDangerValue = list.size() > 6 ? IniValue.parseNumeric(list.get(6)) : lowValue;
        IniValue lowWarningValue = list.size() > 7 ? IniValue.parseNumeric(list.get(7)) : lowValue;
        IniValue highWarningValue = list.size() > 8 ? IniValue.parseNumeric(list.get(8)) : highValue;
        IniValue highDangerValue = list.size() > 9 ? IniValue.parseNumeric(list.get(9)) : highValue;
        IniValue valueDecimalPlaces = list.size() > 10 ? IniValue.parseNumeric(list.get(10)) : IniValue.ofNumeric(0);
        IniValue labelDecimalPlaces = list.size() > 11 ? IniValue.parseNumeric(list.get(11)) : IniValue.ofNumeric(0);

        return new GaugeModel(gaugeName, channel, title, units,
                lowValue, highValue, lowDangerValue, lowWarningValue, highWarningValue, highDangerValue,
                valueDecimalPlaces, labelDecimalPlaces);
    }

    void finishGaugeCategory() {
        if (currentGaugeCategory != null && !currentCategoryGauges.isEmpty()) {
            gaugeCategories.put(currentGaugeCategory,
                    new GaugeCategoryModel(currentGaugeCategory, new ArrayList<>(currentCategoryGauges)));
            currentCategoryGauges.clear();
        }
        currentGaugeCategory = null;
    }

    private void handleSettingContextHelpLine(LinkedList<String> list) {
        if (list.isEmpty())
            return;

        String first = list.get(0);

        // Skip comment
        if (first.startsWith(";"))
            return;

        // help = referenceName, "Title"
        if (first.equals("help") && list.size() >= 3) {
            finishContextHelp();
            currentHelpReferenceName = list.get(1);
            currentHelpTitle = list.get(2);
            return;
        }

        // text = "text line"
        if (first.equals("text") && list.size() >= 2 && currentHelpReferenceName != null) {
            currentHelpTextLines.add(list.get(1));
            return;
        }

        // webHelp = "url"
        if (first.equals("webHelp") && list.size() >= 2 && currentHelpReferenceName != null) {
            currentHelpWebHelp = list.get(1);
            return;
        }

        // Simple tooltip entries: fieldName = "tooltip text"
        // Only process as tooltip if not help/text/webHelp and has exactly 2 elements
        if (list.size() == 2) {
            tooltips.put(first, list.get(1));
        }
    }

    void finishContextHelp() {
        if (currentHelpReferenceName != null) {
            ContextHelpModel helpModel = new ContextHelpModel(
                    currentHelpReferenceName,
                    currentHelpTitle,
                    currentHelpTextLines,
                    currentHelpWebHelp
            );
            contextHelp.put(currentHelpReferenceName, helpModel);

            currentHelpReferenceName = null;
            currentHelpTitle = null;
            currentHelpTextLines.clear();
            currentHelpWebHelp = null;
        }
    }

    private void handleHelpEntry(LinkedList<String> list) {
        if (list.size() >= 3) {
            finishContextHelp();
            currentHelpReferenceName = list.get(1);
            currentHelpTitle = list.get(2);
        }
    }

    private void handleHelpText(LinkedList<String> list) {
        if (list.size() >= 2 && currentHelpReferenceName != null) {
            currentHelpTextLines.add(list.get(1));
        }
    }

    private void handleHelpWebHelp(LinkedList<String> list) {
        if (list.size() >= 2 && currentHelpReferenceName != null) {
            currentHelpWebHelp = list.get(1);
        }
    }

    private double parseDouble(String s) {
        return IniField.parseDouble(s);
    }

    private int parseInt(String s) {
        return (int) IniField.parseDouble(s);
    }

    void finishTable() {
        // Create table if we have all required fields
        if (tableBuilder.isComplete()) {
            TableModel table = tableBuilder.build();
            allTables.put(tableBuilder.getTableId(), table);
            String tableId = tableBuilder.getTableId();
            if (tableId != null && !tableId.isEmpty()) {
                allTables.put(tableId, table);
            }
        }
        tableBuilder.reset();
    }
}
