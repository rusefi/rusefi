import com.rusefi.SerialPortScanner;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;
import com.rusefi.maintenance.jobs.OpenBltManualJob;
import com.rusefi.ui.StatusWindow;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

import static com.rusefi.SerialPortScanner.SerialPortType.OpenBlt;

public class MassUpdater {
    private final StatusWindow mainStatus = new StatusWindow();
    private final Set<String> knownBlts = new HashSet<>();

    public MassUpdater() {
        mainStatus.showFrame("Mass Updater");

        SerialPortScanner.INSTANCE.addListener(currentHardware -> {
            List<SerialPortScanner.PortResult> currentBltList = currentHardware.getKnownPorts().stream().filter(portResult -> portResult.type == OpenBlt).collect(Collectors.toList());
            Set<String> currentSet = currentBltList.stream().map(portResult -> portResult.port).collect(Collectors.toSet());
            for (Iterator<String> it = knownBlts.iterator(); it.hasNext(); ) {
                String port = it.next();
                if (!currentSet.contains(port)) {
                    mainStatus.append(port + ": No longer present");
                    it.remove();
                }
            }
            for (SerialPortScanner.PortResult openBltPort : currentBltList) {
                if (!knownBlts.contains(openBltPort.port)) {
                    knownBlts.add(openBltPort.port);
                    mainStatus.append("New OpenBlt " + openBltPort);

                    AsyncJobExecutor.INSTANCE.executeJob(new OpenBltManualJob(openBltPort, mainStatus.getContent()));
                }
            }
        });
    }

    public static void main(String[] args) throws InterruptedException, InvocationTargetException {
        SwingUtilities.invokeAndWait(MassUpdater::new);
    }
}
