package com.rusefi.ui.mocked;

import com.opensr5.ini.*;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.Field;
import java.util.*;

/**
 * Proxy class that forwards all calls to a real IniFileModel.
 * Use this to override specific behaviors (like filtering menus)
 * without breaking the underlying INI logic.
 */
public class DelegatingIniFileModel implements IniFileModel {
    protected final IniFileModel delegate;

    public DelegatingIniFileModel(IniFileModel delegate) {
        this.delegate = Objects.requireNonNull(delegate, "Delegate cannot be null");
    }

    @Override public Map<String, GaugeModel> getGauges() { return delegate.getGauges(); }
    @Override public GaugeModel getGauge(String name) { return delegate.getGauge(name); }
    @Override public String getSignature() { return delegate.getSignature(); }
    @Override public int getBlockingFactor() { return delegate.getBlockingFactor(); }
    @Override public Map<String, List<String>> getDefines() { return delegate.getDefines(); }
    @Override public Map<String, IniField> getAllIniFields() { return delegate.getAllIniFields(); }
    @Override public Map<String, IniField> getSecondaryIniFields() { return delegate.getSecondaryIniFields(); }
    @Override public Optional<IniField> findIniField(String k) { return delegate.findIniField(k); }
    @Override public IniField getIniField(Field f) { return delegate.getIniField(f); }
    @Override public IniField getIniField(String k) { return delegate.getIniField(k); }
    @Override public IniField getOutputChannel(String k) throws IniMemberNotFound { return delegate.getOutputChannel(k); }

    @Override
    public Map<String, IniField> getAllOutputChannels() {
        return delegate.getAllOutputChannels();
    }

    @Override public String getExpressionOutputChannel(String k) { return delegate.getExpressionOutputChannel(k); }
    @Override public Map<String, String> getExpressionOutputChannels() { return delegate.getExpressionOutputChannels(); }
    @Override public Map<String, String> getProtocolMeta() { return delegate.getProtocolMeta(); }
    @Override public IniFileMetaInfo getMetaInfo() { return delegate.getMetaInfo(); }
    @Override public String getIniFilePath() { return delegate.getIniFilePath(); }
    @Override public Map<String, String> getTooltips() { return delegate.getTooltips(); }
    @Override public Map<String, DialogModel.Field> getFieldsInUiOrder() { return delegate.getFieldsInUiOrder(); }
    @Override public Map<String, DialogModel> getDialogs() { return delegate.getDialogs(); }
    @Override public String getDialogKeyByTitle(String t) { return delegate.getDialogKeyByTitle(t); }
    @Override public IniField findByOffset(int i) { return delegate.findByOffset(i); }
    @Override public Map<String, GaugeCategoryModel> getGaugeCategories() { return delegate.getGaugeCategories(); }
    @Override public GaugeModel findGaugeByChannel(String c) { return delegate.findGaugeByChannel(c); }
    @Override public Map<String, String> getTopicHelp() { return delegate.getTopicHelp(); }
    @Override public Map<String, ContextHelpModel> getContextHelp() { return delegate.getContextHelp(); }
    @Override public ContextHelpModel getContextHelp(String r) { return delegate.getContextHelp(r); }
    @Override public Map<String, TableModel> getTables() { return delegate.getTables(); }
    @Override public Map<String, CurveModel> getCurves() { return delegate.getCurves(); }
    @Override public TableModel getTable(String n) { return delegate.getTable(n); }
    @Override public FrontPageModel getFrontPage() { return delegate.getFrontPage(); }
    @Override public List<MenuModel> getMenus() { return delegate.getMenus(); }
}