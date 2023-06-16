#pragma once

#if EFI_PROD_CODE

extern ioportid_t criticalErrorLedPort;
extern ioportmask_t criticalErrorLedPin;
extern uint8_t criticalErrorLedState;

#define TURN_FATAL_LED() palWritePad(criticalErrorLedPort, criticalErrorLedPin, criticalErrorLedState);

#endif /* EFI_PROD_CODE */
