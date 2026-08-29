package com.rusefi.candump;

import peak.can.basic.PCANBasic;
import peak.can.basic.TPCANBaudrate;
import peak.can.basic.TPCANHandle;
import peak.can.basic.TPCANMsg;
import peak.can.basic.TPCANParameter;
import peak.can.basic.TPCANParameterValue;
import peak.can.basic.TPCANStatus;
import peak.can.basic.TPCANTimestamp;
import peak.can.basic.TPCANType;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintStream;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Locale;

/**
 * Standalone PCAN-USB CAN bus dumper for macOS (MacCAN/libpcanbasic), writing
 * PCAN-View compatible .trc files.
 *
 * <p>Designed for capture sessions with the STOCK ECU: the adapter runs in
 * LISTEN-ONLY mode by default (it never ACKs on the bus, so the dump does not
 * disturb the vehicle), all standard and extended frames are recorded with
 * the driver's microsecond timestamps, and bus status transitions
 * (BUSOFF/BUSHEAVY/BUSLIGHT/BUSPASSIVE) are written into the stream whenever
 * the driver delivers them.
 *
 * <p>Usage:
 * <pre>
 *   can_dump.sh [--channel N] [--bitrate 500k|250k|1m] [-o file.trc] [--active] [--duration seconds]
 * </pre>
 * Ctrl+C stops the dump; the shutdown hook flushes and closes the file.
 */
public final class CanDump {

    private static final String VERSION = "1.0";

    private static final byte MSGTYPE_RTR = 0x01;
    private static final byte MSGTYPE_EXTENDED = 0x02;
    private static final byte MSGTYPE_ERRFRAME = 0x40;
    private static final byte MSGTYPE_STATUS = (byte) 0x80;

    private final PrintStream out;
    private final TPCANHandle channel;
    private final TPCANBaudrate baudrate;
    private final String baudrateName;
    private final File outputFile;
    private final boolean listenOnly;
    private final long durationSeconds;

    	private PCANBasic can;
    	private BufferedWriter writer;
    	private final Object writeLock = new Object();
    	private volatile boolean shuttingDown;
    	private long messageCount;
    private long errorCount;
    private long overrunCount;
    private long firstFrameNanos;
    private long lastFlushNanos;
    private long lastStatsNanos;
    private long statsWindowStartNanos;
    private long statsWindowFrames;

    private long t0DriverMillis;
    private long t0DriverMicros;
    private boolean haveT0;
    private int zeroTimestampFrames;
    private boolean usingNanoTime;

    public CanDump(PrintStream out, TPCANHandle channel, TPCANBaudrate baudrate, String baudrateName,
                   File outputFile, boolean listenOnly, long durationSeconds) {
        this.out = out;
        this.channel = channel;
        this.baudrate = baudrate;
        this.baudrateName = baudrateName;
        this.outputFile = outputFile;
        this.listenOnly = listenOnly;
        this.durationSeconds = durationSeconds;
    }

