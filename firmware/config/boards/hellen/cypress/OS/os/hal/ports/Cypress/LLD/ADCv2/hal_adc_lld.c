/*
    ChibiOS - Copyright (C) 2014 Derek Mulcahy

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    ADCv1/hal_adc_lld.c
 * @brief   Cypress S6E2Cx ADC subsystem low level driver source.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup ADC
 * @{
 */

#include <string.h>
#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief ADC1 driver identifier.*/
#if CYPRESS_ADC_USE_ADC0 || defined(__DOXYGEN__)
ADCDriver ADCD1;
#endif

/** @brief ADC2 driver identifier.*/
#if CYPRESS_ADC_USE_ADC1 || defined(__DOXYGEN__)
ADCDriver ADCD2;
#endif

/** @brief ADC3 driver identifier.*/
#if CYPRESS_ADC_USE_ADC2 || defined(__DOXYGEN__)
ADCDriver ADCD3;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

//!!!!!!!!!!
#define BOARD_MOD1_PORT GPIOD
#define BOARD_MOD1_PIN 5

static void adc_result_callback(ADCDriver *adcp, volatile uint32_t* pu32AdcArgument) {

	// if the interrupt is 'late' (called after the conversion is aborted)
	if (adcp->grpp == NULL) {
		Adc_ClrScanFifo(adcp->adc);
		return;
	}

	int i = adcp->fifoSize;
	
	chSysLockFromISR();
	while (i-- > 0) {
		uint32_t u32AdcResult = Adc_ReadScanFifo(adcp->adc); // *pu32AdcArgument;
		if (u32AdcResult == 0xFFFFFFFF || Adc_GetScanDataValid(adcp->adc, u32AdcResult) != AdcFifoDataValid)
			continue;
	
		uint16_t u16AdcValue = Adc_GetScanData(adcp->adc, u32AdcResult);
		// todo: we don't know the exact order of the HW channels - if it's always ascending, we could use more efficient LUT
		uint8_t hwChannelIdx = Adc_GetScanChannel(adcp->adc, u32AdcResult);
		int logicChannelIdx = adcp->channelLogicIndices[hwChannelIdx];
		int currentStartIndex = (adcp->current_index / adcp->grpp->num_channels) * adcp->grpp->num_channels;

		/* Read the sample into the buffer */
		adcp->samples[currentStartIndex + logicChannelIdx] = u16AdcValue;
		adcp->current_index++;
	}
	chSysUnlockFromISR();

	bool more = true;

	/*  At the end of the buffer then we may be finished */
	if (adcp->current_index >= adcp->number_of_samples) {
    	/* We are never finished in circular mode */
		more = adcp->grpp->circular;

    	_adc_isr_full_code(adcp);

    	adcp->current_index = 0;
	}

	if (more) {
	    /* Signal half completion in circular mode. */
    	if (adcp->grpp->circular && (adcp->current_index == (adcp->number_of_samples / 2))) {
        	_adc_isr_half_code(adcp);
    	}	
    	//if ((adcp->current_index % adcp->grpp->num_channels) == (adcp->grpp->num_channels - 1)) {
    		Adc_SwTriggerScan(adcp->adc);
    		//break;
    	//}
  	}
}

static void adc_error_callback(ADCDriver *adcp) {
	adcp->current_index = 0;
	Adc_ClrScanFifo(adcp->adc);
	_adc_isr_error_code(adcp, ADC_ERR_OVERFLOW);
	if (adcp->grpp->circular)
		Adc_SwTriggerScan(adcp->adc);
}

/////////////////////////////////////////////////////////////////////////

static void adc_result_callback1(volatile uint32_t* pu32AdcArgument) {
	adc_result_callback(&ADCD1, pu32AdcArgument);
}

static void adc_result_callback2(volatile uint32_t* pu32AdcArgument) {
	adc_result_callback(&ADCD2, pu32AdcArgument);
}

static void adc_result_callback3(volatile uint32_t* pu32AdcArgument) {
	adc_result_callback(&ADCD3, pu32AdcArgument);
}

static void adc_error_callback1(void) {
	adc_error_callback(&ADCD1);
}

static void adc_error_callback2(void) {
	adc_error_callback(&ADCD2);
}

static void adc_error_callback3(void) {
	adc_error_callback(&ADCD3);
}

