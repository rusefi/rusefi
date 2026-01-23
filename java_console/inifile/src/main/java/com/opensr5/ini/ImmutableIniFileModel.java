package com.opensr5.ini;

import com.opensr5.ini.field.IniField;
import com.rusefi.config.Field;

import java.util.*;

public class ImmutableIniFileModel implements IniFileModel {
    private final String signature;
    private final int blockingFactor;
    private final Map<String, List<String>> defines;
    private final Map<String, IniField> allIniFields;
    private final Map<String, IniField> secondaryIniFields;
    private final Map<String, IniField> allOutputChannels;
    private final Map<String, String> protocolMeta;
    private final IniFileMetaInfo metaInfo;
    private final String iniFilePath;
    private final Map<String, String> tooltips;
    private final Map<String, DialogModel.Field> fieldsInUiOrder;
    private final Map<String, String> xBinsByZBins;
    private final Map<String, String> yBinsByZBins;
    private final Map<String, DialogModel> dialogs;
    private final Map<String, GaugeCategoryModel> gaugeCategories;
    private final Map<String, GaugeModel> gauges;
    private final Map<String, String> topicHelp;
    private final Map<String, ContextHelpModel> contextHelp;

    private static <V> Map<String, V> copyWithCaseInsensitiveKeys(Map<String, V> source) {
        TreeMap<String, V> result = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        result.putAll(source);
        return Collections.unmodifiableMap(result);
    }

    public ImmutableIniFileModel(String signature,
                                 int blockingFactor,
                                 Map<String, List<String>> defines,
                                 Map<String, IniField> allIniFields,
                                 Map<String, IniField> secondaryIniFields,
                                 Map<String, IniField> allOutputChannels,
                                 Map<String, String> protocolMeta,
                                 IniFileMetaInfo metaInfo,
                                 String iniFilePath,
                                 Map<String, String> tooltips,
                                 Map<String, DialogModel.Field> fieldsInUiOrder,
                                 Map<String, String> xBinsByZBins,
                                 Map<String, String> yBinsByZBins,
                                 Map<String, DialogModel> dialogs,
                                 Map<String, GaugeCategoryModel> gaugeCategories,
                                 Map<String, GaugeModel> gauges,
                                 Map<String, String> topicHelp,
                                 Map<String, ContextHelpModel> contextHelp) {
        this.signature = signature;
        this.blockingFactor = blockingFactor;
        this.defines = Collections.unmodifiableMap(new TreeMap<>(defines));
        this.allIniFields = copyWithCaseInsensitiveKeys(allIniFields);
        this.secondaryIniFields = copyWithCaseInsensitiveKeys(secondaryIniFields);
        this.allOutputChannels = copyWithCaseInsensitiveKeys(allOutputChannels);
        this.protocolMeta = Collections.unmodifiableMap(new TreeMap<>(protocolMeta));
        this.metaInfo = metaInfo;
        this.iniFilePath = iniFilePath;
        this.tooltips = Collections.unmodifiableMap(new TreeMap<>(tooltips));
        this.fieldsInUiOrder = Collections.unmodifiableMap(new LinkedHashMap<>(fieldsInUiOrder));
        this.xBinsByZBins = copyWithCaseInsensitiveKeys(xBinsByZBins);
        this.yBinsByZBins = copyWithCaseInsensitiveKeys(yBinsByZBins);
        this.dialogs = Collections.unmodifiableMap(new TreeMap<>(dialogs));
        this.gaugeCategories = Collections.unmodifiableMap(new LinkedHashMap<>(gaugeCategories));
        this.gauges = copyWithCaseInsensitiveKeys(gauges);
        this.topicHelp = Collections.unmodifiableMap(new TreeMap<>(topicHelp));
        this.contextHelp = Collections.unmodifiableMap(new LinkedHashMap<>(contextHelp));
    }

    @Override
    public String getSignature() {
        return signature;
    }

    @Override
    public int getBlockingFactor() {
        if (blockingFactor == 0)
            throw new IllegalStateException("blockingFactor not found in " + iniFilePath);
        return blockingFactor;
    }

    @Override
    public Map<String, List<String>> getDefines() {
        return defines;
    }

    @Override
    public Map<String, IniField> getAllIniFields() {
        return allIniFields;
    }

    @Override
    public Map<String, IniField> getSecondaryIniFields() {
        return secondaryIniFields;
    }

    @Override
    public Optional<IniField> findIniField(String key) {
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
        return metaInfo;
    }

    @Override
    public String getIniFilePath() {
        return iniFilePath;
    }

    @Override
    public Map<String, String> getTooltips() {
        return tooltips;
    }

    @Override
    public Map<String, DialogModel.Field> getFieldsInUiOrder() {
        return fieldsInUiOrder;
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

    @Override
    public Map<String, DialogModel> getDialogs() {
        return dialogs;
    }

    @Override
    public String getDialogKeyByTitle(String dialogTitle) {
        DialogModel dialog = dialogs.get(dialogTitle);
        return dialog != null ? dialog.getKey() : null;
    }

    @Override
    public IniField findByOffset(int i) {
        for (IniField field : allIniFields.values()) {
            if (i >= field.getOffset() && i < field.getOffset() + field.getSize())
                return field;
        }
        return null;
    }

    @Override
    public Map<String, GaugeCategoryModel> getGaugeCategories() {
        return gaugeCategories;
    }

    @Override
    public Map<String, GaugeModel> getGauges() {
        return gauges;
    }

    @Override
    public GaugeModel getGauge(String name) {
        return gauges.get(name);
    }

    @Override
    public Map<String, String> getTopicHelp() {
        return topicHelp;
    }

    @Override
    public Map<String, ContextHelpModel> getContextHelp() {
        return contextHelp;
    }

    @Override
    public ContextHelpModel getContextHelp(String referenceName) {
        return contextHelp.get(referenceName);
    }
}
