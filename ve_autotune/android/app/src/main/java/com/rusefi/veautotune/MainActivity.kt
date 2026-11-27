package com.rusefi.veautotune

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.LinearLayout
import android.widget.Spinner
import android.widget.TextView
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
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
 * Device picker, Start/Stop control, and a once-per-second polling loop that
 * reads the ECU's output channels over Bluetooth and appends them to a CSV
 * file on external storage.
 *
 * Global Constraint: this class never builds or sends any ECU command
 * itself -- it only ever calls [RusefiBtConnection.readChannels], which is
 * the sole place in the app allowed to build the output-channel read
 * request (see [com.rusefi.veautotune.protocol.buildOutputChannelRequest]).
 */
class MainActivity : AppCompatActivity() {

    private val bluetoothAdapter: BluetoothAdapter? by lazy {
        (getSystemService(Context.BLUETOOTH_SERVICE) as? BluetoothManager)?.adapter
    }

    private lateinit var deviceSpinner: Spinner
    private lateinit var startStopButton: Button
    private lateinit var statusView: TextView

    /** Snapshot of bonded devices backing [deviceSpinner]'s selection. */
    private var devices: List<BluetoothDevice> = emptyList()

    private var isRecording = false
    private var userRequestedStop = false

    private var activeSocket: BluetoothSocket? = null
    private var activeWriter: BufferedWriter? = null
    private var pollingJob: Job? = null

    private val mainScope = CoroutineScope(SupervisorJob() + Dispatchers.Main)

    private val requestBluetoothPermission =
        registerForActivityResult(ActivityResultContracts.RequestPermission()) { granted ->
            if (granted) {
                refreshDeviceList()
                beginRecording()
            } else {
                statusView.text = "Bluetooth permission denied; cannot connect."
            }
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        deviceSpinner = Spinner(this)
        startStopButton = Button(this).apply { text = "Start" }
        statusView = TextView(this).apply {
            text = "Select a paired rusEFI device and press Start."
        }

        val root = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(32, 32, 32, 32)
            addView(deviceSpinner)
            addView(startStopButton)
            addView(statusView)
        }
        setContentView(root)

        startStopButton.setOnClickListener {
            if (isRecording) {
                stopRecording()
            } else {
                onStartClicked()
            }
        }

        if (hasBluetoothConnectPermission()) {
            refreshDeviceList()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        mainScope.cancel()
        closeResources()
    }

    private fun hasBluetoothConnectPermission(): Boolean {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S) return true
        return ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) ==
            PackageManager.PERMISSION_GRANTED
    }

    private fun refreshDeviceList() {
        val adapter = bluetoothAdapter
        if (adapter == null) {
            statusView.text = "This device has no Bluetooth adapter."
            return
        }
        if (!hasBluetoothConnectPermission()) return

        devices = adapter.bondedDevices.toList()
        val names = devices.map { it.name ?: it.address }
        deviceSpinner.adapter = ArrayAdapter(this, android.R.layout.simple_spinner_item, names).apply {
            setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        }
    }

    private fun onStartClicked() {
        if (!hasBluetoothConnectPermission()) {
            requestBluetoothPermission.launch(Manifest.permission.BLUETOOTH_CONNECT)
            return
        }
        if (devices.isEmpty()) {
            refreshDeviceList()
        }
        beginRecording()
    }

    private fun beginRecording() {
        val device = devices.getOrNull(deviceSpinner.selectedItemPosition)
        if (device == null) {
            statusView.text = "No paired rusEFI device selected. Pair it in Android Settings first."
            return
        }

        userRequestedStop = false
        isRecording = true
        startStopButton.text = "Stop"
        statusView.text = "Connecting to ${safeDeviceLabel(device)}..."

        pollingJob = mainScope.launch {
            var outputFile: File? = null
            var errorReason: String? = null
            try {
                val socket = withContext(Dispatchers.IO) {
                    val s = device.createRfcommSocketToServiceRecord(RusefiSpp.UUID)
                    s.connect()
                    s
                }
                activeSocket = socket

                val connection = RusefiBtConnection(SocketTransport(socket))

                val timestamp = SimpleDateFormat("yyyyMMdd-HHmmss", Locale.US).format(Date())
                val file = File(getExternalFilesDir(null), "ride_$timestamp.csv")
                val writer = BufferedWriter(FileWriter(file))
                activeWriter = writer
                outputFile = file

                val logger = CsvLogger(writer)
                logger.writeHeader()

                statusView.text = "Recording to ${file.name}..."

                val startNanos = System.nanoTime()
                while (isActive) {
                    val channels = withContext(Dispatchers.IO) { connection.readChannels() }
                    val tSeconds = (System.nanoTime() - startNanos) / 1_000_000_000.0
                    logger.appendRow(tSeconds, channels)
                    statusView.text = "Recording... RPM = ${channels.rpm.toInt()}"
                    delay(1000)
                }
            } catch (e: BadResponse) {
                errorReason = "bad response from ECU: ${e.message}"
            } catch (e: IOException) {
                if (!userRequestedStop) {
                    errorReason = "Bluetooth I/O error: ${e.message}"
                }
            } catch (e: SecurityException) {
                errorReason = "Bluetooth permission error: ${e.message}"
            } finally {
                closeResources()
                isRecording = false
                startStopButton.text = "Start"
                statusView.text = buildFinalStatus(errorReason, outputFile)
            }
        }
    }

    private fun stopRecording() {
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

    private fun buildFinalStatus(errorReason: String?, outputFile: File?): String {
        val prefix = if (errorReason != null) "Stopped ($errorReason)" else "Stopped"
        return if (outputFile != null) "$prefix. Saved to ${outputFile.absolutePath}" else prefix
    }

    private fun safeDeviceLabel(device: BluetoothDevice): String =
        if (hasBluetoothConnectPermission()) (device.name ?: device.address) else device.address
}
