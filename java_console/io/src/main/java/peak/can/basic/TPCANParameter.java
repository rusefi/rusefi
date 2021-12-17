/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * $Id: TPCANParameter.java 7391 2020-08-10 08:32:30Z Fabrice $
 * @LastChange $Date: 2020-08-10 10:32:30 +0200 (Mon, 10 Aug 2020) $
 * 
 * PCANBasic JAVA Interface.
 *
 * Copyright (C) 2001-2020  PEAK System-Technik GmbH <www.peak-system.com>
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

import java.util.ArrayDeque;
import java.util.Deque;

/**
 * Parameter definition.
 */
public enum TPCANParameter {

	/**
	 * PCAN-USB device number parameter
	 * 
	 * @deprecated Deprecated parameter. Use PCAN_DEVICE_ID instead
	 */
	PCAN_DEVICE_NUMBER(0x01),
	/**
	 * PCAN-USB device number parameter
	 */
	PCAN_DEVICE_ID(0x01),
	/**
	 * PCAN-PC Card 5-Volt power parameter
	 */
	PCAN_5VOLTS_POWER(0x02),
	/**
	 * PCAN receive event handler parameter
	 */
	PCAN_RECEIVE_EVENT(0x03),
	/**
	 * PCAN message filter parameter
	 */
	PCAN_MESSAGE_FILTER(0x04),
	/**
	 * PCAN-Basic API version parameter
	 */
	PCAN_API_VERSION(0x05),
	/**
	 * PCAN device channel version parameter
	 */
	PCAN_CHANNEL_VERSION(0x06),
	/**
	 * PCAN Reset-On-Busoff parameter
	 */
	PCAN_BUSOFF_AUTORESET(0x07),
	/**
	 * PCAN Listen-Only parameter
	 */
	PCAN_LISTEN_ONLY(0x08),
	/**
	 * Directory path for trace files
	 */
	PCAN_LOG_LOCATION(0x09),
	/**
	 * Debug-Trace activation status
	 */
	PCAN_LOG_STATUS(0x0A),
	/**
	 * Configuration of the debugged information (LOG_FUNCTION_***)
	 */
	PCAN_LOG_CONFIGURE(0x0B),
	/**
	 * Custom insertion of text into the log file
	 */
	PCAN_LOG_TEXT(0x0C),
	/**
	 * Availability status of a PCAN-Channel
	 */
	PCAN_CHANNEL_CONDITION(0x0D),
	/**
	 * PCAN hardware name parameter
	 */
	PCAN_HARDWARE_NAME(0x0E),
	/**
	 * Message reception status of a PCAN-Channel
	 */
	PCAN_RECEIVE_STATUS(0x0F),
	/**
	 * CAN-Controller number of a PCAN-Channel
	 */
	PCAN_CONTROLLER_NUMBER(0x10),
	/**
	 * Directory path for PCAN trace files
	 */
	PCAN_TRACE_LOCATION(0x11),
	/**
	 * CAN tracing activation status
	 */
	PCAN_TRACE_STATUS(0x12),
	/**
	 * Configuration of the maximum file size of a CAN trace
	 */
	PCAN_TRACE_SIZE(0x13),
	/**
	 * Configuration of the trace file storing mode (TRACE_FILE_***)
	 */
	PCAN_TRACE_CONFIGURE(0x14),
	/**
	 * Physical identification of a USB based PCAN-Channel by blinking its
	 * associated LED
	 */
	PCAN_CHANNEL_IDENTIFYING(0x15),
	/**
	 * Capabilities of a PCAN device (FEATURE_***)
	 */
	PCAN_CHANNEL_FEATURES(0x16),
	/**
	 * Using of an existing bit rate (PCAN-View connected to a channel)
	 */
	PCAN_BITRATE_ADAPTING(0x17),
	/**
	 * Configured Bit rate as Btr0Btr1 value
	 */
	PCAN_BITRATE_INFO(0x18),
	/**
	 * Configured Bit rate as TPCANBitrateFD string
	 */
	PCAN_BITRATE_INFO_FD(0x19),
	/**
	 * Configured nominal CAN Bus speed as Bits per seconds
	 */
	PCAN_BUSSPEED_NOMINAL(0x1A),
	/**
	 * Configured CAN data speed as Bits per seconds
	 */
	PCAN_BUSSPEED_DATA(0x1B),
	/**
	 * Remote address of a LAN channel as string in IPv4 format
	 */
	PCAN_IP_ADDRESS(0x1C),
	/**
	 * Status of the Virtual PCAN-Gateway Service
	 */
	PCAN_LAN_SERVICE_STATUS(0x1D),
	/**
	 * Status messages reception status within a PCAN-Channel
	 */
	PCAN_ALLOW_STATUS_FRAMES(0x1E),
	/**
	 * RTR messages reception status within a PCAN-Channel
	 */
	PCAN_ALLOW_RTR_FRAMES(0x1F),
	/**
	 * Error messages reception status within a PCAN-Channel
	 */
	PCAN_ALLOW_ERROR_FRAMES(0x20),
	/**
	 * Delay, in microseconds, between sending frames
	 */
	PCAN_INTERFRAME_DELAY(0x21),
	/**
	 * Filter over code and mask patterns for 11-Bit messages
	 */
	PCAN_ACCEPTANCE_FILTER_11BIT(0x22),
	/**
	 * Filter over code and mask patterns for 29-Bit messages
	 */
	PCAN_ACCEPTANCE_FILTER_29BIT(0x23),
	/**
	 * Output mode of 32 digital I/O pin of a PCAN-USB Chip. 1: Output-Active 0 :
	 * Output Inactive
	 */
	PCAN_IO_DIGITAL_CONFIGURATION(0x24),
	/**
	 * Value assigned to a 32 digital I/O pins of a PCAN-USB Chip
	 */
	PCAN_IO_DIGITAL_VALUE(0x25),
	/**
	 * Value assigned to a 32 digital I/O pins of a PCAN-USB Chip - Multiple digital
	 * I/O pins to 1 = High
	 */
	PCAN_IO_DIGITAL_SET(0x26),
	/**
	 * Clear multiple digital I/O pins to 0
	 */
	PCAN_IO_DIGITAL_CLEAR(0x27),
	/**
	 * Get value of a single analog input pin
	 */
	PCAN_IO_ANALOG_VALUE(0x28),
	/**
	 * Get the version of the firmware used by the device associated with a
	 * PCAN-Channel
	 */
	PCAN_FIRMWARE_VERSION(0x29),
	/**
	 * Get the amount of PCAN channels attached to a system
	 */
	PCAN_ATTACHED_CHANNELS_COUNT(0x2A),
	/**
	 * Get information about PCAN channels attached to a system
	 */
	PCAN_ATTACHED_CHANNELS(0x2B);

