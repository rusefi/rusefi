package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.UIContext;

/**
 * Live snapshot of the ECU's INI model and controller configuration image.
 * Non-null factory return guarantees both fields are available; on {@code null}
 * return, callers should bail out.
 */
public class WizardConfig {
    public final BinaryProtocol bp;
    public final IniFileModel ini;
    public final ConfigurationImage image;

    private WizardConfig(BinaryProtocol bp, IniFileModel ini, ConfigurationImage image) {
        this.bp = bp;
        this.ini = ini;
        this.image = image;
    }

    /** Returns a snapshot, or {@code null} if the ECU/ini/image aren't all available. */
    public static WizardConfig snapshot(UIContext uiContext) {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp == null) return null;
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini == null) return null;
        ConfigurationImage image = bp.getControllerConfiguration();
        if (image == null) return null;
        return new WizardConfig(bp, ini, image);
    }
}
