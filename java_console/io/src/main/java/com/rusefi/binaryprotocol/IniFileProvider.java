package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniFileModel;

public interface IniFileProvider {
    IniFileModel provide(String signature);
}
