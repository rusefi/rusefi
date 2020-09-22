/*******************************************************************************
* Copyright (C) 2013-2016, Cypress Semiconductor Corporation or a              *
* subsidiary of Cypress Semiconductor Corporation.  All rights reserved.       *
*                                                                              *
* This software, including source code, documentation and related              *
* materials ("Software"), is owned by Cypress Semiconductor Corporation or     *
* one of its subsidiaries ("Cypress") and is protected by and subject to       *
* worldwide patent protection (United States and foreign), United States       *
* copyright laws and international treaty provisions. Therefore, you may use   *
* this Software only as provided in the license agreement accompanying the     *
* software package from which you obtained this Software ("EULA").             *
*                                                                              *
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,     *
* non-transferable license to copy, modify, and compile the                    *
* Software source code solely for use in connection with Cypress's             *
* integrated circuit products.  Any reproduction, modification, translation,   *
* compilation, or representation of this Software except as specified          *
* above is prohibited without the express written permission of Cypress.       *
*                                                                              *
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                         *
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                         *
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                 *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                              *
* PARTICULAR PURPOSE. Cypress reserves the right to make                       *
* changes to the Software without notice. Cypress does not assume any          *
* liability arising out of the application or use of the Software or any       *
* product or circuit described in the Software. Cypress does not               *
* authorize its products for use in any products where a malfunction or        *
* failure of the Cypress product may reasonably be expected to result in       *
* significant property damage, injury or death ("High Risk Product"). By       *
* including Cypress's product in a High Risk Product, the manufacturer         *
* of such system or application assumes all risk of such use and in doing      *
* so agrees to indemnify Cypress against all liability.                        *
*******************************************************************************/
/******************************************************************************/
/** \file UsbDeviceHidKeyboard.c
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidKeyboardGroup USB Device Keyboard Module description @endlink
 **
 ** History:
 **   - 2013-05-08  1.0  MSc  First Version
 **   - 2013-10-14  1.1  MSc  PDL support added
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "UsbDeviceHidKeyboard.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDKEYBOARD_ENABLED == ON))
/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

static boolean_t SendData(uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode);

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/



/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
const uint8_t u8AsciiToKeyCode[] = {
0, // 0 = 
0,  // 1 = 
0, // 2 = 
0, // 3 = 
0, // 4 = 
0, // 5 = 
0, // 6 = 
0, // 7 = 
0, // 8 = 
0, // 9 = 	
0x28, // 10 = \r
0, // 11 = 
0, // 12 = 
0x58, // 13 = \n
0, // 14 = 
0, // 15 = 
0, // 16 = 
0, // 17 = 
0, // 18 = 
0, // 19 = 
0, // 20 = 
0, // 21 = 
0, // 22 = 
0, // 23 = 
0, // 24 = 
0, // 25 = 
0, // 26 = 
29, // 27 = ESC
0, // 28 = 
0, // 29 = 
0, // 30 = 
0, // 31 = 
44, // 32 = SPACE
0, // 33 = !
0, // 34 = "
0, // 35 = #
0, // 36 = $
0, // 37 = %
0, // 38 = &
0, // 39 = '
0, // 40 = (
0, // 41 = )
0, // 42 = *
0, // 43 = +
0, // 44 = ,
0, // 45 = -
0x37, // 46 = .
0, // 47 = /
39, // 48 = 0
30, // 49 = 1
31, // 50 = 2
32, // 51 = 3
33, // 52 = 4
34, // 53 = 5
35, // 54 = 6
36, // 55 = 7
37, // 56 = 8
38, // 57 = 9
0, // 58 = :
0, // 59 = ;
0, // 60 = <
0, // 61 = =
0, // 62 = >
0, // 63 = ?
0, // 64 = @
0, // 65 = A
0, // 66 = B
0, // 67 = C
0, // 68 = D
0, // 69 = E
0, // 70 = F
0, // 71 = G
0, // 72 = H
0, // 73 = I
0, // 74 = J
0, // 75 = K
0, // 76 = L
0, // 77 = M
0, // 78 = N
0, // 79 = O
0, // 80 = P
0, // 81 = Q
0, // 82 = R
0, // 83 = S
0, // 84 = T
0, // 85 = U
0, // 86 = V
0, // 87 = W
0, // 88 = X
0, // 89 = Y
0, // 90 = Z
0, // 91 = [
0, // 92 = 
0, // 93 = ]
0, // 94 = ^
0, // 95 = _
0, // 96 = `
4, // 97 = a
5, // 98 = b
6, // 99 = c
7, // 100 = d
8, // 101 = e
9, // 102 = f
10, // 103 = g
11, // 104 = h
12, // 105 = i
13, // 106 = j
14, // 107 = k
15, // 108 = l
16, // 109 = m
17, // 110 = n
18, // 111 = o
19, // 112 = p
20, // 113 = q
21, // 114 = r
22, // 115 = s
23, // 116 = t
24, // 117 = u
25, // 118 = v
26, // 119 = w
27, // 120 = x
28, // 121 = y
29, // 122 = z
0, // 123 = {
0, // 124 = |
0, // 125 = }
0, // 126 = ~
0, // 127 = 
0, // 128 = ?
0, // 129 = ?
0, // 130 = ?
0, // 131 = ?
0, // 132 = ?
0, // 133 = ?
0, // 134 = ?
0, // 135 = ?
0, // 136 = ?
0, // 137 = ?
0, // 138 = ?
0, // 139 = ?
0, // 140 = ?
0, // 141 = ?
0, // 142 = ?
0, // 143 = ?
0, // 144 = ?
0, // 145 = ?
0, // 146 = ?
0, // 147 = ?
0, // 148 = ?
0, // 149 = ?
0, // 150 = ?
0, // 151 = ?
0, // 152 = ?
0, // 153 = ?
0, // 154 = ?
0, // 155 = ?
0, // 156 = ?
0, // 157 = ?
0, // 158 = ?
0, // 159 = ?
0, // 160 = ?
0, // 161 = ?
0, // 162 = ?
0, // 163 = ?
0, // 164 = ?
0, // 165 = ?
0, // 166 = ?
0, // 167 = ?
0, // 168 = ?
0, // 169 = ?
0, // 170 = ?
0, // 171 = ?
0, // 172 = ?
0, // 173 = ?
0, // 174 = ?
0, // 175 = ?
0, // 176 = ?
0, // 177 = ?
0, // 178 = ?
0, // 179 = ?
0, // 180 = ?
0, // 181 = ?
0, // 182 = ?
0, // 183 = ?
0, // 184 = ?
0, // 185 = ?
0, // 186 = ?
0, // 187 = ?
0, // 188 = ?
0, // 189 = ?
0, // 190 = ?
0, // 191 = ?
0, // 192 = ?
0, // 193 = ?
0, // 194 = ?
0, // 195 = ?
0, // 196 = ?
0, // 197 = ?
0, // 198 = ?
0, // 199 = ?
0, // 200 = ?
0, // 201 = ?
0, // 202 = ?
0, // 203 = ?
0, // 204 = ?
0, // 205 = ?
0, // 206 = ?
0, // 207 = ?
0, // 208 = ?
0, // 209 = ?
0, // 210 = ?
0, // 211 = ?
0, // 212 = ?
0, // 213 = ?
0, // 214 = ?
0, // 215 = ?
0, // 216 = ?
0, // 217 = ?
0, // 218 = ?
0, // 219 = ?
0, // 220 = ?
0, // 221 = ?
0, // 222 = ?
0, // 223 = ?
0, // 224 = ?
0, // 225 = ?
0, // 226 = ?
0, // 227 = ?
0, // 228 = ?
0, // 229 = ?
0, // 230 = ?
0, // 231 = ?
0, // 232 = ?
0, // 233 = ?
0, // 234 = ?
0, // 235 = ?
0, // 236 = ?
0, // 237 = ?
0, // 238 = ?
0, // 239 = ?
0, // 240 = ?
0, // 241 = ?
0, // 242 = ?
0, // 243 = ?
0, // 244 = ?
0, // 245 = ?
0, // 246 = ?
0, // 247 = ?
0, // 248 = ?
0, // 249 = ?
0, // 250 = ?
0, // 251 = ?
0, // 252 = ?
0, // 253 = ?
0, // 254 = ?
0, // 255 = ?
};
static uint8_t DataSentFlags = 0; //Status for sent data
static uint8_t pu8KeyboardData[8] = {0,0,0,0,0,0,0,0};
static stc_usbn_endpoint_data_t* pstcEndpointIN;
static stc_usbn_endpoint_buffer_t stcEndpointBufferIN;
static stc_usbn_t* pstcUsbHandle = NULL;
static boolean_t bReady = FALSE;
/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/**
 ******************************************************************************
 ** \brief Configuration change callback, used to see if configuration is set
 **        or cleared
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \return none     
 **
 ******************************************************************************/
