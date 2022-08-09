/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * $Id: PCANBasic.java 12274 2021-08-30 12:20:04Z Fabrice $
 * @LastChange $Date: 2021-08-30 14:20:04 +0200 (Mon, 30 Aug 2021) $
 * 
 * PCANBasic JAVA Interface.
 *
 * Copyright (C) 2001-2021  PEAK System-Technik GmbH <www.peak-system.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * PCAN is a registered Trademark of PEAK-System Germany GmbH
 *
 * Author: 		 Jonathan Urban/Uwe Wilhelm/Fabrice Vergnaud
 * Contact:      <linux@peak-system.com>
 * Maintainer:   Fabrice Vergnaud <f.vergnaud@peak-system.com>
 */
package peak.can.basic;

/**
 * This is the main class for using the PCANBasic API with your java
 * applications. Steps to use this api:<br><br>
 * <pre>
 * 1. Create a new PCANBasic object:<br>
 *    example: <br>
 *    can = new PCANBasic();
 *
 * 2. Call the initializeAPI method<br>
 *    example: <br>
 *    can.initializeAPI();<br>
 *
 * 3. Call the Initialize method passing the TPCANHandle parameter which you want use, TPCANBaudrate and other parameters for Non-PNP devices <br>
 *    example: <br>
 *    can.initialize(TPCANHandle.PCAN_USBBUS1, TPCANBaudrate.PCAN_BAUD_1M);<br>
 *
 * 4. Call the read or write method passing the TPCANHandle parameter which is initialized and you want use<br>
 *    example:
 *	TPCANMsg msg = new TPCANMsg();;
 *	can.Read(TPCANHandle.PCAN_USBBUS1, msg, null);
 *	can.Write(TPCANHandle.PCAN_USBBUS1, msg);
 *    (do not forget to check if msg is null after calling the Read method)<br>
 *
 * 5. At the end call the Uninitialize method
 *    example:
 *    can.Uninitialize(TPCANHandle.PCAN_USBBUS1);
 * </pre>
 * <br>
 * A minimalistic program that writes every can message that it receives
 * (ping-pong)<br>
 * looks like this:<br>
 * <pre>
 *import peak.can.basic.*;
 *
 *public class MinimalisticProgram
 *{
 *      public static void main(String[] args)
 *      {
 *          PCANBasic can = null;
 *          TPCANMsg msg = null;
 *          TPCANStatus status = null;
 *          can = new PCANBasic();
 *          if(!can.initializeAPI())
 *          {
 *              System.out.println("Unable to initialize the API");
 *              System.exit(0);
 *          }
 *          status = can.Initialize(TPCANHandle.PCAN_PCIBUS1, TPCANBaudrate.PCAN_BAUD_1M, TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
 *          msg = new TPCANMsg();
 *          while(true)
 *          {
 *              while(can.Read(TPCANHandle.PCAN_PCIBUS1, msg, null) == TPCANStatus.PCAN_ERROR_OK)
 *              {
 *                  status = can.Write(TPCANHandle.PCAN_PCIBUS1, msg);
 *                  if(status != TPCANStatus.PCAN_ERROR_OK)
 *                  {
 *                      System.out.println("Unable to write the CAN message");
 *                      System.exit(0);
 *                  }
 *              }
 *          }
 *      }
 *}
 * </pre>
 */
public class PCANBasic {

    /**
     * Initializes a PCAN Channel
     *
     * @param Channel The handle of a PCAN Channel
     * @param Btr0Btr1 The speed for the communication (BTR0BTR1 code)
     * @param HwType NON PLUG'n'PLAY: The type of hardware and operation mode
     * @param IOPort NON PLUG'n'PLAY: The I/O address for the parallel port
     * @param Interrupt NON PLUG'n'PLAY: Interrupt number of the parallel port
     * @return a TPCANStatus error code
     */
    public native TPCANStatus Initialize(
            TPCANHandle Channel,
            TPCANBaudrate Btr0Btr1,
            TPCANType HwType,
            int IOPort,
            short Interrupt);

