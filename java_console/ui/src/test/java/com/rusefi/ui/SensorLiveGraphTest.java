package com.rusefi.ui;

import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class SensorLiveGraphTest {
    @Test
    void gaugeMenuUsesLoadedIniWithoutAnEcuConnection() throws Throwable {
        IniFileModel ini;
        try (InputStream input = getClass().getResourceAsStream("/january.ini")) {
            assertNotNull(input);
            RawIniFile raw = IniFileReaderUtil.read(input, "/january.ini");
            ini = IniFileReaderUtil.readIniFile(raw, "/january.ini", new IniFileMetaInfoImpl(raw));
        }
        List<String> expectedPaths = new ArrayList<>();
        ini.getGaugeCategories().forEach((category, model) -> model.getGauges().forEach(
            gauge -> expectedPaths.add(category + " / " + gauge.getTitle())));
        assertTrue(expectedPaths.size() > 100, "Exercise the sandbox's full INI catalog");

        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.iniFileState.setIniFileModelForTest(ini);
            assertNull(context.getBinaryProtocol());
            SensorLiveGraph graph = new SensorLiveGraph(context, new Node(), "RPMGauge", null);
            try {
                graph.setActive(false);
                JPopupMenu menu = new JPopupMenu();
                graph.addChangeSensorItems(menu);
                // Reproduction: the loaded INI is ignored without a live protocol.
                assertEquals(7, gaugePaths(menu).size());
                assertNotEquals(expectedPaths, gaugePaths(menu));

                BinaryProtocol protocol = mock(BinaryProtocol.class);
                when(protocol.getIniFileNullable()).thenReturn(ini);
                context.getLinkManager().setBinaryProtocolForTests(protocol);
                JPopupMenu connectedMenu = new JPopupMenu();
                graph.addChangeSensorItems(connectedMenu);
                assertEquals(expectedPaths, gaugePaths(connectedMenu));
            } finally {
                graph.destroy();
            }
        });
    }

    private static List<String> gaugePaths(JPopupMenu menu) {
        List<String> paths = new ArrayList<>();
        for (Component component : menu.getComponents()) {
            JMenu category = (JMenu) component;
            for (Component child : category.getMenuComponents()) {
                paths.add(category.getText() + " / " + ((JMenuItem) child).getText());
            }
        }
        return paths;
    }
}
