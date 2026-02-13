package com.opensr5.ini;

import com.opensr5.ini.field.IniField;
import com.rusefi.config.Field;
import org.jetbrains.annotations.Nullable;

import java.util.List;
import java.util.Map;
import java.util.Optional;

public interface IniFileModel {
    String getSignature();

    int getBlockingFactor();

    Map<String, List<String>> getDefines();

    // todo: rename to 'getPrimaryPageIniFields()'?
    Map<String, IniField> getAllIniFields();

    Map<String, IniField> getSecondaryIniFields();

    Optional<IniField> findIniField(String key);

    IniField getIniField(Field field);

    @Deprecated // always use 'Field' generated parameter with code-generated name?
    IniField getIniField(String key);

    IniField getOutputChannel(String key) throws IniMemberNotFound;

    /**
     * Get an expression-based output channel by name.
     * These are calculated channels like: coolantTemperature = { useMetricOnInterface ? coolant : (coolant * 1.8 + 32) }
     *
     * @param key the channel name
     * @return the expression string, or null if not found
     */
    String getExpressionOutputChannel(String key);

    /**
     * Get all expression-based output channels.
     * @return map of channel names to their expressions
     */
    Map<String, String> getExpressionOutputChannels();

    Map<String, String> getProtocolMeta();

    IniFileMetaInfo getMetaInfo();

    String getIniFilePath();

    Map<String, String> getTooltips();

    Map<String, DialogModel.Field> getFieldsInUiOrder();


    Map<String, DialogModel> getDialogs();

    String getDialogKeyByTitle(String dialogTitle);

    IniField findByOffset(int i);

    Map<String, GaugeCategoryModel> getGaugeCategories();

    Map<String, GaugeModel> getGauges();

    GaugeModel getGauge(String name);

    /**
     * Find a gauge by its output channel name.
     * @param channelName the output channel name (e.g., "TPSValue")
     * @return the gauge model, or null if not found
     */
    @Nullable
    GaugeModel findGaugeByChannel(String channelName);

    Map<String, String> getTopicHelp();

    Map<String, ContextHelpModel> getContextHelp();

    ContextHelpModel getContextHelp(String referenceName);

    Map<String, TableModel> getTables();

    Map<String, CurveModel> getCurves();

    @Nullable
    TableModel getTable(String name);

    FrontPageModel getFrontPage();

    List<MenuModel> getMenus();
}
