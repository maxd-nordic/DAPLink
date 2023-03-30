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

#include "dl_nrf_gpio.h"

// This GPIO configuration is only valid for the nrf52840 HIC
COMPILER_ASSERT(DAPLINK_HIC_ID == DAPLINK_HIC_ID_NRF52840);

#if defined(SEEEDSTUDIO_XIAO_BLE)

#define GPIO_LED_ACTIVE_STATE 0
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP
#define PIN_SWCLK         NRF_GPIO_PIN_MAP(1, 13) // SWDCLK (Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(1, 14) // SWDIO (Input/Output)
#define PIN_nRESET        NRF_GPIO_PIN_MAP(0, 15) // nRESET Pin
#undef LED_RUNNING                                // Target Running LED (Output)
#undef LED_CONNECTED                              // Connected LED (Output)
#define LED_HID           NRF_GPIO_PIN_MAP(0, 26) // HID LED
#define LED_MSC           NRF_GPIO_PIN_MAP(0, 30) // MSC LED
#define LED_CDC           NRF_GPIO_PIN_MAP(0, 6)  // CDC LED
#undef LED_PWR                                    // POWER LED
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(0, 3)  // Reset button (SW_RESET)
#define UART_TX_PIN       NRF_GPIO_PIN_MAP(1, 11) // UART TX (From IMCU to target)
#define UART_RX_PIN       NRF_GPIO_PIN_MAP(1, 12) // UART RX (From target to IMCU)
#define I2C_SCL_PIN       NRF_GPIO_PIN_MAP(0, 5)  // I2C SCL
#define I2C_SDA_PIN       NRF_GPIO_PIN_MAP(0, 4)  // I2C SDA

#elif defined(SSCI_ISP1807)

#define GPIO_LED_ACTIVE_STATE 0
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP
#define PIN_SWCLK         NRF_GPIO_PIN_MAP(0, 26) // SWD CLK
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(0, 14) // SWD IO
#define PIN_nRESET        NRF_GPIO_PIN_MAP(0, 25) // nRESET
#undef LED_RUNNING                                // Target Running LED
#undef LED_CONNECTED                              // Connected LED
#define LED_HID           NRF_GPIO_PIN_MAP(0, 6)  // USB HID LED
#define LED_MSC           NRF_GPIO_PIN_MAP(0, 6)  // USB MSC LED
#define LED_CDC           NRF_GPIO_PIN_MAP(0, 6)  // USB CDC LED
#undef LED_PWR                                    // Power LED
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(1, 6)  // SW_RESET
#define UART_TX_PIN       NRF_GPIO_PIN_MAP(0, 13) // UART TX
#define UART_RX_PIN       NRF_GPIO_PIN_MAP(0, 17) // UART RX
#define I2C_SCL_PIN       NRF_GPIO_PIN_MAP(0, 15) // I2C SCL
#define I2C_SDA_PIN       NRF_GPIO_PIN_MAP(0, 8)  // I2C SDA

#elif defined(NRF52840_DK)

#define GPIO_LED_ACTIVE_STATE 0
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP
#define PIN_SWCLK         NRF_GPIO_PIN_MAP(0, 17) // SWDCLK (Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(0, 19) // SWDIO (Input/Output)
#define PIN_nRESET        NRF_GPIO_PIN_MAP(1, 9)  // nRESET Pin
#undef LED_RUNNING                                // Target Running LED (Output)
#undef LED_CONNECTED                              // Connected LED (Output)
#define LED_HID           NRF_GPIO_PIN_MAP(0, 13) // HID LED (LED1)
#define LED_MSC           NRF_GPIO_PIN_MAP(0, 14) // MSC LED (LED2)
#define LED_CDC           NRF_GPIO_PIN_MAP(0, 15) // CDC LED (LED3)
#define LED_PWR           NRF_GPIO_PIN_MAP(0, 16) // POWER LED (LED4)
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(0, 11) // Reset button (Button 1)
#define UART_TX_PIN       NRF_GPIO_PIN_MAP(0, 21) // UART TX
#define UART_RX_PIN       NRF_GPIO_PIN_MAP(0, 23) // UART RX
#define I2C_SCL_PIN       NRF_GPIO_PIN_MAP(0, 20) // I2C
#define I2C_SDA_PIN       NRF_GPIO_PIN_MAP(0, 22) // I2C

#elif defined(THINGY91)

#define GPIO_LED_ACTIVE_STATE 0
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP
#define PIN_SWD_CTRL      NRF_GPIO_PIN_MAP(0, 17)
#define PIN_SWCLK         NRF_GPIO_PIN_MAP(1, 05)  // SWDCLK (Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(0, 14)  // SWDIO (Input/Output)
#undef PIN_nRESET
#undef LED_RUNNING
#undef LED_CONNECTED
#define LED_HID           NRF_GPIO_PIN_MAP(0, 6)
#define LED_MSC LED_HID
#define LED_CDC LED_HID
#undef LED_PWR
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(1, 13)  // SW1
#define UART_TX_PIN       NRF_GPIO_PIN_MAP(0, 15)  // UART From IMCU to target
#define UART_RX_PIN       NRF_GPIO_PIN_MAP(0, 11)  // UART From target to IMCU

#else

#define GPIO_LED_ACTIVE_STATE 0
#define RESET_BUTTON_PULL NRF_GPIO_PIN_PULLUP
#define PIN_SWCLK         NRF_GPIO_PIN_MAP(0, 13) // SWDCLK (Output)
#define PIN_SWDIO         NRF_GPIO_PIN_MAP(0, 17) // SWDIO (Input/Output)
#define PIN_nRESET        NRF_GPIO_PIN_MAP(0, 9)  // nRESET Pin
#undef LED_RUNNING                                // Target Running LED
#undef LED_CONNECTED                              // Connected LED
#define LED_HID           NRF_GPIO_PIN_MAP(0, 6)  // HID LED (LED1 Green)
#define LED_MSC           NRF_GPIO_PIN_MAP(0, 8)  // MSC LED (LED2 Red)
#define LED_CDC           NRF_GPIO_PIN_MAP(0, 12) // CDC LED (LED2 Blue)
#undef LED_PWR                                    // POWER LED
#define RESET_BUTTON      NRF_GPIO_PIN_MAP(1, 6)  // Reset button (SW1)
#define UART_TX_PIN       NRF_GPIO_PIN_MAP(0, 24) // UART TX
#define UART_RX_PIN       NRF_GPIO_PIN_MAP(1, 0)  // UART RX
#define I2C_SCL_PIN       NRF_GPIO_PIN_MAP(1, 15) // I2C
#define I2C_SDA_PIN       NRF_GPIO_PIN_MAP(0, 2)  // I2C

#endif

#endif