    /**
     * Initializes a FD capable PCAN Channel
     *
     * <p>
     * See PCAN_BR_* values Bitrate string must follow the following
     * construction rules: - parameters and values must be separated by '=' -
     * Couples of Parameter/value must be separated by ',' - Following Parameter
     * must be filled out: f_clock, data_brp, data_sjw, data_tseg1, data_tseg2,
     * nom_brp, nom_sjw, nom_tseg1, nom_tseg2. - Following Parameters are
     * optional (not used yet): data_ssp_offset, nom_samp
     * </p>
     * <p>
     * Example: f_clock_mhz=80, nom_brp=1, nom_tset1=63, nom_tseg2=16,
     * nom_sjw=7, data_brp=4, data_tset1=12, data_tseg2=7, data_sjw=1
     * </p>
     *
     * @param Channel The handle of a FD capable PCAN Channel
     * @param BitrateFD The speed for the communication (FD Bitrate string)
     * @return A TPCANStatus error code
     */
    public native TPCANStatus InitializeFD(
            TPCANHandle Channel,
            TPCANBitrateFD BitrateFD);

    /**
     * Uninitializes one or all PCAN Channels initialized by CAN_Initialize
     * Giving the TPCANHandle value "PCAN_NONEBUS", uninitialize all initialized
     * channels
     *
     * @param Channel The handle of a PCAN Channel
     * @return A TPCANStatus error code
     */
    public native TPCANStatus Uninitialize(TPCANHandle Channel);

    /**
     * Resets the receive and transmit queues of the PCAN Channel A reset of the
     * CAN controller is not performed.
     *
     * @param Channel The handle of a PCAN Channel
     * @return A TPCANStatus error code
     */
    public native TPCANStatus Reset(
            TPCANHandle Channel);
    
    /**
     * Gets the current status of a PCAN Channel
     *
     * @param Channel The handle of a PCAN Channel
     * @return A TPCANStatus error code
     */
    public native TPCANStatus GetStatus(
            TPCANHandle Channel);
    
    /**
     * Transmits a CAN message
     *
     * @param Channel The handle of a PCAN Channel
     * @param MessageBuffer A TPCANMsg buffer with the message to be read
     * @param TimestampBuffer A TPCANTimestamp structure buffer to get the
     * reception time of the message. If this value is not desired, this
     * parameter should be passed as NULL
     * @return A TPCANStatus error code
     */
    public native TPCANStatus Read(
            TPCANHandle Channel,
            TPCANMsg MessageBuffer,
            TPCANTimestamp TimestampBuffer);
    
    /**
     * Reads a CAN message from the receive queue of a FD capable PCAN Channel
     *
     * @param Channel The handle of a FD capable PCAN Channel
     * @param MessageBuffer A TPCANMsgFD structure buffer to store the CAN
     * message
     * @param TimestampBuffer A TPCANTimestampFD buffer to get the reception
     * time of the message
     * @return A TPCANStatus error code
     */
    public native TPCANStatus ReadFD(
            TPCANHandle Channel,
            TPCANMsgFD MessageBuffer,
            TPCANTimestampFD TimestampBuffer);
    
    /**
     * Transmits a CAN message
     *
     * @param Channel The handle of a PCAN Channel
     * @param MessageBuffer A TPCANMsg buffer with the message to be sent
     * @return A TPCANStatus error code
     */
    public native TPCANStatus Write(
            TPCANHandle Channel,
            TPCANMsg MessageBuffer);
    
    /**
     * Transmits a CAN message over a FD capable PCAN Channel
     *
     * @param Channel The handle of a FD capable PCAN Channel
     * @param MessageBuffer A TPCANMsgFD buffer with the message to be sent
     * @return A TPCANStatus error code
     */
    public native TPCANStatus WriteFD(
            TPCANHandle Channel,
            TPCANMsgFD MessageBuffer);
    
