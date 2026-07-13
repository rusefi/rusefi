package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class NumberOfCylindersPanelTest {
    @Test
    void savesCylinderCountAndStrokeUsingOfflineWizardContext() throws Exception {
        UIContext context = WizardSandbox.createOfflineContext();
        WizardConfig config = WizardConfig.snapshot(context);
        assertNotNull(config);
        IniField twoStrokeField = config.ini.findIniField("twoStroke").orElseThrow();

        WizardStepResult fourStroke = NumberOfCylindersPanel.createResult(config.ini, config.image, 4, false);
        WizardStepResult twoStroke = NumberOfCylindersPanel.createResult(config.ini, config.image, 2, true);

        assertNotNull(fourStroke);
        assertEquals("cylindersCount", fourStroke.configFieldName);
        assertEquals("4", fourStroke.value);
        assertEquals("Four Stroke", ConfigurationImageGetterSetter.getStringValue(twoStrokeField, fourStroke.modifiedImage));
        assertEquals("Two Stroke", ConfigurationImageGetterSetter.getStringValue(twoStrokeField, twoStroke.modifiedImage));
        assertEquals("Four Stroke", ConfigurationImageGetterSetter.getStringValue(twoStrokeField, config.image));
    }

    @Test
    void missingStrokeFieldDoesNotProducePartialResult() {
        IniFileModel ini = mock(IniFileModel.class);
        when(ini.findIniField("twoStroke")).thenReturn(Optional.empty());

        assertNull(NumberOfCylindersPanel.createResult(ini, new ConfigurationImage(new byte[1]), 4, false));
    }
}
