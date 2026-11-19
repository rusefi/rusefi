package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.core.SignatureHelper;
import com.rusefi.maintenance.FirmwareSelection;
import com.rusefi.maintenance.FirmwareOperations;
import com.rusefi.maintenance.OpenBltWipeArtifact;

import javax.swing.*;
import java.util.concurrent.atomic.AtomicReference;

public class OpenBltManualJobFactory {
    interface NoIniFlashOperation {
        boolean flash(JComponent parent, PortResult port, String signature, UpdateOperationCallbacks callbacks,
                      ConnectivityContext connectivityContext, ConnectedEcuTarget target,
                      FirmwareSelection selection);
    }

    interface NoIniFirmwarePreparation {
        FirmwareSelection prepare(UpdateOperationCallbacks callbacks, ConnectedEcuTarget target, String signature);
    }

    public static OpenBltManualJob createProduction(PortResult port, final JComponent parent, final ConnectivityContext connectivityContext) {
        return new OpenBltManualJob(port, parent, connectivityContext, ProductionFlashSteps.PRODUCTION_STEPS);
    }

    public static OpenBltManualJob createEmergencyWipe(PortResult port, final JComponent parent,
                                                        final ConnectivityContext connectivityContext,
                                                        final OpenBltWipeArtifact artifact) {
        return new OpenBltManualJob("Emergency OpenBLT wipe", port, parent, connectivityContext,
            ProductionFlashSteps.emergencyWipe(artifact));
    }

    public static OpenBltManualJob createNoIniUpdate(PortResult port, final JComponent parent,
                                                    final ConnectivityContext connectivityContext,
                                                    final LinkManager linkManager, final String signature) {
        return new OpenBltManualJob("Update firmware (no ini)", port, parent, connectivityContext,
            createNoIniUpdateSteps(port, connectivityContext, linkManager, signature));
    }

    static OpenBltManualJob.FlashSteps createNoIniUpdateSteps(
        PortResult port, ConnectivityContext connectivityContext, LinkManager linkManager, String signature) {
        return createNoIniUpdateSteps(port, connectivityContext, linkManager, signature,
            (callbacks, target, expectedSignature) ->
                FirmwareOperations.prepareNoIniFirmware(expectedSignature, callbacks, target),
            FirmwareOperations::flashOpenbltWithoutSettings);
    }

    static OpenBltManualJob.FlashSteps createNoIniUpdateSteps(
        PortResult port, ConnectivityContext connectivityContext, LinkManager linkManager, String signature,
        NoIniFlashOperation noIniFlashOperation) {
        return createNoIniUpdateSteps(port, connectivityContext, linkManager, signature,
            (callbacks, target, expectedSignature) ->
                FirmwareOperations.prepareNoIniFirmware(expectedSignature, callbacks, target),
            noIniFlashOperation);
    }

    static OpenBltManualJob.FlashSteps createNoIniUpdateSteps(
        PortResult port, ConnectivityContext connectivityContext, LinkManager linkManager, String signature,
        NoIniFirmwarePreparation noIniFirmwarePreparation, NoIniFlashOperation noIniFlashOperation) {
        com.rusefi.core.RusEfiSignature parsedSignature = SignatureHelper.parse(signature);
        if (parsedSignature == null || parsedSignature.getBundleTarget() == null) {
            throw new IllegalArgumentException("A valid ECU signature is required for no-INI recovery");
        }
        final ConnectedEcuTarget jobTarget =
            ConnectedEcuTarget.forManualRecovery(parsedSignature.getBundleTarget());
        final AtomicReference<FirmwareSelection> selection = new AtomicReference<>();
        return new OpenBltManualJob.FlashSteps() {
                @Override
                public boolean ensureFirmware(UpdateOperationCallbacks callbacks, ConnectedEcuTarget target) {
                    FirmwareSelection prepared =
                        noIniFirmwarePreparation.prepare(callbacks, jobTarget, signature);
                    selection.set(prepared);
                    return prepared != null;
                }

                @Override
                public boolean flash(JComponent flashParent, String flashPort, UpdateOperationCallbacks callbacks,
                                     ConnectedEcuTarget target) {
                    FirmwareSelection selected = selection.get();
                    if (selected == null) {
                        return false;
                    }
                    boolean flashed = noIniFlashOperation.flash(
                        flashParent, new PortResult(flashPort, port.type), signature, callbacks, connectivityContext,
                        jobTarget, selected);
                    if (flashed) {
                        if (linkManager != null) {
                            LinkManager.MissingIniFailure failure = linkManager.getMissingIniFailure();
                            if (failure != null && failure.port.equals(flashPort) && failure.signature.equals(signature)) {
                                linkManager.clearMissingIniFailure();
                            }
                        }
                        com.rusefi.MissingIniRecoveryState.Failure probedFailure =
                            connectivityContext.getMissingIniRecoveryState().get();
                        if (probedFailure != null && probedFailure.port.equals(flashPort)
                            && probedFailure.signature.equals(signature)) {
                            connectivityContext.getMissingIniRecoveryState().clearForPort(flashPort);
                        }
                    }
                    return flashed;
                }

                @Override
                public void restoreCalibrations(UpdateOperationCallbacks callbacks,
                                                ConnectivityContext ignoredContext) {
                    // No matching INI means there is no safe calibration backup or restoration path.
                }

                @Override
                public OpenBltManualJob.RestoreResult restoreCalibrationsWithResult(
                    UpdateOperationCallbacks callbacks, ConnectivityContext ignoredContext) {
                    callbacks.logLine("Settings backup and restoration were intentionally skipped because no matching INI was available.");
                    return OpenBltManualJob.RestoreResult.SKIPPED;
                }
            };
    }
}
