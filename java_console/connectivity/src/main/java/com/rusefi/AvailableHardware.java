package com.rusefi;

import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

public class AvailableHardware {

    private final List<PortResult> ports;
    private final boolean dfuFound;
    private final boolean stLinkConnected;
    private final boolean PCANConnected;

    public AvailableHardware(List<PortResult> ports, boolean dfuFound, boolean stLinkConnected, boolean PCANConnected) {
        this.ports = ports;
        this.dfuFound = dfuFound;
        this.stLinkConnected = stLinkConnected;
        this.PCANConnected = PCANConnected;
    }

    @NotNull
    public List<PortResult> getKnownPorts() {return new ArrayList<>(ports);}

    @NotNull
    public List<PortResult> getKnownPorts(final SerialPortType type) {
        return ports.stream().filter(port -> port.type == type).collect(Collectors.toList());
    }

    @NotNull
    public List<PortResult> getKnownPorts(final Set<SerialPortType> typesToReturn) {
        return ports.stream().filter(port -> typesToReturn.contains(port.type)).collect(Collectors.toList());
    }

    public boolean isDfuFound() {
        return dfuFound;
    }

    public boolean isStLinkConnected() {return stLinkConnected;}
    public boolean isPCANConnected(){return PCANConnected;}

    public boolean isPortAvailable(final PortResult port) {
        return ports.contains(port);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        AvailableHardware that = (AvailableHardware) o;
        return dfuFound == that.dfuFound && stLinkConnected == that.stLinkConnected && PCANConnected == that.PCANConnected && ports.equals(that.ports);
    }

    public boolean isEmpty() {
        return !dfuFound && !stLinkConnected && !PCANConnected && ports.isEmpty();
    }

    @Override
    public String toString() {
        return "AvailableHardware{" +
            "ports=" + ports +
            ", dfuFound=" + dfuFound +
            ", stLinkConnected=" + stLinkConnected +
            ", PCANConnected=" + PCANConnected +
            '}';
    }
}

