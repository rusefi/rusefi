Bluetooth module setup procedure. User Manual.
	
To activate your Bluetooth module, you'll need:
1) Connect your PC to the rusEFI board using UART cable.
2) Run "rusEFI Console" software.
3) Turn on the board power and wait for the console connection.
4) Type the following command:
   "bluetooth_hc06 <baud> <name> <pincode>"
   Where:
   - <baud> is the baud rate of the Bluetooth connection. Allowed values are: 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200.
       Please set this to the baud rate currently used by your UART console.
   - <name> is the name of your Bluetooth connection. It will be visible in the device list on your PC.
   - <pincode> is 4-digit PIN-code needed to authorize the Bluetooth connection to your PC.
   Example: "bluetooth_hc06 38400 rusefi 1234"
5) After entering the command the following text will appear:
	*** Bluetooth module setup procedure ***
	!Warning! Please make sure you're not currently using the BT module for communication (not paired)!
	TO START THE PROCEDURE: PLEASE DISCONNECT YOUR PC COM-PORT FROM THE BOARD NOW!
	After that please don't turn off the board power and wait for ~15 seconds to complete. Then reconnect to the board!
6) If you ignore this message and do nothing, the Bluetooth secup procedure will be cancelled in 10 seconds.
  You can also use "bluetooth_cancel" command to cancel it.
7) If you disconnect the UART cable or close the software, then the procedure will begin.
8) It takes up to 15 seconds to complete. The Bluetooth LED may blink several times during the procedure - it's normal.
9) After that, you may restore the UART connection to your PC or start using Bluetooth module.


*** Developers Section ***

How it works, in two words:
- adds 2 new commands: "bluetooth_hc06(baud, name, pincode)" and "bluetooth_cancel";
- adds stopTsPort() to tunerstudio_io - needed to change the baud rate on fly;
- added bluetoothSoftwareDisconnectNotify() handler to runBinaryProtocolLoop() - to detect UART disconnects;
- added a thread with btThreadEntryPoint():
  - It will wait up to 10 seconds for the user to disconnect the UART cable.
    This is required because the BT setup procedure reads a response from the module during the communication - using the same UART.
    Thus any bytes sent from the Console Software may interfere with the procedure.
- added bluetoothStart() which processes user input and creates a command list;
- added runCommands() where all the Bluetooth magic happens!
