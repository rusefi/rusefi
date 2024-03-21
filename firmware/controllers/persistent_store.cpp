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

#ifndef PERSISTENT_LOCATION_TODO
#define PERSISTENT_LOCATION CCM_OPTIONAL
#else
#pragma message(PERSISTENT_LOCATION_VALUE)
//#define PERSISTENT_LOCATION CCM_OPTIONAL
#define PERSISTENT_LOCATION __attribute__((section(".ram1")))
#endif

persistent_config_container_s persistentState PERSISTENT_LOCATION;

#else // EFI_UNIT_TEST

persistent_config_s * config;
engine_configuration_s * engineConfiguration;

#endif /* EFI_UNIT_TEST */
