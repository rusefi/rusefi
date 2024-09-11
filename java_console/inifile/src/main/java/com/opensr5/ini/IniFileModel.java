package com.opensr5.ini;

import com.opensr5.ini.field.IniField;

import java.util.List;
import java.util.Map;
import java.util.Set;

public interface IniFileModel {
    IniFileModelImpl findAndReadIniFile(String iniFilePath);

    Map<String, List<String>> getDefines();

    Map<String, IniField> getAllIniFields();

    Map<String, String> getProtocolMeta();

    IniFileMetaInfo getMetaInfo();

    Map<String, String> getTooltips();

    Map<String, DialogModel.Field> getFieldsInUiOrder();

    String getXBin(String tableName);

    Set<String> getTables();

    String getYBin(String tableName);

    Map<String, DialogModel> getDialogs();
}