static void ConfCallback(stc_usbn_t* pstcUsb)
{
    if (UsbDevice_GetStatus(pstcUsb) == UsbConfigured)
    {
        bReady = TRUE;
    } else
    {
        bReady = FALSE;
    }
}

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

void UsbDeviceHidKeyboard_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;
    
    stcUsbClassConfig.u8InterfaceNumber          = 0xFF;                          // do not use fix interface number, choose by class, subclass, protocol
    stcUsbClassConfig.u8InterfaceClass           = 0x03;                          // HID Class
    stcUsbClassConfig.u8InterfaceSubClass        = 0x01;                          // Custom Sub Class
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x01;                          // Custom Protocol
    stcUsbClassConfig.pfnSetupRequestCallback    = NULL;                          // setup requests handled
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                          // No connection callback handled
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                          // No disconnection callback handled
    stcUsbClassConfig.pfnConfCallback            = ConfCallback;                  // Callback for configuration set
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);
        
    for(u8i = 0;u8i < u8InterfaceLength;)
    {
        u8i += pu8Interface[u8i];
        if (USBDESCR_ENDPOINT == pu8Interface[u8i + 1])
        {
            stcEndpointConfig.u8EndpointAddress = pu8Interface[u8i + 2];
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) == 0)
            {
                /* NOT USED */
            }
            else
            {
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferIN;
                pstcEndpointIN = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
            }
            u8NumEndpoints++;
        }
    }
    
}

