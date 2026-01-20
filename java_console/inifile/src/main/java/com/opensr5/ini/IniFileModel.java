package com.opensr5.ini;

import com.opensr5.ini.field.IniField;
import com.rusefi.config.Field;

import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

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

    Map<String, String> getProtocolMeta();

    IniFileMetaInfo getMetaInfo();

    String getIniFilePath();

    Map<String, String> getTooltips();

    Map<String, DialogModel.Field> getFieldsInUiOrder();

    String getXBin(String tableName);

    Set<String> getTables();

    String getYBin(String tableName);

    Map<String, DialogModel> getDialogs();

    IniField findByOffset(int i);
}
