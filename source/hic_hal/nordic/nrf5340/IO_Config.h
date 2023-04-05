/**
 * @file    IO_Config.h
 * @brief   IO Configuration for nrf52820 HIC
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2021, Arm Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dl_nrf_gpio.h"

// Override all defines if IO_CONFIG_OVERRIDE is defined
#ifdef IO_CONFIG_OVERRIDE
#include "IO_Config_Override.h"
#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__
#endif
#endif

#ifndef __IO_CONFIG_H__
#define __IO_CONFIG_H__

#include "device.h"
#include "compiler.h"
#include "daplink.h"
#include "nrf_gpio.h"

#define GPIO_LED_ACTIVE_STATE 1

#define JLINK_SWD1


#ifdef JLINK_SWD0

#define PIN_SWCLK         NRF_GPIO_PIN_MAP(1, 12)  // SWDCLK (Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(1, 14)  // SWDIO (Input/Output)
#define PIN_SWSWO         NRF_GPIO_PIN_MAP(1, 11)   // SWO
#define PIN_nRESET        NRF_GPIO_PIN_MAP(0, 31)   // nRESET Pin
// pin to override the target select switch on Nordic boards
#define PIN_SWD_CTRL      NRF_GPIO_PIN_MAP(0, 30)

#endif

#ifdef JLINK_SWD1

#define PIN_SWCLK         NRF_GPIO_PIN_MAP(0, 25)  // SWDCLK (Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(0, 27)  // SWDIO (Input/Output)
#define PIN_SWSWO         NRF_GPIO_PIN_MAP(1, 9)   // SWO
#define PIN_nRESET        NRF_GPIO_PIN_MAP(1, 7)   // nRESET Pin
// pin to override the target select switch on Nordic boards
#define PIN_SWD_CTRL      NRF_GPIO_PIN_MAP(1, 5)

#endif

#ifdef JLINK_SWD3

#define PIN_SWCLK         NRF_GPIO_PIN_MAP(0, 1)  // SWDCLK (Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(0, 0)  // SWDIO (Input/Output)
#define PIN_SWSWO         NRF_GPIO_PIN_MAP(0, 2)   // SWO
#define PIN_nRESET        NRF_GPIO_PIN_MAP(0, 3)   // nRESET Pin

#endif


#undef  LED_RUNNING                                // Target Running LED (Output)
#undef  LED_CONNECTED                              // Connected LED (Output)
#define LED_HID           NRF_GPIO_PIN_MAP(0, 14)
#define LED_MSC           NRF_GPIO_PIN_MAP(0, 15)
#define LED_CDC           NRF_GPIO_PIN_MAP(0, 26)
#undef  LED_PWR
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(0, 24) // SW1
#define UART_TX_PIN       NRF_GPIO_PIN_MAP(0, 29)  // UART From IMCU to target
#define UART_RX_PIN       NRF_GPIO_PIN_MAP(1, 4)  // UART From target to IMCU


#endif
