package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.ini.field.EnumIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.FieldType;
import com.rusefi.ui.util.UiUtils;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

/**
 * [tag:knock_view_switch_writes] Choosing All / Sensors / Cylinders only changes how the spectrum
 * is drawn. KnockPane.switchCanvasType() used to toggle capture off and on, uploading the
 * "enableKnockSpectrogram" configuration to the ECU twice per switch (both from the same image, so
 * with the same flipped value). A view switch must leave the ECU and the capture state alone.
 */
class KnockPaneViewSwitchTest {
    @Test
    void switchingViewDoesNotWriteTheEcuConfiguration() throws Throwable {
        EnumIniField enabled = new EnumIniField("enableKnockSpectrogram", 0, FieldType.INT,
            new EnumIniField.EnumKeyValueMap(Collections.emptyMap()), 0, 0);
        IniFileModel ini = IniFileModelMocks.empty();
        when(ini.getIniField("enableKnockSpectrogram")).thenReturn(enabled);
        BinaryProtocol bp = mock(BinaryProtocol.class);
        when(bp.getControllerConfiguration()).thenReturn(new ConfigurationImage(new byte[16]));

        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(ini);
        context.getLinkManager().setBinaryProtocolForTests(bp);

        String[] startStop = new String[2];
        SwingUtilities.invokeAndWait(() -> {
            KnockPane pane = new KnockPane(context);
            JButton button = (JButton) find(pane, KnockPane.START_CAPTURE);
            startStop[0] = button.getText();
            find(pane, KnockPane.VIEW_SENSORS).doClick(0);
            find(pane, KnockPane.VIEW_CYLINDERS).doClick(0);
            startStop[1] = button.getText();
        });

        verify(bp, after(500).never()).uploadChanges(any());
        assertEquals(startStop[0], startStop[1], "Switching view leaves capture as it was");
    }

    private static AbstractButton find(KnockPane pane, String text) {
        return (AbstractButton) UiUtils.getAllComponents(pane.getContent()).stream()
            .filter(c -> c instanceof AbstractButton && text.equals(((AbstractButton) c).getText()))
            .findFirst().orElseThrow(() -> new AssertionError("No button " + text));
    }
}