/**
 * @brief   ADC interrupt handler.
 *
 * @isr
 */
static bool initAdc(ADCDriver *adcp) {
	int chn;
    PDL_ZERO_STRUCT(adcp->stcAdcConfig);      // Clear local configuration to zero.
    PDL_ZERO_STRUCT(adcp->stcScanCfg);

    if (adcp->grpp->num_channels < 1) {
    	// we need to return otherwise we never get an interrupt, and the whole process halts
    	// todo: is it enough to say the HAL driver to abort?
    	adcp->state = ADC_STOP;
    	return false;
    }

    adcp->stcScanCfg.u32ScanCannelSelect.u32AD_CHn = 0;
    adcp->stcAdcConfig.u32SamplingTimeSelect.u32AD_CHn = 0;
	for (chn = 0; chn < adcp->grpp->num_channels; chn++) {
		adcp->stcScanCfg.u32ScanCannelSelect.u32AD_CHn |= 1u << adcp->channelHwIndices[chn];
		//adcp->stcAdcConfig.u32SamplingTimeSelect.u32AD_CHn |= 1u << adcp->channelHwIndices[chn];
	}

	adcp->fifoSize = (adcp->grpp->num_channels < CYPRESS_ADC_FIFO_SIZE) ? adcp->grpp->num_channels : CYPRESS_ADC_FIFO_SIZE;
	
    adcp->stcScanCfg.enScanMode = ScanSingleConversion; // ScanRepeatConversion
    adcp->stcScanCfg.enScanTimerTrigger = AdcNoTimer;
    adcp->stcScanCfg.bScanTimerStartEnable = FALSE;
    adcp->stcScanCfg.u8ScanFifoDepth = adcp->fifoSize - 1;

    adcp->stcAdcConfig.bLsbAlignment = TRUE;
    adcp->stcAdcConfig.enSamplingTimeN0 = Value8; // Value32
    adcp->stcAdcConfig.u8SamplingTime0 = 8u; // 30u
    adcp->stcAdcConfig.enSamplingTimeN1 = Value8; // Value32
    adcp->stcAdcConfig.u8SamplingTime1 = 8u;//30u;
    adcp->stcAdcConfig.u8ComparingClockDiv = 3u;     // Frequency division ratio: 5, 0:Ratio 2, 1:Ratio 3, ...
    adcp->stcAdcConfig.pstcScanInit = &adcp->stcScanCfg;
    adcp->stcAdcConfig.u8EnableTime = 10u;//0x80;

    memset(&adcp->stcIrqEn, 0, sizeof(adcp->stcIrqEn));
    adcp->stcIrqEn.bScanIrq = TRUE;
    adcp->stcIrqEn.bFifoOverrunIrq = TRUE;
    
	adcp->stcAdcConfig.pstcIrqEn = &adcp->stcIrqEn;
	adcp->stcAdcConfig.pstcIrqCb = &adcp->stcIrqCb;

    adcp->stcAdcConfig.bTouchNvic = TRUE;
    
    if (Adc_Init((volatile stc_adcn_t*)adcp->adc, &adcp->stcAdcConfig) == Ok) {    
        Adc_EnableWaitReady(adcp->adc);
        return true;
    }
    return false;
}

static void adc_init_structures(ADCDriver *adcp) {
	adcObjectInit(adcp);
    memset(&adcp->stcIrqCb, 0, sizeof(adcp->stcIrqCb));
	memset(adcp->oldChannelHwIndices, 0, sizeof(adcp->oldChannelHwIndices));
	ADCD1.oldNumChannels = 0;
	ADCD1.oldDepth = 0;
	adcp->wasInit = false;
}


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ADC driver initialization.
 *
 * @notapi
 */
