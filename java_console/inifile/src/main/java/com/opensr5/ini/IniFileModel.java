package com.opensr5.ini;

import com.opensr5.ini.field.IniField;

import java.util.List;
import java.util.Map;
import java.util.Set;

public interface IniFileModel {
    String getSignature();

    int getBlockingFactor();

    Map<String, List<String>> getDefines();

    Map<String, IniField> getAllIniFields();

    IniField getIniField(String key);

    Map<String, String> getProtocolMeta();

    IniFileMetaInfo getMetaInfo();

    String getIniFilePath();

    Map<String, String> getTooltips();

    Map<String, DialogModel.Field> getFieldsInUiOrder();

    String getXBin(String tableName);

    Set<String> getTables();

    String getYBin(String tableName);

    Map<String, DialogModel> getDialogs();
}
