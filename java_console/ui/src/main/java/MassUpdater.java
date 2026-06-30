import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.UiVersion;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;
import com.rusefi.maintenance.jobs.DfuManualJob;
import com.rusefi.maintenance.jobs.OpenBltManualJob;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.widgets.ToolButtons;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.stream.Collectors;

import static com.rusefi.SerialPortType.OpenBlt;

public class MassUpdater {
    private final StatusWindow mainStatus = new StatusWindow();
    private final Set<String> knownPorts = new HashSet<>();

    public MassUpdater(ConnectivityContext connectivityContext) {
        mainStatus.showFrame("Mass Updater " + UiVersion.CONSOLE_VERSION);

        final AtomicBoolean previousDfuState = new AtomicBoolean();
        AtomicBoolean isUsingDfu = new AtomicBoolean(); // it seems like DFU detection is not 100% reliable? a work-around to avoid double-DFU

        connectivityContext.getSerialPortScanner().addListener(currentHardware -> {

            if (!isUsingDfu.get() && currentHardware.isDfuFound() != previousDfuState.get()) {
                mainStatus.getContent().logLine(currentHardware.isDfuFound() ? "I see a DFU device!" : "No DFU...");
                if (currentHardware.isDfuFound()) {
                    isUsingDfu.set(true);
                    UpdateOperationCallbacks releaseSemaphore = new UpdateOperationCallbacks() {
                        @Override
                        public void log(String message, boolean breakLineOnTextArea, boolean sendToLogger) {

                        }

                        @Override
                        public void done() {
                            isUsingDfu.set(false);
                        }

                        @Override
                        public void error() {
                            isUsingDfu.set(false);
                        }

                        @Override
                        public void warning() {
                        }

                        @Override
                        public void clear() {}
                    };
                    SwingUtilities.invokeLater(() -> AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(
                        new DfuManualJob(),
                        releaseSemaphore,
                        () -> {}
                    ));
                }
                previousDfuState.set(currentHardware.isDfuFound());
            }

            SerialPortType target = OpenBlt;

            List<PortResult> currentFilteredList = currentHardware.getKnownPorts().stream().filter(portResult -> portResult.type == target).collect(Collectors.toList());
            Set<String> currentSet = currentFilteredList.stream().map(portResult -> portResult.port).collect(Collectors.toSet());
            for (Iterator<String> it = knownPorts.iterator(); it.hasNext(); ) {
                String port = it.next();
                if (!currentSet.contains(port)) {
                    mainStatus.getContent().logLine(port + ": No longer present");
                    it.remove();
                }
            }
            for (PortResult openPort : currentFilteredList) {
                if (!knownPorts.contains(openPort.port)) {
                    knownPorts.add(openPort.port);
                    mainStatus.getContent().logLine("New port " + openPort);

                    SwingUtilities.invokeLater(() -> AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(new OpenBltManualJob(openPort, mainStatus.getContent())));
                }
            }
        });
    }

    public static void main(String[] args) throws InterruptedException, InvocationTargetException {
        ToolButtons.showDeviceManager();
        SwingUtilities.invokeAndWait(() -> new MassUpdater(ConnectivityContext.INSTANCE));
    }
}
