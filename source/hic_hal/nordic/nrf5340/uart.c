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

static void clear_buffers(void)
{
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));
}

static nrfx_uarte_t dev = NRFX_UARTE_INSTANCE(0);

static nrfx_uarte_config_t conf =  NRFX_UARTE_DEFAULT_CONFIG(UART_TX_PIN, UART_RX_PIN);

static void resume_tx(void)
{
	if (!nrfx_uarte_tx_in_progress(&dev)) {
		uint32_t len = 0;
		const uint8_t *data = circ_buf_peek(&write_buffer, &len);
		nrfx_uarte_tx(&dev, data, len);
	}
}

static void event_handler(nrfx_uarte_event_t const * p_event, void *p_context)
{
	int ret;


	if (p_event->type == NRFX_UARTE_EVT_TX_DONE)
	{
		resume_tx();
	}
	if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
	{
		circ_buf_write(&read_buffer, &rx_byte, p_event->data.rxtx.bytes);
		nrfx_uarte_rx(&dev, &rx_byte, 1);
	}
	if (p_event->type == NRFX_UARTE_EVT_ERROR)
	{
	}
}

extern int32_t uart_initialize(void)
{
	clear_buffers();
	nrfx_uarte_init(&dev, &conf, event_handler);
	nrfx_uarte_rx(&dev, &rx_byte, 1);

	return 0;
}
extern int32_t uart_uninitialize(void)
{
	nrfx_uarte_uninit(&dev);

	return 0;
}
extern int32_t uart_reset(void)
{
	uart_uninitialize();
	uart_initialize();
	return 0;
}
extern int32_t uart_set_configuration(UART_Configuration *config)
{
	/* there is no databits setting */
	if (config->DataBits != UART_DATA_BITS_8) {
		return -1;
	}

	switch (config->Parity) {
		case UART_PARITY_EVEN:
		conf.hal_cfg.parity = NRF_UARTE_PARITY_INCLUDED;
#if defined(UART_CONFIG_PARITYTYPE_Msk)
		conf.hal_cfg.paritytype = NRF_UARTE_PARITYTYPE_EVEN;
#endif
		break;

		case UART_PARITY_ODD:
		conf.hal_cfg.parity =  NRF_UARTE_PARITY_INCLUDED;
#if defined(UART_CONFIG_PARITYTYPE_Msk)
		conf.hal_cfg.paritytype = NRF_UARTE_PARITYTYPE_ODD;
#endif
		break;

		case UART_PARITY_NONE: /* fallthrough */
		default:
		conf.hal_cfg.parity =  NRF_UARTE_PARITY_EXCLUDED;
		break;
	}

	switch (config->StopBits) {
		case UART_STOP_BITS_1: /* fallthrough */
		conf.hal_cfg.stop = NRF_UARTE_STOP_ONE;
		break;

		case UART_STOP_BITS_2:
		conf.hal_cfg.stop = NRF_UARTE_STOP_TWO;
		break;

		default:
		return -1;
	}

	if (config->FlowControl != UART_FLOW_CONTROL_NONE) {
		return -1;
	}

	conf.baudrate = NRF_UARTE_BAUDRATE_115200;

	switch (config->Baudrate) {
	case 1200:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud1200;
	break;
	case 2400:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud2400;
	break;
	case 4800:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud4800;
	break;
	case 9600:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud9600;
	break;
	case 14400:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud14400;
	break;
	case 19200:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud19200;
	break;
	case 28800:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud28800;
	break;
	case 31250:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud31250;
	break;
	case 38400:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud38400;
	break;
	case 56000:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud56000;
	break;
	case 57600:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud57600;
	break;
	case 76800:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud76800;
	break;
	case 115200:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud115200;
	break;
	case 230400:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud230400;
	break;
	case 250000:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud250000;
	break;
	case 460800:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud460800;
	break;
	case 921600:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud921600;
	break;
	case 1000000:
	conf.baudrate = UARTE_BAUDRATE_BAUDRATE_Baud1M;
	break;
	default:
	return -1;
	}

	uart_reset();

	return 0;
}
extern int32_t uart_get_configuration(UART_Configuration *config)
{
	/* this is not even used - waste of time */
	return 1;
}
extern int32_t uart_write_free(void)
{
	return circ_buf_count_free(&write_buffer);
}
extern int32_t uart_write_data(uint8_t *data, uint16_t size)
{
	if (size == 0) {
		return 0;
	}
	uint32_t cnt = circ_buf_write(&write_buffer, data, size);

	resume_tx();
}
extern int32_t uart_read_data(uint8_t *data, uint16_t size)
{
	return circ_buf_read(&read_buffer, data, size);
}
extern void uart_set_control_line_state(uint16_t ctrl_bmp) {}
extern void uart_software_flow_control(void) {}
extern void uart_enable_flow_control(bool enabled) {}
