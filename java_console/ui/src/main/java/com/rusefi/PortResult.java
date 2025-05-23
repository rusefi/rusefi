package com.rusefi;

import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.maintenance.CalibrationsInfo;

public class PortResult {
    public final String port;
    public final SerialPortType type;
    public final CalibrationsInfo calibrations;
    public final RusEfiSignature signature;

    public PortResult(final String port, final SerialPortType type, final CalibrationsInfo calibrations) {
        this.port = port;
        this.type = type;
        this.calibrations = calibrations;
        this.signature = (calibrations != null ?
            SignatureHelper.parse(calibrations.getImage().getMeta().getEcuSignature()) :
            null
        );
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
}
