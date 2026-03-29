package com.rusefi.ui.mocked;

import com.opensr5.ini.*;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.Field;

import java.util.*;

public class StubIniFileModel implements IniFileModel {
    private final Map<String, GaugeModel> gauges = new HashMap<>();
    public void register(GaugeModel g) { gauges.put(g.getName(), g); }
    @Override public Map<String, GaugeModel> getGauges() { return gauges; }
    @Override public GaugeModel getGauge(String name) { return gauges.get(name); }

    // --- Interface scaffolding ---
    @Override public String getSignature() { return "PRO_STUB"; }
    @Override public int getBlockingFactor() { return 128; }
    @Override public Map<String, List<String>> getDefines() { return Collections.emptyMap(); }
    @Override public Map<String, IniField> getAllIniFields() { return Collections.emptyMap(); }
    @Override public Map<String, IniField> getSecondaryIniFields() { return Collections.emptyMap(); }
    @Override public Optional<IniField> findIniField(String k) { return Optional.empty(); }
    @Override public IniField getIniField(Field f) { return null; }
    @Override public IniField getIniField(String k) { return null; }
    @Override public IniField getOutputChannel(String k) { return null; }

    @Override
    public Map<String, IniField> getAllOutputChannels() {
        return Collections.emptyMap();
    }

    @Override public String getExpressionOutputChannel(String k) { return null; }
    @Override public Map<String, String> getExpressionOutputChannels() { return Collections.emptyMap(); }
    @Override public Map<String, String> getProtocolMeta() { return Collections.emptyMap(); }
    @Override public IniFileMetaInfo getMetaInfo() { return null; }
    @Override public String getIniFilePath() { return ""; }
    @Override public Map<String, String> getTooltips() { return Collections.emptyMap(); }
    @Override public Map<String, DialogModel.Field> getFieldsInUiOrder() { return Collections.emptyMap(); }
    @Override public Map<String, DialogModel> getDialogs() { return Collections.emptyMap(); }
    @Override public String getDialogKeyByTitle(String t) { return null; }
    @Override public IniField findByOffset(int i) { return null; }
    @Override public Map<String, GaugeCategoryModel> getGaugeCategories() { return Collections.emptyMap(); }
    @Override public GaugeModel findGaugeByChannel(String c) { return null; }
    @Override public Map<String, String> getTopicHelp() { return Collections.emptyMap(); }
    @Override public Map<String, ContextHelpModel> getContextHelp() { return Collections.emptyMap(); }
    @Override public ContextHelpModel getContextHelp(String r) { return null; }
    @Override public Map<String, TableModel> getTables() { return Collections.emptyMap(); }
    @Override public Map<String, CurveModel> getCurves() { return Collections.emptyMap(); }
    @Override public TableModel getTable(String n) { return null; }
    @Override public FrontPageModel getFrontPage() { return null; }
    @Override public List<MenuModel> getMenus() { return Collections.emptyList(); }
}