    /**
     * Configures the reception filter. The message filter will be expanded with
     * every call to this function. If it is desired to reset the filter, please
     * use the CAN_SetParameter function
     *
     * @param Channel The handle of a PCAN Channel
     * @param FromID The lowest CAN ID to be received
     * @param ToID The highest CAN ID to be received
     * @param Mode Message type, Standard (11-bit identifier) or Extended
     * (29-bit identifier)
     * @return A TPCANStatus error code
     */
    public native TPCANStatus FilterMessages(
            TPCANHandle Channel,
            int FromID,
            int ToID,
            TPCANMode Mode);
    
    /**
     * Retrieves a PCAN Channel value Parameters can be present or not according
     * with the kind of Hardware (PCAN Channel) being used. If a parameter is
     * not available, a PCAN_ERROR_ILLPARAMTYPE error will be returned
     *
     * @param Channel The handle of a PCAN Channel
     * @param Parameter The TPCANParameter parameter to get
     * @param Buffer Buffer for the parameter value
     * @param BufferLength Size in bytes of the buffer
     * @return A TPCANStatus error code
     */
    public native TPCANStatus GetValue(
            TPCANHandle Channel,
            TPCANParameter Parameter,
            Object Buffer,
            int BufferLength);
    
    /**
     * Configures or sets a PCAN Channel value Parameters can be present or not
     * according with the kind of Hardware (PCAN Channel) being used. If a
     * parameter is not available, a PCAN_ERROR_ILLPARAMTYPE error will be
     * returned
     *
     * @param Channel The handle of a PCAN Channel
     * @param Parameter The TPCANParameter parameter to get
     * @param Buffer Buffer for the parameter value
     * @param BufferLength Size in bytes of the buffer
     * @return A TPCANStatus error code
     */
    public native TPCANStatus SetValue(
            TPCANHandle Channel,
            TPCANParameter Parameter,
            Object Buffer,
            int BufferLength);

    /**
     * Returns a descriptive text of a given TPCANStatus error code, in any
     * desired language The current languages available for translation are:
     * Neutral (0x00), German (0x07), English (0x09), Spanish (0x0A), Italian
     * (0x10) and French (0x0C)
     *
     * @param Error A TPCANStatus error code
     * @param Language Indicates a 'Primary language ID'
     * @param Buffer Buffer for a null terminated char array
     * @return A TPCANStatus error code
     */
    public native TPCANStatus GetErrorText(
            TPCANStatus Error,
            short Language,
            StringBuffer Buffer);

    /**
     * Finds a PCAN-Basic channel that matches with the given parameters
     *
     * @param Parameters A comma separated string contained pairs of parameter-name/value to be matched within a PCAN-Basic channel
     * @param FoundChannel Buffer for returning the PCAN-Basic channel
     * @param Buffer Buffer for a null terminated char array
     * @return A TPCANStatus error code
     */
    public native TPCANStatus LookUpChannel(
    		StringBuffer Parameters, 
    	    MutableTPCANHandle FoundChannel);

    /**
     * Sets the handle of the Receive-Event for the Channel. static method
     * peak.can.basic.RcvEventDispatcher.dispatchRcvEvent is used to notify each
     * Receive-Event
     *
     * @param Channel The handle of a PCAN Channel
     * @return A TPCANStatus error code
     */
    public native TPCANStatus SetRcvEvent(TPCANHandle Channel);

    /**
     * Resets the handle of the Receive-Event for the Channel.
     *
     * @param Channel The handle of a PCAN Channel
     * @return A TPCANStatus error code
     */
    public native TPCANStatus ResetRcvEvent(TPCANHandle Channel);

    /**
     * Initializes the PCANBasic API
     *
     * @return a boolean to indicate if API is successfully loaded
     */
    public native boolean initializeAPI();

    static {
        try {
            System.loadLibrary("pcanbasic_jni");
        } catch (UnsatisfiedLinkError e) {
            System.out.println(e.getMessage());
            System.loadLibrary("PCANBasic_JNI");
        }
    }
}
