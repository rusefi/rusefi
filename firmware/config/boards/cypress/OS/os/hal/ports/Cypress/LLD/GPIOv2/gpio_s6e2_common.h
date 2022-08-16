/*
    ChibiOS - Copyright (C) 2014-2015 Fabio Utzig

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
 * @file    GPIOv2/gpio_s6e2_common.h
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup PAL
 * @{
 */

#pragma once

// copied from s6e2c5xl.h
#ifndef bFM4_GPIO_PDIR3_P0
#define bFM4_GPIO_PDIR3_P0                        *((volatile  uint8_t *)(0x42DE6180UL))
#endif

#ifndef bFM4_GPIO_PDOR3_P0
#define bFM4_GPIO_PDOR3_P0                        *((volatile  uint8_t *)(0x42DE8180UL))
#endif

#ifndef bFM4_GPIO_PDIR5_P0
#define bFM4_GPIO_PDIR5_P0                        *((volatile  uint8_t *)(0x42DE6280UL))
#endif

#ifndef bFM4_GPIO_PDOR5_P0
#define bFM4_GPIO_PDOR5_P0                        *((volatile  uint8_t *)(0x42DE8280UL))
#endif

#ifndef bFM4_GPIO_PDIR9_P0
#define bFM4_GPIO_PDIR9_P0                        *((volatile  uint8_t *)(0x42DE6480UL))
#endif

#ifndef bFM4_GPIO_PDOR9_P0
#define bFM4_GPIO_PDOR9_P0                        *((volatile  uint8_t *)(0x42DE8480UL))
#endif

#ifndef bFM4_GPIO_PDIRB_P0
#define bFM4_GPIO_PDIRB_P0                        *((volatile  uint8_t *)(0x42DE6580UL))
#endif

#ifndef bFM4_GPIO_PDORB_P0
#define bFM4_GPIO_PDORB_P0                        *((volatile  uint8_t *)(0x42DE8580UL))
#endif

#ifndef bFM4_GPIO_PDIRF_P0
#define bFM4_GPIO_PDIRF_P0                        *((volatile  uint8_t *)(0x42DE6780UL))
#endif

#ifndef bFM4_GPIO_PDORF_P0
#define bFM4_GPIO_PDORF_P0                        *((volatile  uint8_t *)(0x42DE8780UL))
#endif