	private TPCANParameter(int value) {
		this.value = value;
	}

	/**
	 * The identifier of the CAN parameter
	 * 
	 * @return Identifier of the CAN parameter
	 */
	public int getValue() {
		return this.value;
	}

	/**
	 * Returns the list of PCAN Parameters which are customizable as array
	 * 
	 * @return the list of PCAN Parameters which are customizable as array
	 */
	public static TPCANParameter[] customizableParameters() {
		Deque<TPCANParameter> result = new ArrayDeque<TPCANParameter>();
		result.add(TPCANParameter.PCAN_DEVICE_ID);
		result.add(TPCANParameter.PCAN_5VOLTS_POWER);
		result.add(TPCANParameter.PCAN_API_VERSION);
		result.add(TPCANParameter.PCAN_CHANNEL_VERSION);
		result.add(TPCANParameter.PCAN_BUSOFF_AUTORESET);
		result.add(TPCANParameter.PCAN_LISTEN_ONLY);
		result.add(TPCANParameter.PCAN_LOG_LOCATION);
		result.add(TPCANParameter.PCAN_LOG_STATUS);
		result.add(TPCANParameter.PCAN_LOG_CONFIGURE);
		result.add(TPCANParameter.PCAN_CHANNEL_CONDITION);
		result.add(TPCANParameter.PCAN_HARDWARE_NAME);
		result.add(TPCANParameter.PCAN_RECEIVE_STATUS);
		result.add(TPCANParameter.PCAN_CONTROLLER_NUMBER);
		result.add(TPCANParameter.PCAN_TRACE_LOCATION);
		result.add(TPCANParameter.PCAN_TRACE_STATUS);
		result.add(TPCANParameter.PCAN_TRACE_SIZE);
		result.add(TPCANParameter.PCAN_TRACE_CONFIGURE);
		result.add(TPCANParameter.PCAN_CHANNEL_IDENTIFYING);
		result.add(TPCANParameter.PCAN_CHANNEL_FEATURES);
		result.add(TPCANParameter.PCAN_BITRATE_ADAPTING);
		result.add(TPCANParameter.PCAN_BITRATE_INFO);
		result.add(TPCANParameter.PCAN_BITRATE_INFO_FD);
		result.add(TPCANParameter.PCAN_BUSSPEED_NOMINAL);
		result.add(TPCANParameter.PCAN_BUSSPEED_DATA);
		result.add(TPCANParameter.PCAN_IP_ADDRESS);

		result.add(TPCANParameter.PCAN_LAN_SERVICE_STATUS);
		result.add(TPCANParameter.PCAN_ALLOW_STATUS_FRAMES);
		result.add(TPCANParameter.PCAN_ALLOW_RTR_FRAMES);
		result.add(TPCANParameter.PCAN_ALLOW_ERROR_FRAMES);
		result.add(TPCANParameter.PCAN_INTERFRAME_DELAY);
		result.add(TPCANParameter.PCAN_ACCEPTANCE_FILTER_11BIT);
		result.add(TPCANParameter.PCAN_ACCEPTANCE_FILTER_29BIT);
		result.add(TPCANParameter.PCAN_IO_DIGITAL_CONFIGURATION);
		result.add(TPCANParameter.PCAN_IO_DIGITAL_VALUE);
		result.add(TPCANParameter.PCAN_IO_DIGITAL_SET);
		result.add(TPCANParameter.PCAN_IO_DIGITAL_CLEAR);
		result.add(TPCANParameter.PCAN_IO_ANALOG_VALUE);
		result.add(TPCANParameter.PCAN_FIRMWARE_VERSION);
		result.add(TPCANParameter.PCAN_ATTACHED_CHANNELS_COUNT);
		result.add(TPCANParameter.PCAN_ATTACHED_CHANNELS);

		return result.toArray(new TPCANParameter[result.size()]);
	}

