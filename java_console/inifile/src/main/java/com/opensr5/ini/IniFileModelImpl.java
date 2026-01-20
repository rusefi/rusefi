package com.opensr5.ini;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.*;
import com.rusefi.config.Field;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.*;
import java.util.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 12/23/2015.
 */
public class IniFileModelImpl implements IniFileModel {
    private static final Logging log = Logging.getLogging(IniFileModelImpl.class);
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
    private final Map<String, DialogModel> dialogs = new TreeMap<>();
    // this is only used while reading model - TODO extract reader
    private final List<DialogModel.Field> fieldsOfCurrentDialog = new ArrayList<>();
    private final List<DialogModel.Command> commandsOfCurrentDialog = new ArrayList<>();
    private final Map<String, IniField> allIniFields = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<String, IniField> secondaryIniFields = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<String, IniField> allOutputChannels = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    public final Map<String, DialogModel.Field> fieldsInUiOrder = new LinkedHashMap<>();

    private final Map</*field name*/String, String> tooltips = new TreeMap<>();
    private final Map<String, String> protocolMeta = new TreeMap<>();
    private boolean isConstantsSection;
    private boolean isOutputChannelsSection;
    private String currentYBins;
    private String currentXBins;
    private final Map<String, String> xBinsByZBins = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<String, String> yBinsByZBins = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final IniFileMetaInfo metaInfo;
    private final String iniFilePath;

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

    private IniFileModelImpl(@Nullable final IniFileMetaInfoImpl metaInfo, final String iniFilePath) {
        this.metaInfo = metaInfo;
        this.iniFilePath = iniFilePath;
    }

    @Override
    public String getSignature() {
        return metaInfo.getSignature();
    }

    @Override
    public int getBlockingFactor() {
        if (blockingFactorOverride != null)
            return blockingFactorOverride;
        if (blockingFactor == 0)
            throw new IllegalStateException("blockingFactor not found in " + iniFilePath);
        return blockingFactor;
    }

    @Override
    public Map<String, List<String>> getDefines() {
        return defines;
    }

    @Override
    // todo: rename to 'getPrimaryPageIniFields()'?
    public Map<String, IniField> getAllIniFields() {
        return Collections.unmodifiableMap(allIniFields);
    }

    @Override
    public Map<String, IniField> getSecondaryIniFields() {
        return Collections.unmodifiableMap(secondaryIniFields);
    }

    @Override
    public Optional<IniField> findIniField(final String key) {
        return Optional.ofNullable(allIniFields.get(key));
    }

    @Override
    public IniField getIniField(Field field) {
        return getIniField(field.getName());
    }

    @Override
    public IniField getIniField(String key) {
        IniField result = allIniFields.get(key);
        return Objects.requireNonNull(result, () -> key + " field not found");
    }

    @Override
    public IniField getOutputChannel(String key) throws IniMemberNotFound {
        IniField result = allOutputChannels.get(key);
        if (result == null)
            throw new IniMemberNotFound(key + " field not found");
        return result;
    }

    @Override
    public Map<String, String> getProtocolMeta() {
        return protocolMeta;
    }

    @Override
    public IniFileMetaInfo getMetaInfo() {
        // pageSize lives inside!
        return Objects.requireNonNull(metaInfo, "metaInfo");
    }

    @Override
    public String getIniFilePath() {
        return Objects.requireNonNull(iniFilePath, "iniFilePath");
    }

    @Override
    public Map<String, String> getTooltips() {
        return tooltips;
    }

    @Override
    public Map<String, DialogModel.Field> getFieldsInUiOrder() {
        return fieldsInUiOrder;
    }

    @NotNull
    public static IniFileModelImpl readIniFile(String fileName) throws FileNotFoundException {
        Objects.requireNonNull(fileName, "fileName");
        log.info("Reading " + fileName);
        File input = new File(fileName);
        RawIniFile content = IniFileReader.read(input);
        return readIniFile(content, true, fileName);
    }

    /**
     * @param initMeta - part of our tests do not use `getMetaInfo` getter and will throw MandatoryLineMissing exception
     *                 on attempt to create IniFileMetaInfoImpl instance from test data; to avoid this exception such
     *                 tests should use `false` as value for this parameter
     */
    @NotNull
    public static IniFileModelImpl readIniFile(
        final RawIniFile content,
        final boolean initMeta,
        final String iniFilePath
    ) {
        final IniFileModelImpl result = new IniFileModelImpl(
            initMeta ? new IniFileMetaInfoImpl(content) : null,
            iniFilePath
        );
        for (RawIniFile.Line line : content.getLines()) {
            result.handleLine(line);
        }
        result.finishDialog();
        return result;
    }

    private void finishDialog() {
        if (fieldsOfCurrentDialog.isEmpty() && commandsOfCurrentDialog.isEmpty())
            return;
        if (dialogUiName == null)
            dialogUiName = dialogId;
        dialogs.put(dialogUiName, new DialogModel(dialogId, dialogUiName, fieldsOfCurrentDialog, commandsOfCurrentDialog));

        dialogId = null;
        fieldsOfCurrentDialog.clear();
        commandsOfCurrentDialog.clear();
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
                    isInSettingContextHelp = false;
                }
                if (list.size() == 2)
                    tooltips.put(list.get(0), list.get(1));
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
                if (first.contains("[SettingContextHelp]")) {
                    isInsidePageDefinition = false;
                    isInSettingContextHelp = true;
                    return;
                }
                isConstantsSection = first.equals("[Constants]");
                isOutputChannelsSection = first.equals("[OutputChannels]");
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

    private void handleZBins(LinkedList<String> list) {
        list.removeFirst();
        String zBins = list.removeFirst();
        addField(zBins);
        if (currentXBins == null || currentYBins == null)
            throw new IllegalStateException("X or Y missing for " + zBins);
        xBinsByZBins.put(zBins, currentXBins);
        yBinsByZBins.put(zBins, currentYBins);
    }

    @Override
    public String getXBin(String tableName) {
        return xBinsByZBins.get(tableName);
    }

    @Override
    public Set<String> getTables() {
        return xBinsByZBins.keySet();
    }

    @Override
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
                registerField(EnumIniField.parse(list, line, this));
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

    @Override
    public Map<String, DialogModel> getDialogs() {
        return dialogs;
    }
}