    public static void main(String[] args) {
        TPCANHandle channel = TPCANHandle.PCAN_USBBUS1;
        TPCANBaudrate baudrate = TPCANBaudrate.PCAN_BAUD_500K;
        String baudrateName = "500 kbit/s";
        File output = null;
        boolean listenOnly = true;
        long duration = -1;

        for (int i = 0; i < args.length; i++) {
            String a = args[i];
            switch (a) {
                case "-h": case "--help":
                    printUsage(System.err);
                    return;
                case "--channel":
                    channel = resolveChannel(requireValue(args, ++i, "--channel"));
                    break;
                case "--bitrate":
                    String br = requireValue(args, ++i, "--bitrate");
                    switch (br.toLowerCase(Locale.ROOT)) {
                        case "500k": baudrate = TPCANBaudrate.PCAN_BAUD_500K; baudrateName = "500 kbit/s"; break;
                        case "250k": baudrate = TPCANBaudrate.PCAN_BAUD_250K; baudrateName = "250 kbit/s"; break;
                        case "1m": baudrate = TPCANBaudrate.PCAN_BAUD_1M; baudrateName = "1 Mbit/s"; break;
                        default: throw new IllegalArgumentException("Unknown bitrate '" + br + "' (use 500k, 250k or 1m)");
                    }
                    break;
                case "-o": case "--output":
                    output = new File(requireValue(args, ++i, "-o"));
                    break;
                case "--active":
                    listenOnly = false;
                    break;
                case "--duration":
                    duration = Long.parseLong(requireValue(args, ++i, "--duration"));
                    if (duration <= 0) {
                        throw new IllegalArgumentException("--duration must be positive");
                    }
                    break;
                default:
                    throw new IllegalArgumentException("Unknown option '" + a + "' (use --help)");
            }
        }

        if (output == null) {
            output = new File("can_dump_" + LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyyMMdd_HHmmss")) + ".trc");
        }

        try {
            CanDump dump = new CanDump(System.out, channel, baudrate, baudrateName, output, listenOnly, duration);
            dump.run();
        } catch (Throwable t) {
            System.err.println("FATAL: " + t.getMessage());
            System.exit(1);
        }
    }

    private static String requireValue(String[] args, int i, String option) {
        if (i >= args.length) {
            throw new IllegalArgumentException(option + " requires a value");
        }
        return args[i];
    }

    private static TPCANHandle resolveChannel(String value) {
        String v = value.trim();
        try {
            if (v.matches("\\d+")) {
                return TPCANHandle.valueOf("PCAN_USBBUS" + v);
            }
            return TPCANHandle.valueOf(v.toUpperCase(Locale.ROOT));
        } catch (IllegalArgumentException e) {
            throw new IllegalArgumentException("Unknown PCAN channel '" + value + "' (use 1, 2, ... or PCAN_USBBUSn)");
        }
    }

    private static void printUsage(PrintStream s) {
        s.println("Usage: can_dump.sh [options]");
        s.println("Record a PCAN-USB CAN bus dump to a PCAN-View compatible .trc file.");
        s.println("  -o, --output <file.trc>  output file (default can_dump_YYYYMMDD_HHMMSS.trc)");
        s.println("      --channel <n>        PCAN-USB channel (default 1)");
        s.println("      --bitrate <500k|250k|1m>  bus bitrate (default 500k; the vehicle bus is 500k)");
        s.println("      --active             normal mode (adapter ACKs on the bus); default is listen-only");
        s.println("      --duration <seconds> stop automatically after N seconds (default: until Ctrl+C)");
        s.println("Ctrl+C stops the dump; the file is flushed on exit.");
        s.println("NOTE: close the rusEFI console first - it holds the PCAN adapter exclusively.");
    }

    public void run() throws IOException {
        can = new PCANBasic();
        can.initializeAPI();

        out.println("Opening CAN channel " + channel + " at " + baudrateName + (listenOnly ? " (listen-only)" : " (active)") + "...");

        TPCANStatus init = can.Initialize(channel, baudrate, TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
        if (init != TPCANStatus.PCAN_ERROR_OK) {
            throw new IOException("PCAN Initialize failed: " + init + " - is another application (rusEFI console, PCAN-View) holding the adapter?");
        }

        // Listen-only keeps the sniffer invisible on the bus (no ACKs).
        if (listenOnly) {
            TPCANStatus lo = can.SetValue(channel, TPCANParameter.PCAN_LISTEN_ONLY,
                    TPCANParameterValue.PCAN_PARAMETER_ON, 4);
            if (lo != TPCANStatus.PCAN_ERROR_OK) {
                out.println("WARNING: listen-only mode not supported by the driver (" + lo + ") - running active");
            }
        }

        // Ask the driver to deliver bus-status transitions as regular frames.
        can.SetValue(channel, TPCANParameter.PCAN_ALLOW_STATUS_FRAMES,
                TPCANParameterValue.PCAN_PARAMETER_ON, 4);

        String device = readDeviceInfo();

        writer = new BufferedWriter(new FileWriter(outputFile), 1 << 20);
        writeHeader(device);

        // Flush and uninitialize even when killed with SIGINT.
        Thread shutdownHook = new Thread(this::closeQuietly, "can-dump-shutdown");
        Runtime.getRuntime().addShutdownHook(shutdownHook);

        out.println("Recording to " + outputFile.getAbsolutePath() + " - press Ctrl+C to stop.");

        long startNanos = System.nanoTime();
        try {
            readLoop(startNanos);
        } finally {
            closeQuietly();
            try {
                Runtime.getRuntime().removeShutdownHook(shutdownHook);
            } catch (IllegalStateException ignored) {
                // JVM is already shutting down
            }
        }

        out.println("Done: " + messageCount + " frames, " + errorCount + " errors, " + overrunCount + " overruns -> " + outputFile.getAbsolutePath());
    }

    private String readDeviceInfo() {
        try {
            StringBuilder sb = new StringBuilder(256);
            TPCANStatus st = can.GetValue(channel, TPCANParameter.PCAN_CHANNEL_VERSION, sb, 256);
            if (st == TPCANStatus.PCAN_ERROR_OK && sb.length() > 0) {
                return sb.toString().trim();
            }
        } catch (Throwable ignored) {
        }
        return null;
    }

    private void writeHeader(String device) throws IOException {
        LocalDateTime now = LocalDateTime.now();
        writer.write(";$FILEVERSION=1.1\n");
        writer.write(";$STARTTIME=" + String.format(Locale.ROOT, "%.9f", CanDumpFormat.startTimeSerial(now)) + "\n");
        writer.write(";\n");
        writer.write(";   Start time: " + CanDumpFormat.startTimeText(now) + "\n");
        writer.write(";   Generated by rusEFI CanDump v" + VERSION + "\n");
        writer.write(";   Channel: " + channel + "\n");
        writer.write(";   Bit rate: " + baudrateName + "\n");
        writer.write(";   Mode: " + (listenOnly ? "listen-only (no ACK on the bus)" : "active") + "\n");
        if (device != null) {
            writer.write(";   Device: " + device + "\n");
        }
        writer.write(";   Timestamps: PCAN driver timestamps (1 us), time offset relative to the first frame\n");
        writer.write(";\n");
        writer.write(";   Message Number\n");
        writer.write(";   |         Time Offset (ms)\n");
        writer.write(";   |         |        Type\n");
        writer.write(";   |         |        |        ID (hex)\n");
        writer.write(";   |         |        |        |     Data Length\n");
        writer.write(";   |         |        |        |     |   Data Bytes (hex) ...\n");
        writer.write(";   |         |        |        |     |   |\n");
        writer.write(";---+--   ----+----  --+--  ----+---  +  -+ -- -- -- -- -- -- --\n");
        writer.flush();
    }

    	private void readLoop(long startNanos) throws IOException {
        		firstFrameNanos = startNanos;
        		lastFlushNanos = startNanos;
        		lastStatsNanos = startNanos;
        		statsWindowStartNanos = startNanos;

        		while (!shuttingDown) {
            if (durationSeconds > 0) {
                long elapsed = (System.nanoTime() - startNanos) / 1_000_000_000;
                if (elapsed >= durationSeconds) {
                    return;
                }
            }

            TPCANMsg rx = new TPCANMsg(Byte.MAX_VALUE);
            TPCANTimestamp ts = new TPCANTimestamp();
            TPCANStatus status = can.Read(channel, rx, ts);

            if (status == TPCANStatus.PCAN_ERROR_OK) {
                handleMessage(rx, ts);
            } else if (status == TPCANStatus.PCAN_ERROR_QRCVEMPTY) {
                sleepTiny();
            } else if (status == TPCANStatus.PCAN_ERROR_QOVERRUN) {
                overrunCount++;
                writeStatusLine(offsetNow(), "QOVERRUN (receive queue overflow)", 0x40);
            } else {
                errorCount++;
                writeStatusLine(offsetNow(), "read error " + status, status.getValue());
                sleepTiny();
            }

            			long nowNanos = System.nanoTime();
            			if (nowNanos - lastFlushNanos >= 2_000_000_000L) {
            				synchronized (writeLock) {
            					if (writer != null) {
            						writer.flush();
            					}
            				}
            				lastFlushNanos = nowNanos;
            			}
            if (nowNanos - lastStatsNanos >= 5_000_000_000L) {
                long windowFrames = messageCount - statsWindowFrames;
                double windowSec = (nowNanos - statsWindowStartNanos) / 1e9;
                out.printf(Locale.ROOT, "%d frames total, %.0f frames/s, %d errors, %d overruns%n",
                        messageCount, windowFrames / Math.max(windowSec, 0.001), errorCount, overrunCount);
                lastStatsNanos = nowNanos;
                statsWindowStartNanos = nowNanos;
                statsWindowFrames = messageCount;
            }
        }
    }

    	private void handleMessage(TPCANMsg rx, TPCANTimestamp ts) throws IOException {
    		double offsetMs = offset(rx, ts);
    		long number = ++messageCount;

    		byte type = rx.getType();

    		String row;
    		if ((type & MSGTYPE_STATUS) != 0) {
    			// Status frame: the data bytes carry the PCAN status code (LE).
    			int code = statusCode(rx);
    			String name = CanDumpFormat.statusName(code);
    			row = CanDumpFormat.statusRow(number, offsetMs,
    					name != null ? name : "STATUS", code);
    		} else if ((type & MSGTYPE_ERRFRAME) != 0) {
    			errorCount++;
    			row = CanDumpFormat.statusRow(number, offsetMs, "ERRFRAME " + CanDumpFormat.hex(rx.getData(), rx.getLength()), 0x40);
    		} else {
    			boolean extended = (type & MSGTYPE_EXTENDED) != 0;
    			int dlc = rx.getLength() & 0xFF;
    			row = CanDumpFormat.messageRow(number, offsetMs, "Rx",
    					rx.getID(), extended, dlc, rx.getData());
    		}

    		// The shutdown hook flushes/closes the writer from another thread
    		// (SIGINT) while this loop may still be running - serialize on the
    		// writer so a flush can never interleave with a half-written row.
    		synchronized (writeLock) {
    			if (writer == null) {
    				return;
    			}
    			writer.write(row);
    			writer.newLine();
    			if (messageCount % 500 == 0) {
    				writer.flush();
    			}
    		}
    	}

    private int statusCode(TPCANMsg rx) {
        byte[] d = rx.getData();
        int len = rx.getLength() & 0xFF;
        if (len >= 4) {
            return (d[0] & 0xFF) | ((d[1] & 0xFF) << 8) | ((d[2] & 0xFF) << 16) | ((d[3] & 0xFF) << 24);
        }
        return 0;
    }

    /**
     * Time offset in ms relative to the first frame. Prefers the driver's
     * hardware timestamps; falls back to host nanoTime when the driver
     * reports no timestamps (older MacCAN versions).
     */
    private double offset(TPCANMsg rx, TPCANTimestamp ts) {
        long driverMillis = (((long) (ts.getMillis_overflow() & 0xFFFF)) << 32) | (ts.getMillis() & 0xFFFFFFFFL);
        long driverMicros = ts.getMicros() & 0xFFFF;

        if (!haveT0 && !usingNanoTime) {
            if (driverMillis == 0 && driverMicros == 0) {
                zeroTimestampFrames++;
                // The driver gave no timestamp for the first frames - switch to host time.
                if (zeroTimestampFrames >= 10) {
                    usingNanoTime = true;
                }
            }
        }

        if (usingNanoTime) {
            long now = System.nanoTime();
            if (!haveT0) {
                haveT0 = true;
                firstFrameNanos = now;
                return 0;
            }
            return (now - firstFrameNanos) / 1e6;
        }

        if (!haveT0) {
            haveT0 = true;
            t0DriverMillis = driverMillis;
            t0DriverMicros = driverMicros;
            return 0;
        }
        return (driverMillis - t0DriverMillis) + (driverMicros - t0DriverMicros) / 1000.0;
    }

    private double offsetNow() {
        if (haveT0) {
            return (System.nanoTime() - firstFrameNanos) / 1e6;
        }
        return 0;
    }

    	private void writeStatusLine(double offsetMs, String text, int code) throws IOException {
    		synchronized (writeLock) {
    			if (writer == null) {
    				return;
    			}
    			writer.write(CanDumpFormat.statusRow(++messageCount, offsetMs, text, code));
    			writer.newLine();
    			writer.flush();
    		}
    	}

    private static void sleepTiny() {
        try {
            Thread.sleep(1);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    	private void closeQuietly() {
    		shuttingDown = true;
    		synchronized (writeLock) {
    			try {
    				if (writer != null) {
    					writer.flush();
    					writer.close();
    					writer = null;
    				}
    			} catch (IOException ignored) {
    			}
    		}
    		try {
    			if (can != null) {
    				can.Uninitialize(channel);
    				can = null;
    			}
    		} catch (Throwable ignored) {
    		}
    	}
}
