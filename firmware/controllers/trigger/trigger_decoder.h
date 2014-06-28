/**
 * @file	trigger_decoder.h
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TRIGGER_DECODER_H_
#define TRIGGER_DECODER_H_

#include <time.h>

#include "trigger_structure.h"
#include "engine_configuration.h"
#include "ec2.h"

#ifdef __cplusplus
extern "C"
{
#endif

int isTriggerDecoderError(void);
void initializeSkippedToothTriggerShapeExt(engine_configuration2_s *engineConfiguration2, int totalTeethCount, int skippedCount);
void initializeTriggerShape(Logging *logger, engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2);
int findTriggerZeroEventIndex(trigger_shape_s * shape, trigger_config_s const*triggerConfig);

void initTriggerDecoder(void);

#ifdef __cplusplus
}
#endif
#endif /* TRIGGER_DECODER_H_ */
