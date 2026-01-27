package com.rusefi;

import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ConfigurationImageMeta;
import com.opensr5.ini.field.IniField;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.maintenance.CalibrationsInfo;

import java.util.Optional;

public class PortResult {
    private static final String HASH3_FIELD_NAME = "hash3";

    public final String port;
    public final SerialPortType type;
    private final CalibrationsInfo calibrations;
    private final RusEfiSignature signature;

    public PortResult(final String port, final SerialPortType type, final CalibrationsInfo calibrations) {
        this.port = port;
        this.type = type;
        this.calibrations = calibrations;
        if (calibrations == null) {
            signature = null;
        } else {
            ConfigurationImageMeta meta = calibrations.getImage().getMeta();
            if (meta == null) {
                signature = null;
            } else {
                signature = SignatureHelper.parse(meta.getEcuSignature());
            }
        }
    }
    public PortResult(String port, SerialPortType type) {
        this(port, type, null);
    }

    protected PortResult(final PortResult origin) {
        this.port = origin.port;
        this.type = origin.type;
        this.calibrations = origin.calibrations;
        this.signature = origin.signature;
    }

    @Override
    public String toString() {
        if (type.friendlyString == null) {
            return this.port;
        } else {
            return this.port + " (" + type.friendlyString + ")";
        }
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }

        if (o == null) {
            return false;
        }

        if (getClass() != o.getClass()) {
            return false;
        }

        PortResult other = (PortResult) o;

        return this.port.equals(other.port) && this.type.equals(other.type);
    }

    public boolean isEcu() {
        return type == SerialPortType.Ecu || type == SerialPortType.EcuWithOpenblt;
    }

    public Optional<String> getFirmwareHash() {
        final Optional<IniField> hash3IniField = calibrations.getIniFile().findIniField(HASH3_FIELD_NAME);
        return hash3IniField.map(field -> ConfigurationImageGetterSetter.getStringValue(field, calibrations.getImage().getConfigurationImage()));
    }

    public CalibrationsInfo getCalibrations() {
        return calibrations;
    }
}
