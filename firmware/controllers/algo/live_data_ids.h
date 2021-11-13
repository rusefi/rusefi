/**
 * @file live_data_ids.h
 *
 * Live Documentation Structure
 * these indexes are used by custom protocol command TS_GET_STRUCT
 */

#pragma once

typedef enum {
LDS_SPEED_DENSITY,
LDS_ENGINE,
LDS_FUEL_TRIM,
LDS_TPS_TPS_ENRICHMENT,
LDS_TRIGGER_CENTRAL,
LDS_ETB_PID,
LDS_IDLE_PID,
//LDS_ALTERNATOR_PID,
//LDS_CJ125_PID,
LDS_TRIGGER_STATE,
LDS_AC_CONTROL,
LDS_IDLE,
LDS_TPS_ACCEL
} live_data_e;
