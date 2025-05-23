package com.rusefi.maintenance.jobs;

import com.devexperts.logging.Logging;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.ui.basic.EcuPortInfo;

import java.util.Collections;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.ui.basic.EcuPortInfo.HASH3_FIELD_NAME;

public class LoadCurrentEcuCalibrationsJob extends AsyncJobWithContext<SerialPortJobContext<EcuPortInfo>> {
    private static final Logging log = getLogging(LoadCurrentEcuCalibrationsJob.class);

    public LoadCurrentEcuCalibrationsJob(final EcuPortInfo port) {
        super(String.format("Enreach Connected Ecu Info (%s)", port), new SerialPortJobContext<>(port));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        final EcuPortInfo portInfo = context.getPort();
        if (portInfo.existsAnyOfIniFields(Collections.singleton(HASH3_FIELD_NAME))) {
            log.info(String.format("We're loading calibrations from ECU on port `%s`...", portInfo));
            final Optional<CalibrationsInfo> currentCalibrations = CalibrationsHelper.readCurrentCalibrations(
                portInfo,
                callbacks
            );
            currentCalibrations.ifPresent(portInfo::setCalibrationsInfo);
        }
    }
}
