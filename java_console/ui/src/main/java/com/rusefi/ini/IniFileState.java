package com.rusefi.ini;


import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PrimeTunerStudioCache;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;

import java.io.FileNotFoundException;
import java.util.Objects;

import static com.devexperts.logging.Logging.getLogging;

/**
 * ECU could be connected or not, UI considerations are asking for at least some version of .ini file on start-up
 * <p>
 * another thing to consider is when one type of ECU is un-hooked and another type of ECU is connected
 */
public class IniFileState {
    private static final Logging log = getLogging(RealIniFileProvider.class);
    private final UIContext uiContext;

    private IniFileModel iniFileModel;
    // Signature of the ECU whose INI model is currently cached. Null means local fallback.
    private String lastSignature;

    public IniFileState(UIContext uiContext) {
        this.uiContext = uiContext;
        // todo: do not access file system from AWT thread, start new thread? but that would require smarter sub/notify mechanism for UI
        String localIniFile = PrimeTunerStudioCache.findLocalIniFile();
        if (localIniFile != null) {
            try {
                iniFileModel = IniFileReaderUtil.readIniFile(localIniFile);
            } catch (FileNotFoundException e) {
                log.warn("error " + e);
            }
        }
    }

    // todo: proper design? mock?
    public void setIniFileModelForTest(IniFileModel iniFileModel) {
        this.iniFileModel = iniFileModel;
    }

    public IniFileModel getIniFileModel() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp != null) {
            String currentSignature = bp.signature;
            IniFileModel current = bp.getIniFileNullable();
            if (current != null) {
                // bp has a fully-loaded model — always take it and track the signature.
                if (!Objects.equals(currentSignature, lastSignature)) {
                    log.info("ECU signature changed [" + lastSignature + "] -> [" + currentSignature + "], updating INI model");
                    lastSignature = currentSignature;
                }
                iniFileModel = current;
            } else if (currentSignature != null && !Objects.equals(currentSignature, lastSignature)) {
                // bp is connected, but the INI hasn't loaded yet, and the ECU signature differs
                // from the last cached one — discard the stale model so callers don't use wrong
                // field offsets from the old firmware while waiting for the new INI to load.
                log.info("ECU signature changed [" + lastSignature + "] -> [" + currentSignature + "], invalidating stale INI model");
                lastSignature = currentSignature;
                iniFileModel = null;
            }
            // else: same-ECU reconnect (signature unchanged or not yet known) — keep cached model
            // so the UI stays populated during a transient drop, matching original intent.
        }
        return iniFileModel;
    }
}
