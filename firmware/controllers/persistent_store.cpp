/**
 * @file    persistent_store.cpp
 * @brief   Controllers package entry point code
 *
 *
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "pch.h"

#if !EFI_UNIT_TEST
#include "sensor_chart.h"
#include "trigger_central.h"

#ifndef PERSISTENT_LOCATION
#define PERSISTENT_LOCATION CCM_OPTIONAL
#else
/* nothing */
#endif

// Magic from https://stackoverflow.com/questions/1562074/how-do-i-show-the-value-of-a-define-at-compile-time
/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)
#pragma message(VAR_NAME_VALUE(PERSISTENT_LOCATION))

persistent_config_container_s persistentState PERSISTENT_LOCATION;

#else // EFI_UNIT_TEST

persistent_config_s * config;
engine_configuration_s * engineConfiguration;

#endif /* EFI_UNIT_TEST */
