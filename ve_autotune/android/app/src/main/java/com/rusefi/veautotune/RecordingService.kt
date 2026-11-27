package com.rusefi.veautotune

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Intent
import android.content.pm.ServiceInfo
import android.os.Binder
import android.os.Build
import android.os.IBinder
import androidx.core.app.NotificationCompat
import com.rusefi.veautotune.bt.RusefiBtConnection
import com.rusefi.veautotune.bt.RusefiSpp
import com.rusefi.veautotune.bt.SocketTransport
import com.rusefi.veautotune.logger.CsvLogger
import com.rusefi.veautotune.protocol.BadResponse
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.cancel
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.BufferedWriter
import java.io.File
import java.io.FileWriter
import java.io.IOException
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

/**
 * How many consecutive [BadResponse] failures (CRC mismatch or non-OK
 * status -- ordinary Bluetooth bit noise over a long ride) are tolerated
 * before the recording session is ended as a connection error. Generous
 * enough that transient BT noise doesn't trip it, small enough that a
 * truly dead link still ends the session in a few seconds at the 1Hz poll
 * rate (5 seconds at the current poll interval).
 */
private const val MAX_CONSECUTIVE_BAD_FRAMES = 5

private const val NOTIFICATION_CHANNEL_ID = "recording"
private const val NOTIFICATION_ID = 1

/**
 * Pure (no Android framework dependency) consecutive-bad-frame counter,
 * extracted out of the polling loop below specifically so it can be unit
 * tested on the JVM -- unlike the rest of this file, which needs a real
 * Android `Service`/notification/BT runtime (same situation as
 * [SocketTransport]) and so has no direct unit test.
 */
class BadFrameTracker(private val maxConsecutive: Int) {

    var consecutiveFailures: Int = 0
        private set

    /** Records one more bad frame; returns true once the link should be
     * treated as dead (i.e. [consecutiveFailures] has reached [maxConsecutive]). */
    fun recordFailure(): Boolean {
        consecutiveFailures++
        return consecutiveFailures >= maxConsecutive
    }

    /** Records a good frame, resetting the streak. */
    fun recordSuccess() {
        consecutiveFailures = 0
    }
}

/**
 * Foreground service that owns the BT connection, the polling loop, and the
 * [CsvLogger] for a recording session -- moved here from `MainActivity` so
 * the session survives the Activity going to the background (screen off,
 * app switch, or the OS reclaiming it under memory pressure) during a long
 * ride instead of being silently killed or throttled by Doze.
 *
 * Global Constraint: unchanged from `MainActivity`'s previous ownership of
 * this logic -- this class never builds or sends any ECU command itself, it
 * only ever calls [RusefiBtConnection.readChannels]. This fix does not touch
 * the protocol layer at all.
 */
class RecordingService : Service() {

    /** Status/result callback for whatever Activity is currently bound. */
    interface Listener {
        fun onStatusUpdate(status: String)
        fun onRecordingStopped(errorReason: String?, outputFile: File?)
    }

    inner class RecordingBinder : Binder() {
        fun getService(): RecordingService = this@RecordingService
    }

    private val binder = RecordingBinder()

    /** Set by whichever Activity is currently bound; cleared on unbind. */
    var listener: Listener? = null

    var isRecording: Boolean = false
        private set

    private var userRequestedStop = false
    private var activeSocket: BluetoothSocket? = null
    private var activeWriter: BufferedWriter? = null
    private var pollingJob: Job? = null

    private val serviceScope = CoroutineScope(SupervisorJob() + Dispatchers.Main)

    override fun onBind(intent: Intent?): IBinder = binder

    override fun onDestroy() {
        super.onDestroy()
        serviceScope.cancel()
        closeResources()
    }