/**
 ******************************************************************************
 ** Is called from main application to send data via endpoint
 **
 ** \param pu8Buffer Buffer to send
 ** \param u32DataSize Buffersize
 ** \param enMode    polled sending, interrupt sending or DMA
 **
 ** \return 1: if succesful, 0: if usb was not ready
 *****************************************************************************/
static boolean_t SendData(uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    if (UsbDevice_GetStatus(pstcUsbHandle) == UsbConfigured)
    {
        DataSentFlags &= ~(1<<1);
		
	    UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, pu8Buffer, u32DataSize, enMode);
        if (enMode == UsbPOLL)
        {
            DataSentFlags |= (1<<1);
        }
        return TRUE;
    }
    return FALSE;
}


/**
 ******************************************************************************
 ** Set modifier keys
 **
 ** \param u8ModifierKey Bitmask of modifier keys:
 **                      MODIFIERKEY_LEFT_CTRL
 **                      MODIFIERKEY_LEFT_SHIFT
 **                      MODIFIERKEY_LEFT_ALT
 **                      MODIFIERKEY_LEFT_GUI
 **                      MODIFIERKEY_RIGHT_CTRL
 **                      MODIFIERKEY_RIGHT_SHIFT
 **                      MODIFIERKEY_RIGHT_ALT
 **                      MODIFIERKEY_RIGHT_GUI
 **
 ** \return none
 *****************************************************************************/
void UsbDeviceHidKeyboard_SetModifierKey(uint8_t u8ModifierKey)
{
    pu8KeyboardData[0] |= u8ModifierKey;
    SendData((uint8_t*)pu8KeyboardData,8,UsbPOLL);
}
/**
 ******************************************************************************
 ** Clear modifier keys
 **
 ** \param u8ModifierKey Bitmask of modifier keys:
 **                      MODIFIERKEY_LEFT_CTRL
 **                      MODIFIERKEY_LEFT_SHIFT
 **                      MODIFIERKEY_LEFT_ALT
 **                      MODIFIERKEY_LEFT_GUI
 **                      MODIFIERKEY_RIGHT_CTRL
 **                      MODIFIERKEY_RIGHT_SHIFT
 **                      MODIFIERKEY_RIGHT_ALT
 **                      MODIFIERKEY_RIGHT_GUI
 **
 ** \return none
 *****************************************************************************/
void UsbDeviceHidKeyboard_ClearModifierKey(uint8_t u8ModifierKey)
{
    pu8KeyboardData[0] &= ~u8ModifierKey;
    SendData((uint8_t*)pu8KeyboardData,8,UsbPOLL);
}

/**
 ******************************************************************************
 ** Press key (Down - Up)
 **
 ** \param u8Index       0..5 (parallel pressed keys)
 ** \param u8KeyCode     Keycode  
 **
 ** \return none
 *****************************************************************************/
void UsbDeviceHidKeyboard_KeyPress(uint8_t u8Index, uint8_t u8KeyCode)
{
    pu8KeyboardData[u8Index + 2] = u8KeyCode;
    SendData((uint8_t*)pu8KeyboardData,8,UsbPOLL);
    pu8KeyboardData[u8Index + 2] = 0;
    SendData((uint8_t*)pu8KeyboardData,8,UsbPOLL);
}

/**
 ******************************************************************************
 ** Press key (Down)
 **
 ** \param u8Index       0..5 (parallel pressed keys)
 ** \param u8KeyCode     Keycode  
 **
 ** \return none
 *****************************************************************************/
void UsbDeviceHidKeyboard_KeyDown(uint8_t u8Index, uint8_t u8KeyCode)
{
    pu8KeyboardData[u8Index + 2] = u8KeyCode;
    SendData((uint8_t*)pu8KeyboardData,8,UsbPOLL);
}

/**
 ******************************************************************************
 ** Press key (Up)
 **
 ** \param u8Index       0..5 (parallel pressed keys)
 ** \param u8KeyCode     Keycode  
 **
 ** \return none
 *****************************************************************************/
void UsbDeviceHidKeyboard_KeyUp(uint8_t u8Index, uint8_t u8KeyCode)
{
    pu8KeyboardData[u8Index + 2] = u8KeyCode;
    SendData((uint8_t*)pu8KeyboardData,8,UsbPOLL);
}


/**
 ******************************************************************************
 ** Return connected state
 **
 ** \return TRUE if ready
 **
 *****************************************************************************/
boolean_t UsbDeviceHidKeyboard_IsConnected(void)
{
    return bReady;
}
#endif
