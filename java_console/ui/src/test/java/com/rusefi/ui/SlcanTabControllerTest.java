package com.rusefi.ui;

import com.opensr5.ini.GroupMenuModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.MenuModel;
import com.opensr5.ini.SubMenuModel;
import org.junit.jupiter.api.Test;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;

import javax.swing.*;
import java.awt.Graphics;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.awt.image.BufferedImage;
import java.util.Collections;
import java.util.function.Supplier;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class SlcanTabControllerTest {
    @Test
    void parsedIniRequiresMenuEntryEvenWhenDialogExists() throws IniParsingException {
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getSignature()).thenReturn("test");
        String menu = "[Menu]\nmenu = \"Connectivity\"\n";
        String entry = "subMenu = canBusSniffer, \"CAN Bus sniffer\"\n";
        String dialog = "[UserDefined]\ndialog = canBusSniffer, \"CAN Bus sniffer\"\n";
        for (boolean supported : new boolean[]{false, true}) {
            String text = menu + (supported ? entry : "") + dialog;
            RawIniFile raw = IniFileReaderUtil.read(
                new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
            IniFileModel parsed = IniFileReaderUtil.readIniFile(raw, "test", meta);
            assertEquals(supported, SlcanTabController.supportsSniffer(parsed));
        }
    }

    private static IniFileModel ini(boolean supported) {
        IniFileModel ini = mock(IniFileModel.class);
        MenuModel menu = new MenuModel("Connectivity");
        GroupMenuModel group = new GroupMenuModel("CAN");
        group.getItems().add(new SubMenuModel(supported ? "canBusSniffer" : "canBus", "CAN"));
        menu.getItems().add(group);
        when(ini.getMenus()).thenReturn(Collections.singletonList(menu));
        return ini;
    }

    @Test
    void visibilityFollowsBoardAndDisconnectWithoutStartingScanner() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTabbedPane tabs = new JTabbedPane();
            JPanel device = new JPanel();
            tabs.addTab("Device", device);
            @SuppressWarnings("unchecked")
            Supplier<SlcanTab> factory = mock(Supplier.class);
            SlcanTabController controller = new SlcanTabController(tabs, factory);
            controller.update(null, false);
            assertEquals(1, tabs.getTabCount());
            controller.update(ini(false), false);
            assertEquals(1, tabs.getTabCount());
            IniFileModel supported = ini(true);
            controller.update(supported, false);
            assertEquals(2, tabs.getTabCount());
            assertSame(device, tabs.getSelectedComponent());
            controller.update(supported, false);
            assertEquals(2, tabs.getTabCount());
            controller.update(ini(false), false);
            assertEquals(1, tabs.getTabCount());
            controller.update(supported, false);
            controller.update(null, false);
            assertEquals(1, tabs.getTabCount());
            verifyNoInteractions(factory);
        });
    }

    @Test
    void openedTabIsClosedAndRecreatedForAnotherSupportedBoard() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTabbedPane tabs = new JTabbedPane();
            SlcanTab sniffer = mock(SlcanTab.class);
            when(sniffer.getContent()).thenReturn(new JPanel());
            SlcanTabController controller = new SlcanTabController(tabs, () -> sniffer);
            IniFileModel first = ini(true);
            controller.update(first, false);
            java.awt.Component original = tabs.getComponentAt(0);
            Graphics graphics = new BufferedImage(100, 100, BufferedImage.TYPE_INT_RGB).getGraphics();
            try {
                original.paint(graphics);
            } finally {
                graphics.dispose();
            }
            verify(sniffer).getContent();
            controller.update(first, false);
            verify(sniffer, never()).close();
            controller.update(ini(true), false);
            verify(sniffer).close();
            assertEquals(1, tabs.getTabCount());
            assertNotSame(original, tabs.getComponentAt(0));
        });
    }

    @Test
    void overrideAllowsStandaloneAndOlderFirmware() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTabbedPane tabs = new JTabbedPane();
            SlcanTabController controller = new SlcanTabController(tabs, () -> {
                throw new AssertionError("Tab must remain lazy");
            });
            controller.update(null, true);
            assertEquals(1, tabs.getTabCount());
            controller.update(ini(false), true);
            assertEquals(1, tabs.getTabCount());
            controller.update(null, false);
            assertEquals(0, tabs.getTabCount());
        });
    }
}
