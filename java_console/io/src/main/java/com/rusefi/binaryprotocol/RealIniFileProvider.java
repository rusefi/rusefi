package com.rusefi.binaryprotocol;

import com.devexperts.logging.Logging;
import com.opensr5.ini.*;
import com.rusefi.ini.reader.IniFileReader;
import com.rusefi.core.SignatureHelper;
import com.rusefi.ui.StatusConsumer;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ini.reader.IniParsingException;
import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class RealIniFileProvider implements IniFileProvider {
    private static final Logging log = getLogging(RealIniFileProvider.class);

    /**
     * Last-resort manual .ini source: when download and local lookups all fail, the UI registers
     * a picker here so the user can point at a local .ini. Kept as an interface so this (io) module
     * stays Swing-free. See {@link com.rusefi.core.SignatureHelper#importIntoCache}.
     */
    public interface ManualIniPicker {
        File pick(String signature);
    }

    public static ManualIniPicker manualPicker = null;

    private StatusConsumer statusConsumer = StatusConsumer.ANONYMOUS;

    public void setStatusConsumer(StatusConsumer statusConsumer) {
        this.statusConsumer = statusConsumer;
    }

    @Override
    @NotNull
    public IniFileModel provide(String signature) throws IniNotFoundException {
        /**
         * first we look at {@link SignatureHelper#LOCAL_INI_CACHE_FOLDER}
         * second we attempt downloading
         * third we look via {@link SignatureHelper#EXTRA_INI_SOURCE} environment variable
         */
        String localIniFile = SignatureHelper.downloadIfNotAvailable(SignatureHelper.getUrl(signature));
        if (localIniFile == null) {
            String message = "Failed to download " + signature + " maybe custom board?";
            log.info(message);
            statusConsumer.logLine(message);
            // 4th option: current folder
            localIniFile = IniLocator.findIniFile(".");
        }
        if (localIniFile == null) {
            // 5th option: one level up or environment variable direction
            localIniFile = IniLocator.findIniFile(IniFileReader.INI_FILE_PATH);
        }
        if (localIniFile == null && manualPicker != null) {
            // 6th option: ask the user to point at a local .ini and cache it for next time
            File picked = manualPicker.pick(signature);
            if (picked != null) {
                try {
                    localIniFile = SignatureHelper.importIntoCache(signature, picked);
                    if (localIniFile == null)
                        localIniFile = picked.getAbsolutePath();
                } catch (IOException e) {
                    log.info("Failed to import picked .ini into cache: " + e);
                    localIniFile = picked.getAbsolutePath();
                }
            }
        }
        if (localIniFile == null)
            throw new IniNotFoundException("Failed to locate .ini file in five different places!");
        IniFileModel iniFileModel;
        try {
                iniFileModel = IniFileReaderUtil.readIniFileChecked(localIniFile);
        } catch (IniParsingException e) {
            throw new IniNotFoundException("Parsing error: " + e, e);
        } catch (FileNotFoundException e) {
            throw new IniNotFoundException(e.toString());
        }
        PrimeTunerStudioCache.prime(iniFileModel, localIniFile);
        return iniFileModel;
    }
}
