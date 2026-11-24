package com.rusefi.ui;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.*;

class SlcanTuneEditTest {
    @Test
    void snifferTogglePreservesPendingTuneEditsAndUsesTheirUploadQueue() throws Throwable {
        IniFileModel ini = ini();
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.setOfflineMode(true);
            context.setIniFileModelForOffline(ini);
            BinaryProtocol protocol = mock(BinaryProtocol.class);
            when(protocol.getControllerConfiguration()).thenReturn(ConfigurationImageTestHelper.newLittleEndian(8));
            context.getLinkManager().setBinaryProtocolForTests(protocol);
            TuningPane tuning = new TuningPane(context);
            SlcanTab sniffer = null;
            try {
                tuning.seedOfflineImage(ConfigurationImageTestHelper.newLittleEndian(8), "canHw1");
                context.setTuneEditor(tuning::applyEdit);
                assertTrue(tuning.applyEdit(image -> image.getContent()[2] = 42));
                sniffer = new SlcanTab(context, message -> fail(message), false);
                JCheckBox checkbox = checkbox(sniffer.getContent());
                assertNotNull(checkbox);
                checkbox.doClick(0);

                byte[] working = tuning.getDialogForTests().getWorkingImage().getContent();
                assertEquals(1, working[0]);
                assertEquals(1, working[1]);
                assertEquals(42, working[2], "A sniffer toggle must not replace pending tuning edits");
                assertArrayEquals(working, context.getCurrentConfigurationImage().getContent());
                verify(protocol, never()).uploadChangesWithoutBurn(any());

                // The production firmware-update guard can reject an auxiliary edit.
                context.setTuneEditor(edit -> false);
                checkbox.doClick(0);
                assertTrue(checkbox.isSelected(), "Rejected changes keep the displayed state");
                assertEquals(1, tuning.getDialogForTests().getWorkingImage().getContent()[0]);
            } finally {
                if (sniffer != null) {
                    sniffer.close();
                }
                tuning.destroy();
                context.getLinkManager().setNotifyGlobalStatusOnClose(false).close();
            }
        });
    }

    private static JCheckBox checkbox(Container parent) {
        for (Component component : parent.getComponents()) {
            if (component instanceof JCheckBox) {
                return (JCheckBox) component;
            }
            if (component instanceof Container) {
                JCheckBox found = checkbox((Container) component);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static IniFileModel ini() throws Throwable {
        String text = "[Constants]\n"
            + "signature = \"slcan-edit-test\"\nnPages = 1\npageSize = 8\n"
            + "pageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "canSniffer1_listenOurs = scalar, U08, 0, \"\", 1, 0, 0, 1, 0\n"
            + "canSniffer1_read = scalar, U08, 1, \"\", 1, 0, 0, 1, 0\n"
            + "fuel = scalar, U08, 2, \"\", 1, 0, 0, 100, 0\n"
            + "[Menu]\nmenu = \"Setup\"\nsubMenu = canHw1, \"CAN 1\"\n"
            + "[UserDefined]\ndialog = canHw1, \"CAN 1\"\nfield = \"Fuel\", fuel\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(raw, "slcan-edit-test.ini", new IniFileMetaInfoImpl(raw));
    }
}
