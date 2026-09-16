package com.rusefi.maintenance;

import com.rusefi.AvailableHardware;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.maintenance.jobs.DfuManualJob;
import com.rusefi.ui.basic.SingleAsyncJobExecutor;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Component;
import java.awt.Container;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class ProgramSelectorDfuUiTest {
    @Test
    void mainButtonAndDropdownStartTheSameManualJobAndRespectBusyState() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            PortResult dfu = new PortResult("DFU", SerialPortType.Dfu);
            AvailableHardware hardware = new AvailableHardware(Collections.singletonList(dfu), true, false, false);
            ConnectivityContext context = mock(ConnectivityContext.class);
            when(context.getCurrentHardware()).thenReturn(hardware);
            when(context.getConnectedEcuTarget()).thenReturn(ConnectedEcuTarget.forManualRecovery("proteus_f7"));
            ProgramSelector selector = new ProgramSelector(context, new JComboBox<>(new PortResult[]{dfu}),
                ignored -> {}, () -> {});
            SingleAsyncJobExecutor executor = mock(SingleAsyncJobExecutor.class);
            when(executor.isNotInProgress()).thenReturn(true);
            selector.setJobExecutor(executor);

            JPopupMenu menu = selector.apply(hardware, true, true, "universal", "proteus_f7");
            assertEquals(1, menu.getComponentCount(), "obfuscated old files must not hide manual recovery");
            JMenuItem manual = (JMenuItem) menu.getComponent(0);
            assertEquals(UpdateMode.DFU_MANUAL.displayText, manual.getText());
            JButton main = findButton(selector.getControl(), UpdateMode.DFU_MANUAL.displayText);
            assertNotNull(main);
            assertTrue(main.isEnabled());
            main.doClick();
            manual.doClick();
            verify(executor, times(2)).startJob(isA(DfuManualJob.class), any(Component.class));

            when(executor.isNotInProgress()).thenReturn(false);
            selector.apply(hardware, true, false, "universal", "proteus_f7");
            assertFalse(main.isEnabled());
            JButton arrow = findButton(selector.getControl(), "");
            assertNotNull(arrow);
            assertFalse(arrow.isEnabled());
            main.doClick();
            verify(executor, times(2)).startJob(isA(DfuManualJob.class), any(Component.class));
        });
    }

    private static JButton findButton(Container container, String text) {
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && text.equals(((JButton) component).getText())) {
                return (JButton) component;
            }
            if (component instanceof Container) {
                JButton found = findButton((Container) component, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