    /** Starts (or no-ops if already) a recording session against [device]. */
    fun startRecording(device: BluetoothDevice) {
        if (isRecording) return

        userRequestedStop = false
        isRecording = true

        // Must happen synchronously, before any suspending work, so the
        // foreground-service start window (required once startForegroundService
        // has been called on this service) is satisfied promptly.
        startForegroundNotification("Recording ECU data...")

        pollingJob = serviceScope.launch {
            var outputFile: File? = null
            var errorReason: String? = null
            try {
                val socket = withContext(Dispatchers.IO) {
                    val s = device.createRfcommSocketToServiceRecord(RusefiSpp.UUID)
                    // Publish to activeSocket BEFORE the blocking connect() call so
                    // that if Stop fires (or the service is destroyed) while
                    // connect() is still in flight, stopRecording()'s
                    // activeSocket?.close() can reach this socket and interrupt
                    // the connection attempt instead of it completing unobserved
                    // and leaking an RFCOMM connection to the BT module.
                    activeSocket = s
                    try {
                        s.connect()
                    } catch (e: Throwable) {
                        // connect() failed or was interrupted by a concurrent
                        // close()/cancellation -- make sure the socket doesn't
                        // end up connected-but-abandoned in either direction.
                        try {
                            s.close()
                        } catch (closeException: IOException) {
                            // Already failing; nothing more to do with this.
                        }
                        activeSocket = null
                        throw e
                    }
                    s
                }

                val connection = RusefiBtConnection(SocketTransport(socket))

                val timestamp = SimpleDateFormat("yyyyMMdd-HHmmss", Locale.US).format(Date())
                val file = File(getExternalFilesDir(null), "ride_$timestamp.csv")
                val writer = BufferedWriter(FileWriter(file))
                activeWriter = writer
                outputFile = file

                val logger = CsvLogger(writer)
                logger.writeHeader()

                reportStatus("Recording to ${file.name}...")

                val startNanos = System.nanoTime()
                val badFrames = BadFrameTracker(MAX_CONSECUTIVE_BAD_FRAMES)
                while (isActive) {
                    val channels = try {
                        withContext(Dispatchers.IO) { connection.readChannels() }
                    } catch (e: BadResponse) {
                        // Ordinary BT bit noise (CRC mismatch or non-OK status) --
                        // drop this one row and retry rather than ending the
                        // whole ride on the first bad frame. Only a run of
                        // MAX_CONSECUTIVE_BAD_FRAMES in a row means the link is
                        // actually dead.
                        val dead = badFrames.recordFailure()
                        if (dead) {
                            errorReason = "bad response from ECU: ${e.message}"
                            break
                        }
                        reportStatus(
                            "Recording... bad frame, retrying... " +
                                "(${badFrames.consecutiveFailures}/$MAX_CONSECUTIVE_BAD_FRAMES)"
                        )
                        delay(1000)
                        continue
                    }
                    badFrames.recordSuccess()
                    val tSeconds = (System.nanoTime() - startNanos) / 1_000_000_000.0
                    logger.appendRow(tSeconds, channels)
                    reportStatus("Recording... RPM = ${channels.rpm.toInt()}")
                    delay(1000)
                }
            } catch (e: IOException) {
                if (!userRequestedStop) {
                    errorReason = "Bluetooth I/O error: ${e.message}"
                }
            } catch (e: SecurityException) {
                errorReason = "Bluetooth permission error: ${e.message}"
            } finally {
                closeResources()
                isRecording = false
                listener?.onRecordingStopped(errorReason, outputFile)
                stopForegroundAndSelf()
            }
        }
    }

    fun stopRecording() {
        userRequestedStop = true
        pollingJob?.cancel()
        // Closing the socket breaks any in-flight blocking read, so the
        // polling coroutine's IOException branch runs promptly instead of
        // waiting indefinitely on the next scheduled poll.
        try {
            activeSocket?.close()
        } catch (e: IOException) {
            // Already stopping; nothing more to do with this.
        }
    }

    private fun reportStatus(status: String) {
        listener?.onStatusUpdate(status)
        updateNotification(status)
    }

    private fun closeResources() {
        try {
            activeWriter?.flush()
        } catch (e: IOException) {
            // Best effort -- we're tearing down regardless.
        }
        try {
            activeWriter?.close()
        } catch (e: IOException) {
            // Best effort -- we're tearing down regardless.
        }
        try {
            activeSocket?.close()
        } catch (e: IOException) {
            // Best effort -- we're tearing down regardless.
        }
        activeWriter = null
        activeSocket = null
    }

    private fun stopForegroundAndSelf() {
        @Suppress("DEPRECATION")
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            stopForeground(STOP_FOREGROUND_REMOVE)
        } else {
            stopForeground(true)
        }
        stopSelf()
    }

    private fun ensureNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val manager = getSystemService(NotificationManager::class.java)
            if (manager.getNotificationChannel(NOTIFICATION_CHANNEL_ID) == null) {
                val channel = NotificationChannel(
                    NOTIFICATION_CHANNEL_ID,
                    "ECU recording",
                    NotificationManager.IMPORTANCE_LOW,
                )
                manager.createNotificationChannel(channel)
            }
        }
    }

    private fun buildNotification(text: String): Notification {
        ensureNotificationChannel()
        return NotificationCompat.Builder(this, NOTIFICATION_CHANNEL_ID)
            .setContentTitle("VE Autotune Recorder")
            .setContentText(text)
            .setSmallIcon(android.R.drawable.stat_notify_sync)
            .setOngoing(true)
            .setPriority(NotificationCompat.PRIORITY_LOW)
            .build()
    }

    /**
     * Starts this service in the foreground with [text] as the initial
     * notification content. Must be called synchronously from
     * [startRecording], before any suspending work.
     */
    private fun startForegroundNotification(text: String) {
        val notification = buildNotification(text)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            startForeground(NOTIFICATION_ID, notification, ServiceInfo.FOREGROUND_SERVICE_TYPE_CONNECTED_DEVICE)
        } else {
            // API 26-28: FOREGROUND_SERVICE_TYPE_CONNECTED_DEVICE doesn't
            // apply the same way (the typed startForeground overload was
            // added in API 29) -- a plain startForeground is correct here.
            startForeground(NOTIFICATION_ID, notification)
        }
    }

    private fun updateNotification(text: String) {
        if (!isRecording) return
        val manager = getSystemService(NotificationManager::class.java)
        manager.notify(NOTIFICATION_ID, buildNotification(text))
    }
}
