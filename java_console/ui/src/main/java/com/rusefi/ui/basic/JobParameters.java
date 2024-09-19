package com.rusefi.ui.basic;

import com.rusefi.SerialPortScanner;
import com.rusefi.maintenance.JobType;

public class JobParameters {
    public final JobType jobType;
    public final SerialPortScanner.PortResult port;

    public JobParameters(final JobType jobType, final SerialPortScanner.PortResult port) {
        this.jobType = jobType;
        this.port = port;
    }
}
