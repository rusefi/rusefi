package com.rusefi.binaryprotocol;

import com.opensr5.ini.IniFileModel;
import com.rusefi.ui.StatusConsumer;
import org.jetbrains.annotations.NotNull;

import java.util.function.BooleanSupplier;

public interface IniFileProvider {
    @NotNull
    IniFileModel provide(String signature) throws IniNotFoundException;

    default IniFileModel provide(String signature, String port, long connectionGeneration)
            throws IniNotFoundException {
        return provide(signature);
    }

    default IniFileModel provide(String signature, String port, long connectionGeneration,
                                 BooleanSupplier connectionCurrent) throws IniNotFoundException {
        return provide(signature, port, connectionGeneration);
    }

    default void setStatusConsumer(StatusConsumer statusConsumer) {
    }
}
