import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.UiVersion;
import com.rusefi.io.BootloaderHelper;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.*;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.widgets.ToolButtons;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.SerialPortType.*;

public class MassUpdater {
    private static final Logging log = getLogging(MassUpdater.class);


    private final StatusWindow mainStatus = new StatusWindow();
    private final Set<String> knownPorts = new HashSet<>();

    public MassUpdater(ConnectivityContext connectivityContext, SerialPortType target) {
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

                    AsyncJobWithContext<SerialPortWithParentComponentJobContext> job;
                    if (EcuWithOpenblt == target) {
                        job = new OpenBltAutoJob(openPort, mainStatus.getContent(), ConnectivityContext.INSTANCE, new UIContext().getLinkManager());
                    } else {
                        job = new OpenBltManualJob(openPort, mainStatus.getContent());
                    }


                    SwingUtilities.invokeLater(() -> AsyncJobExecutor.INSTANCE.executeJobWithStatusWindow(job));
                }
            }
        });
    }

    public static void main(String[] args) throws InterruptedException, InvocationTargetException {
        SerialPortType target = getTargetFromArguments(args);
        log.error("Target port type: " + target);

        ToolButtons.showDeviceManager();
        SwingUtilities.invokeAndWait(() -> new MassUpdater(ConnectivityContext.INSTANCE, target));
    }

    private static @NotNull SerialPortType getTargetFromArguments(String[] args) {
        SerialPortType target = OpenBlt;
        if (args.length > 0) {
            String arg = args[0];
            for (SerialPortType type : SerialPortType.values()) {
                if (type.name().equalsIgnoreCase(arg)) {
                    target = type;
                    break;
                }
            }
        }
        return target;
    }
}
