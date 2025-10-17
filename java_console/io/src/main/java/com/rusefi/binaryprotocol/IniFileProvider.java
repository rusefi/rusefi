package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniFileModel;
import org.jetbrains.annotations.NotNull;

public interface IniFileProvider {
    @NotNull
    IniFileModel provide(String signature) throws IniNotFoundException;
}