void adc_lld_init(void) {

#if CYPRESS_ADC_USE_ADC0
	/* Driver initialization.*/
    adc_init_structures(&ADCD1);
	ADCD1.adc = (stc_adcn_t *)&ADC0;
	ADCD1.stcIrqCb.pfnScanIrqCb = &adc_result_callback1;
	ADCD1.stcIrqCb.pfnScanErrIrqCb = &adc_error_callback1;
#endif

#if CYPRESS_ADC_USE_ADC1
	/* Driver initialization.*/
    adc_init_structures(&ADCD2);
	ADCD2.adc = (stc_adcn_t *)&ADC1;
	ADCD2.stcIrqCb.pfnScanIrqCb = &adc_result_callback2;
	ADCD2.stcIrqCb.pfnScanErrIrqCb = &adc_error_callback2;
#endif

#if CYPRESS_ADC_USE_ADC2
	/* Driver initialization.*/
    adc_init_structures(&ADCD3);
	ADCD3.adc = (stc_adcn_t *)&ADC2;
	ADCD3.stcIrqCb.pfnScanIrqCb = &adc_result_callback3;
	ADCD3.stcIrqCb.pfnScanErrIrqCb = &adc_error_callback3;
#endif

}

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start(ADCDriver *adcp) {

	/* If in stopped state then enables the ADC clock.*/
	if (adcp->state == ADC_STOP) {
	}
}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop(ADCDriver *adcp) {

	/* If in ready state then disables the ADC clock.*/
	if (adcp->state == ADC_READY) {
	}
}

/**
 * @brief   Starts an ADC conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start_conversion(ADCDriver *adcp) {
	const ADCConversionGroup *grpp = adcp->grpp;

	int chn;
	adcp->channelHwIndices[0] = ADCx_SC1n_ADCH_DISABLED;
	for (chn = 0; chn < grpp->num_channels; chn++) {
		if (chn < 6) {
			adcp->channelHwIndices[chn] = (grpp->sqr3 >> (chn * 5)) & 31;
		} else if (chn < 12) {
			adcp->channelHwIndices[chn] = (grpp->sqr2 >> ((chn - 6) * 5)) & 31;
		} else if (chn < 18) {	// actually, it's 16
			adcp->channelHwIndices[chn] = (grpp->sqr1 >> ((chn - 12) * 5)) & 31;
		} else if (chn < 24) {
			adcp->channelHwIndices[chn] = (grpp->sqr4 >> ((chn - 18) * 5)) & 31;
		} else if (chn < 30) {
			adcp->channelHwIndices[chn] = (grpp->sqr5 >> ((chn - 24) * 5)) & 31;
		}
		// we need to restore the correct order of the channels in the IRQ handler
		adcp->channelLogicIndices[adcp->channelHwIndices[chn]] = chn;
	}

	adcp->number_of_samples = adcp->depth * grpp->num_channels;
	adcp->current_index = 0;

	bool isChanged = true;
	// check if the config is changed
	if (grpp->num_channels == adcp->oldNumChannels && adcp->depth == adcp->oldDepth) {
		// we can store bitmask and compare but we cannot be sure about the order of the channels.
		if (memcmp(adcp->channelHwIndices, adcp->oldChannelHwIndices, sizeof(adcp->channelHwIndices[0]) * grpp->num_channels) == 0) {
			isChanged = false;
		}
	}
	// config has changed, we need to re-initialize the ADC driver
	if (isChanged) {
		if (adcp->wasInit) {
			Adc_DeInit(adcp->adc, TRUE);
			adcp->wasInit = false;
		}
		// store new config
		adcp->oldNumChannels = grpp->num_channels;
		adcp->oldDepth = adcp->depth;
		memcpy(adcp->oldChannelHwIndices, adcp->channelHwIndices, sizeof(adcp->channelHwIndices[0]) * grpp->num_channels);
	}
	
	// late init - only when we know the channel configuration
	if (!adcp->wasInit) {
		adcp->wasInit = true;
		initAdc(adcp);
	} else {
		//Adc_EnableWaitReady(adcp->adc);
	}
	//Adc_EnableIrq(adcp->adc, &adcp->stcIrqEn);
	//Adc_StartScanRepeat(adcp->adc);
	Adc_SwTriggerScan(adcp->adc);
	
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {
	// we stop the repeative scan and wait for the next adc_lld_start_conversion() call
  	//Adc_StopScanRepeat(adcp->adc);
  	// abort pending interrupt
  	//Adc_ClrIrqFlag(adcp->adc, AdcScanIrq);
  	//Adc_DisableIrq(adcp->adc, &adcp->stcIrqEn);
  	//Adc_Disable(adcp->adc);

	//Adc_DeInit(adcp->adc, TRUE);
}

#endif /* HAL_USE_ADC */

/** @} */
