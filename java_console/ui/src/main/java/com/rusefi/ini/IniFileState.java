package com.rusefi.ini;


import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PrimeTunerStudioCache;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;

import java.io.FileNotFoundException;

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

    public IniFileModel getIniFileModel() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp != null) {
            iniFileModel = bp.getIniFile();
        }
        return iniFileModel;
    }
}
