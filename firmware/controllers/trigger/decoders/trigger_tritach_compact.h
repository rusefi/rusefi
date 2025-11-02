/*
 * trigger_tritach_compact.h
 *
 * Memory-efficient Audi TriTach trigger configuration.
 * 
 * Audi TriTach uses 135 teeth on the flywheel with a reference marker,
 * implemented as a 135-1 skipped tooth pattern to save memory.
 *
 * Add to: firmware/controllers/trigger/decoders/
 */

#pragma once

#include "trigger_structure.h"

// Configure TriTach trigger using memory-efficient skipped tooth approach
void configureTriTachCompact(TriggerWaveform *s);