	/**
	 * Returns a description for the given TPCANParameter
	 * 
	 * @param param TPCANParameter for which we need description
	 * @return a string description for the given TPCANParameter
	 */
	public static String getParameterDescription(TPCANParameter param) {
		String description = "";
		switch (param) {
		case PCAN_DEVICE_NUMBER:
		case PCAN_DEVICE_ID:
			description = "This parameter is used on PCAN-USB hardware to distinguish between 2 (or more) of them on the same computer. This value is persistent, i.e. the identifier will not be lost after disconnecting and connecting again the hardware.";
			break;
		case PCAN_5VOLTS_POWER:
			description = "This parameter is used on PCAN-PC Card hardware for switching the external 5V on the D-Sub connector of the PC Card. This is useful when connecting external bus converter modules to the card (AU5790 / TJA1954)).";
			break;
		case PCAN_RECEIVE_EVENT:
			description = "This parameter allows the user to easily configure the message filter of a PCAN channel. With it is possible to fully open or complete close the filter.";
			break;
		case PCAN_MESSAGE_FILTER:
			description = "This parameter is used to let the PCAN driver notify an application when CAN messages are placed in its receive queue.";
			break;
		case PCAN_API_VERSION:
			description = "This parameter is used to get information about the PCAN-Basic API implementation version.";
			break;
		case PCAN_CHANNEL_VERSION:
			description = "This parameter is used to get version information about the Driver of a PCAN Channel.";
			break;
		case PCAN_BUSOFF_AUTORESET:
			description = "This parameter instructs the PCAN driver to reset automatically the CAN controller of a PCAN channel when a bus-off state is detected. Since no communication is possible on a bus-off state, it is useful to let the driver to catch this event automatically and reset the controller, avoiding extra handling of this problem in an end application.";
			break;
		case PCAN_LISTEN_ONLY:
			description = "This parameter allows the user to set a CAN hardware in Listen-Only mode. When this mode is set, the CAN controller doens't take part on active events (eg. transmit CAN messages) but stays in a passive mode (CAN monitor), in which it can analyse the traffic on the CAN bus used by a PCAN channel. See also the Philips Data Sheet \"SJA1000 Stand-alone CAN controller\".";
			break;
		case PCAN_LOG_LOCATION:
			description = "This value is used to set the folder location on a computer for the Log-File generated by the PCAN-Basic API, within a debug session. Setting this value starts recording debug information automatically. If a debug session is running (a log file is being written), PCAN_LOG_LOCATION instructs the API to close the current log file and to start the process again with the new folder information. Note that the name of the log file cannot be specified, this name is fixed as PCANBasic.log.";
			break;
		case PCAN_LOG_STATUS:
			description = "This value is used to control the activity status of a debug session within the PCAN-Basic API. If the log status is set to ON without having set a location for the log file or without having configured the information to be traced, then the session process will start with the default values.";
			break;
		case PCAN_LOG_CONFIGURE:
			description = "This value is used to configure the debug information to be included in the log file generated in a debug session within the PCAN-Basic API.";
			break;
		case PCAN_LOG_TEXT:
			description = "This Parameter is used to insert custom information in the log file generated in a debug session within the PCAN-Basic API.";
			break;
		case PCAN_CHANNEL_CONDITION:
			description = "This parameter is used to check and detect available PCAN hardware on a computer, even before trying to connect any of them. This is useful when an application wants the user to select which hardware should be using in a communication session.";
			break;
		case PCAN_HARDWARE_NAME:
			description = "This parameter is to read the Name of the Hardware.";
			break;
		case PCAN_RECEIVE_STATUS:
			description = "This Parameter is to get the reception status of a PCAN-Channel.";
			break;
		case PCAN_CONTROLLER_NUMBER:
			description = "This Parameter is to get CAN-Controller number of a PCAN-Channel. Only usefull with 2 Channel HW.";
			break;
		case PCAN_TRACE_LOCATION:
			description = "This Parameter is used to configure the Trace File Directory.";
			break;
		case PCAN_TRACE_STATUS:
			description = "This Parameter is used to control the activity of the Tracer status.";
			break;
		case PCAN_TRACE_SIZE:
			description = "This Parameter is used to configure the maximum file size of a CAN trace.";
			break;
		case PCAN_TRACE_CONFIGURE:
			description = "This Parameter is used to configure the trace file storing mode (TRACE_FILE_***).";
			break;
		case PCAN_CHANNEL_IDENTIFYING:
			description = "This Parameter is used to Physically identify a USB based PCAN-Channel by blinking its associated LED.";
			break;
		case PCAN_CHANNEL_FEATURES:
			description = "This Parameter is used to get the capabilities of a PCAN device.";
			break;
		case PCAN_BITRATE_ADAPTING:
			description = "This Parameter is used to force an initialization process to succeed, even if the PCAN-Channel is being used by a PCAN-View with a different or unknown bit rate.";
			break;
		case PCAN_BITRATE_INFO:
			description = "This Parameter is used to read the currently configured communication speed of a PCAN Channel connected as standard CAN.";
			break;
		case PCAN_BITRATE_INFO_FD:
			description = "This Parameter is used to read the currently configured communication speed, as a parameterized string value (FD Bit rate String), of a PCAN Channel connected as CAN FD.";
			break;
		case PCAN_BUSSPEED_NOMINAL:
			description = "This Parameter is used to read the configured nominal CAN Bus speed as Bits per seconds.";
			break;
		case PCAN_BUSSPEED_DATA:
			description = "This Parameter is used to read configured CAN data speed as Bits per seconds.";
			break;
		case PCAN_IP_ADDRESS:
			description = "This Parameter is used to read the remote address of a LAN channel as string in IPv4 format.";
			break;
		case PCAN_LAN_SERVICE_STATUS:
			description = "This Parameter is used to read the status of the Virtual PCAN-Gateway Service.";
			break;
		case PCAN_ALLOW_STATUS_FRAMES:
			description = "This Parameter is used to configure the reception of status messages within a PCAN-Channel.";
			break;
		case PCAN_ALLOW_RTR_FRAMES:
			description = "This Parameter is used to configure the reception of RTR messages within a PCAN-Channel.";
			break;
		case PCAN_ALLOW_ERROR_FRAMES:
			description = "This Parameter is used to configure the reception of Error messages within a PCAN-Channel.";
			break;
		case PCAN_INTERFRAME_DELAY:
			description = "This Parameter is used to configure the delay, in microseconds, between sending frames.";
			break;
		case PCAN_ACCEPTANCE_FILTER_11BIT:
			description = "This Parameter is used to configure the filter over code and mask patterns for 11-Bit messages.";
			break;
		case PCAN_ACCEPTANCE_FILTER_29BIT:
			description = "This Parameter is used to configure the filter over code and mask patterns for 29-Bit messages.";
			break;
		case PCAN_IO_DIGITAL_CONFIGURATION:
			description = "This Parameter is used to configure the output mode of 32 digital I/O pin of a PCAN-USB Chip (1: Output Active, 0: Output Inactive).";
			break;
		case PCAN_IO_DIGITAL_VALUE:
			description = "This Parameter is used to configure the value assigned to a 32 digital I/O pins of a PCAN-USB Chip.";
			break;
		case PCAN_IO_DIGITAL_SET:
			description = "This Parameter is used to configure the value assigned to a 32 digital I/O pins of a PCAN-USB Chip (Multiple digital I/O pins to 1 = High).";
			break;
		case PCAN_IO_DIGITAL_CLEAR:
			description = "This Parameter is used to clear multiple digital I/O pins to 0.";
			break;
		case PCAN_IO_ANALOG_VALUE:
			description = "This Parameter is used to read the value of a single analog input pin.";
			break;
		case PCAN_FIRMWARE_VERSION:
			description = "This Parameter is used to read the version of the firmware used by the device associated with a PCAN-Channel.";
			break;
		case PCAN_ATTACHED_CHANNELS_COUNT:
			description = "This Parameter is used to read the amount of PCAN channels attached to a system.";
			break;
		case PCAN_ATTACHED_CHANNELS:
			description = "This Parameter is used to read information about PCAN channels attached to a system.";
			break;
		default:
			break;
		}
		return description;
	}

	private final int value;
}
