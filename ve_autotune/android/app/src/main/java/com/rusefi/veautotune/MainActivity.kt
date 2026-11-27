package com.rusefi.veautotune

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.ServiceConnection
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.os.IBinder
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.LinearLayout
import android.widget.Spinner
import android.widget.TextView
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import java.io.File

/**
 * Device picker, Start/Stop control, and a status display.
 *
 * As of the foreground-service follow-up, this Activity owns no Bluetooth
 * connection, polling loop, or [com.rusefi.veautotune.logger.CsvLogger]
 * itself any more -- all of that moved into [RecordingService], a proper
 * foreground service, so a long ride survives this Activity going to the
 * background (screen off, app switched away, or the OS reclaiming it under
 * memory pressure). This class only binds to that service, tells it to
 * start/stop, and reflects its status back into the UI.
 *
 * Global Constraint: unchanged -- still enforced inside RecordingService /
 * RusefiBtConnection, not here. This fix does not touch the protocol layer.
 */
class MainActivity : AppCompatActivity(), RecordingService.Listener {

    private val bluetoothAdapter: BluetoothAdapter? by lazy {
        (getSystemService(Context.BLUETOOTH_SERVICE) as? BluetoothManager)?.adapter
    }

    private lateinit var deviceSpinner: Spinner
    private lateinit var startStopButton: Button
    private lateinit var statusView: TextView

    /** Snapshot of bonded devices backing [deviceSpinner]'s selection. */
    private var devices: List<BluetoothDevice> = emptyList()

    private var boundService: RecordingService? = null
    private var isBound = false

    /** Device to start recording with once the service connection completes,
     * for the (rare) case where Start is tapped before onServiceConnected
     * has run. */
    private var pendingStartDevice: BluetoothDevice? = null

    private val serviceConnection = object : ServiceConnection {
        override fun onServiceConnected(name: ComponentName?, binder: IBinder?) {
            val service = (binder as RecordingService.RecordingBinder).getService()
            boundService = service
            isBound = true
            service.listener = this@MainActivity
            if (service.isRecording) {
                startStopButton.text = "Stop"
            }
            pendingStartDevice?.let { device ->
                pendingStartDevice = null
                service.startRecording(device)
            }
        }

        override fun onServiceDisconnected(name: ComponentName?) {
            boundService?.listener = null
            boundService = null
            isBound = false
        }
    }

    private val requestBluetoothPermission =
        registerForActivityResult(ActivityResultContracts.RequestPermission()) { granted ->
            if (granted) {
                refreshDeviceList()
                beginRecording()
            } else {
                statusView.text = "Bluetooth permission denied; cannot connect."
            }
        }

    // Best-effort: on API 33+ this is required for the recording
    // notification to actually show, but the foreground service still runs
    // fine without it, so a denial here is not fatal.
    private val requestNotificationPermission =
        registerForActivityResult(ActivityResultContracts.RequestPermission()) { }

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
            if (boundService?.isRecording == true) {
                stopRecording()
            } else {
                onStartClicked()
            }
        }

        if (hasBluetoothConnectPermission()) {
            refreshDeviceList()
        }
        maybeRequestNotificationPermission()
    }

    override fun onStart() {
        super.onStart()
        bindService(Intent(this, RecordingService::class.java), serviceConnection, Context.BIND_AUTO_CREATE)
    }

    override fun onStop() {
        super.onStop()
        boundService?.listener = null
        if (isBound) {
            unbindService(serviceConnection)
            isBound = false
        }
        // boundService itself is intentionally left set to null only via
        // onServiceDisconnected/unbind bookkeeping below -- a running
        // recording keeps going in RecordingService regardless, since it
        // was started with startForegroundService, not just bound.
        boundService = null
    }

    private fun maybeRequestNotificationPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU &&
            ContextCompat.checkSelfPermission(this, Manifest.permission.POST_NOTIFICATIONS) !=
            PackageManager.PERMISSION_GRANTED
        ) {
            requestNotificationPermission.launch(Manifest.permission.POST_NOTIFICATIONS)
        }
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

        startStopButton.text = "Stop"
        statusView.text = "Connecting to ${safeDeviceLabel(device)}..."

        val intent = Intent(this, RecordingService::class.java)
        ContextCompat.startForegroundService(this, intent)

        val service = boundService
        if (service != null) {
            service.startRecording(device)
        } else {
            // Not bound yet -- onServiceConnected (triggered by onStart's
            // bindService, or the bind below if that hasn't happened yet
            // for some reason) will start the recording once it fires.
            pendingStartDevice = device
            if (!isBound) {
                bindService(intent, serviceConnection, Context.BIND_AUTO_CREATE)
            }
        }
    }

    private fun stopRecording() {
        boundService?.stopRecording()
    }

    override fun onStatusUpdate(status: String) {
        runOnUiThread { statusView.text = status }
    }

    override fun onRecordingStopped(errorReason: String?, outputFile: File?) {
        runOnUiThread {
            startStopButton.text = "Start"
            statusView.text = buildFinalStatus(errorReason, outputFile)
        }
    }

    private fun buildFinalStatus(errorReason: String?, outputFile: File?): String {
        val prefix = if (errorReason != null) "Stopped ($errorReason)" else "Stopped"
        return if (outputFile != null) "$prefix. Saved to ${outputFile.absolutePath}" else prefix
    }

    private fun safeDeviceLabel(device: BluetoothDevice): String =
        if (hasBluetoothConnectPermission()) (device.name ?: device.address) else device.address
}
