package com.opensr5.ini;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.*;
import com.rusefi.core.FindFileHelper;

import java.io.*;
import java.util.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 12/23/2015.
 */
public class IniFileModel {
    private static final Logging log = Logging.getLogging(IniFileModel.class);
    public static final String RUSEFI_INI_PREFIX = "rusefi";
    public static final String RUSEFI_INI_SUFFIX = ".ini";
    public static final String INI_FILE_PATH = System.getProperty("ini_file_path", "..");
    private static final String SECTION_PAGE = "page";
    private static final String FIELD_TYPE_SCALAR = "scalar";
    public static final String FIELD_TYPE_STRING = "string";
    private static final String FIELD_TYPE_ARRAY = "array";
    private static final String FIELD_TYPE_BITS = "bits";

    public Map<String, List<String>> defines = new TreeMap<>();

    private static IniFileModel INSTANCE;
    private String dialogId;
    private String dialogUiName;
    private final Map<String, DialogModel> dialogs = new TreeMap<>();
    // this is only used while reading model - TODO extract reader
    private final List<DialogModel.Field> fieldsOfCurrentDialog = new ArrayList<>();
    public Map<String, IniField> allIniFields = new LinkedHashMap<>();
    public final Map<String, DialogModel.Field> fieldsInUiOrder = new LinkedHashMap<>();

    public Map</*field name*/String, String> tooltips = new TreeMap<>();
    public Map<String, String> protocolMeta = new TreeMap<>();
    private boolean isConstantsSection;
    private String currentYBins;
    private String currentXBins;
    private final Map<String, String> xBinsByZBins = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<String, String> yBinsByZBins = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private IniFileMetaInfo metaInfo;

    private boolean isInSettingContextHelp = false;
    private boolean isInsidePageDefinition;

    public IniFileModel findAndReadIniFile(String iniFilePath) {
        String fileName = findMetaInfoFile(iniFilePath);
        return readIniFile(fileName);
    }

    public IniFileMetaInfo getMetaInfo() {
        // pageSize lives inside!
        return Objects.requireNonNull(metaInfo, "metaInfo");
    }

    public IniFileModel readIniFile(String fileName) {
        File input = null;
        if (fileName != null)
            input = new File(fileName);
        if (fileName == null || !input.exists()) {
            log.error("No such file: " + fileName);
            return null;
        }

        log.info("Reading " + fileName);
        RawIniFile content = IniFileReader.read(input);
        metaInfo = new IniFileMetaInfo(content);

        readIniFile(content);
        return this;
    }

    public IniFileModel readIniFile(RawIniFile content) {
        for (RawIniFile.Line line : content.getLines()) {
            handleLine(line);
        }
        finishDialog();
        return this;
    }

    private static String findMetaInfoFile(String iniFilePath) {
        return FindFileHelper.findFile(iniFilePath, RUSEFI_INI_PREFIX, RUSEFI_INI_SUFFIX);
    }

    private void finishDialog() {
        if (fieldsOfCurrentDialog.isEmpty())
            return;
        if (dialogUiName == null)
            dialogUiName = dialogId;
        dialogs.put(dialogUiName, new DialogModel(dialogId, dialogUiName, fieldsOfCurrentDialog));

        dialogId = null;
        fieldsOfCurrentDialog.clear();
    }

    private void handleLine(RawIniFile.Line line) {

        String rawText = line.getRawText();
        try {
            LinkedList<String> list = new LinkedList<>(Arrays.asList(line.getTokens()));
            if (!list.isEmpty() && list.get(0).equals("#define")) {
                defines.put(list.get(1), list.subList(2, list.size()));
                return;
            }

            if (!list.isEmpty() && list.get(0).equals(SECTION_PAGE)) {
                isInsidePageDefinition = true;
                return;
            }

            // todo: use TSProjectConsumer constant
            if (isInSettingContextHelp) {
                // todo: use TSProjectConsumer constant
                if (rawText.contains("SettingContextHelpEnd")) {
                    isInSettingContextHelp = false;
                }
                if (list.size() == 2)
                    tooltips.put(list.get(0), list.get(1));
                return;
            } else if (rawText.contains("SettingContextHelp")) {
                isInsidePageDefinition = false;
                isInSettingContextHelp = true;
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

            if (first.startsWith("[") && first.endsWith("]")) {
                log.info("Section " + first);
                isConstantsSection = first.equals("[Constants]");
            }

            if (isConstantsSection) {
                if (isInsidePageDefinition) {
                    if (list.size() > 1)
                        handleFieldDefinition(list, line);
                    return;
                } else {
                    if (list.size() > 1) {
                        protocolMeta.put(list.get(0), list.get(1));
                    }
                }
            }


            switch (first) {
                case "field":
                    handleField(list);
                    break;
                case "slider":
                    handleSlider(list);
                    break;
                case "dialog":
                    handleDialog(list);
                    break;
                case "table":
                    handleTable(list);
                    break;
                case "xBins":
                    handleXBins(list);
                    break;
                case "yBins":
                    handleYBins(list);
                    break;
                case "zBins":
                    handleZBins(list);
                    break;
            }
        } catch (RuntimeException e) {
            throw new IllegalStateException("Failed to handle [" + rawText + "]: " + e, e);
        }
    }

    private void handleZBins(LinkedList<String> list) {
        list.removeFirst();
        String zBins = list.removeFirst();
        addField(zBins);
        if (currentXBins == null || currentYBins == null)
            throw new IllegalStateException("X or Y missing for " + zBins);
        xBinsByZBins.put(zBins, currentXBins);
        yBinsByZBins.put(zBins, currentYBins);
    }

    public String getXBin(String tableName) {
        return xBinsByZBins.get(tableName);
    }

    public Set<String> getTables() {
        return xBinsByZBins.keySet();
    }

    public String getYBin(String tableName) {
        return yBinsByZBins.get(tableName);
    }

    private void handleYBins(LinkedList<String> list) {
        list.removeFirst();
        currentYBins = list.removeFirst();
        addField(currentYBins);
    }

    private void handleXBins(LinkedList<String> list) {
        list.removeFirst();
        currentXBins = list.removeFirst();
        addField(currentXBins);
    }

    private void addField(String key) {
        DialogModel.Field field = new DialogModel.Field(key, key);
        fieldsInUiOrder.put(key, field);
    }

    private void handleTable(LinkedList<String> list) {
        list.removeFirst();
        String tableName = list.removeFirst();
    }

    private void handleFieldDefinition(LinkedList<String> list, RawIniFile.Line line) {
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
                registerField(EnumIniField.parse(list, line, this));
                break;
            default:
                throw new IllegalStateException("Unexpected " + list);
        }
    }

    private void registerField(IniField field) {
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
            fieldsInUiOrder.put(key, field);
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

    private void trim(LinkedList<String> list) {
        while (!list.isEmpty() && list.getFirst().isEmpty())
            list.removeFirst();
    }

    public IniField findByOffset(int i) {
        for (IniField field : allIniFields.values()) {
            if (i >= field.getOffset() && i < field.getOffset() + field.getSize())
                return field;
        }
        return null;
    }

    public static synchronized IniFileModel getInstance() {
        if (INSTANCE == null) {
            INSTANCE = new IniFileModel();
            INSTANCE.findAndReadIniFile(INI_FILE_PATH);
        }
        return INSTANCE;
    }

    public Map<String, DialogModel> getDialogs() {
        return dialogs;
    }
}
