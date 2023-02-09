/**
 * @file    uart.c
 * @brief   UART Function for nrf5340 HIC
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

#include "IO_Config.h"
#include "string.h"
#include "nrfx_config.h"
#include "nrfx_uarte.h"
#include "uart.h"
#include "util.h"
#include "cortex_m.h"
#include "circ_buf.h"
#include "settings.h" // for config_get_overflow_detect

static void clear_buffers(void);

#define BUFFER_SIZE         (4096)

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];
uint8_t rx_byte;

static void clear_buffers(void) {}

static void resume_tx(void) {}

static void event_handler(nrfx_uarte_event_t const * p_event, void *p_context) {}

extern int32_t uart_initialize(void) { return 0; }
extern int32_t uart_uninitialize(void) { return 0; }
extern int32_t uart_reset(void) { return 0; }
extern int32_t uart_set_configuration(UART_Configuration *config){ return 0; }
extern int32_t uart_get_configuration(UART_Configuration *config){ return 0; }
extern int32_t uart_write_free(void){ return 0; }
extern int32_t uart_write_data(uint8_t *data, uint16_t size){ return 0; }
extern int32_t uart_read_data(uint8_t *data, uint16_t size){ return 0; }
extern void uart_set_control_line_state(uint16_t ctrl_bmp) {}
extern void uart_software_flow_control(void) {}
extern void uart_enable_flow_control(bool enabled) {}
