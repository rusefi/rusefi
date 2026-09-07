#include "pcan_helper.h"
#include <windows.h>
#include <stdio.h>

typedef TPCANStatus (__stdcall *CAN_GetStatus_t)(TPCANHandle Channel);
typedef TPCANStatus (__stdcall *CAN_GetValue_t)(TPCANHandle Channel, TPCANParameter Parameter, void* Buffer, DWORD BufferLength);

static CAN_GetStatus_t pcanGetStatus = NULL;
static CAN_GetValue_t pcanGetValue = NULL;

void checkPcanDevice(void) {
    // Try to load the DLL from the specified location
    // The issue description says: relevant DLL is C:\stuff\rusefi\java_console\PCANBasic.dll
    // Relative to simulator executable (usually in simulator/build or simulator/),
    // java_console is a sibling of simulator.

    HMODULE hModule = LoadLibraryA("PCANBasic.dll");
    if (!hModule) {
        // Try relative path if not in search path
        hModule = LoadLibraryA("../java_console/PCANBasic.dll");
    }

    if (!hModule) {
        printf("PCAN: PCANBasic.dll not found. PCAN detection skipped.\n");
        return;
    }

    pcanGetStatus = (CAN_GetStatus_t)GetProcAddress(hModule, "CAN_GetStatus");
    pcanGetValue = (CAN_GetValue_t)GetProcAddress(hModule, "CAN_GetValue");

    if (!pcanGetStatus || !pcanGetValue) {
        printf("PCAN: Failed to find required functions in PCANBasic.dll\n");
        FreeLibrary(hModule);
        return;
    }

    // List of common PCAN channels to check
    TPCANHandle channels[] = {
        PCAN_USBBUS1, PCAN_USBBUS2,
        PCAN_PCIBUS1, PCAN_PCIBUS2,
        PCAN_LANBUS1
    };
    const char* names[] = {
        "PCAN_USBBUS1", "PCAN_USBBUS2",
        "PCAN_PCIBUS1", "PCAN_PCIBUS2",
        "PCAN_LANBUS1"
    };

    bool found = false;
    for (int i = 0; i < sizeof(channels) / sizeof(channels[0]); i++) {
        unsigned int condition = 0;
        TPCANStatus status = pcanGetValue(channels[i], PCAN_CHANNEL_CONDITION, &condition, sizeof(condition));

        if (status == PCAN_ERROR_OK) {
            if (condition & PCAN_CHANNEL_AVAILABLE) {
                printf("PCAN: Device %s is AVAILABLE\n", names[i]);
                found = true;
            } else if (condition & PCAN_CHANNEL_OCCUPIED) {
                printf("PCAN: Device %s is OCCUPIED\n", names[i]);
                found = true;
            }
        }
    }

    if (!found) {
        printf("PCAN: No PCAN devices found.\n");
    }

    FreeLibrary(hModule);
}
