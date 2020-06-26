package com.rusefi.io.test;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.Field;
import com.rusefi.config.generated.Fields;
import com.rusefi.tune.xml.Constant;
import org.junit.Test;

public class TcpCommunicationIntegrationTest {
    @Test
    public void testConnect() {
        ConfigurationImage ci = new ConfigurationImage(Fields.TOTAL_CONFIG_SIZE);

        Field field = Fields.CYLINDERSCOUNT;

        ScalarIniField scalarIniField = new ScalarIniField(field.getName(), field.getOffset(), "", field.getType(), 1);
        int input = 239;
        scalarIniField.setValue(ci, new Constant(field.getName(), "", Integer.toString(input)));


    }
}
