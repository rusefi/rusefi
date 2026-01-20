package com.rusefi.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.IniField;
import com.rusefi.tune.xml.Constant;

public class ConfigurationImageGetterSetter2 {
    public static void setValue(IniField iniField, ConfigurationImage image, Constant constant) {
        ConfigurationImageGetterSetter.setValue2(iniField, image, constant.getName(), constant.getValue());
    }
}
