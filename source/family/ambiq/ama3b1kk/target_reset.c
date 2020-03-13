/**
 * @file    target_reset.c
 * @brief   Target reset for the new target
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

//#include "target_reset.h"
#include "swd_host.h"
#include "target_family.h"
#include "DAP_config.h"
#include "util.h"
#include "DAP.h"
#include "debug_cm.h"
#include "cmsis_os2.h"

#define NVIC_Addr (0xe000e000)
#define DBG_Addr (0xe000edf0)

#define MCUCTRL_SCRATCH0 0x400401B0

static void target_before_init_debug(void)
{
    // any target specific sequences needed before attaching
    //	to the DAP across JTAG or SWD
    return;
}

static uint8_t target_unlock_sequence(void)
{
    // if the device can secure the flash and there is a way to
    //	erase all it should be implemented here.
    return 1;
}

static uint8_t target_set_state(target_state_t state)
{
    // if a custom state machine is needed to set the TARGET_RESET_STATE state
    return 1;
}

static uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    // if there are security bits in the programmable flash region
    //	a check should be performed. This method is used when programming
    //	by drag-n-drop and should refuse to program an image requesting
    //	to set the device security. This can be performed with the debug channel
    //	if needed.
    return 0;
}

// static void swd_set_target_reset_ama3b1kk(uint8_t asserted)
// {
//     uint32_t scratch0;
//     if (asserted)
//     {
//         //Set POWER->RESET on NRF to 1
//         if (!swd_write_ap(AP_TAR, MCUCTRL_SCRATCH0))
//         {
//             util_assert(0);
//             return;
//         }

//         if (!(swd_read_ap(AP_TAR, &scratch0)))
//         {
//             util_assert(0);
//             return;
//         }

//         if (!swd_write_ap(AP_DRW, MCUCTRL_SCRATCH0))
//         {
//             util_assert(0);
//             return;
//         }

//         if (!swd_write_ap(AP_DRW, scratch0 | 0x01))
//         {
//             util_assert(0);
//             return;
//         }

//         //PIN_nRESET_OUT(1);
//     }
//     else
//     {
//         //PIN_nRESET_OUT(0);
//     }
// }

uint8_t swd_set_state_ama3b1kk(target_state_t state)
{
    uint32_t val, scratch0;
    swd_init();
    switch (state)
    {
    case RESET_HOLD:
        swd_set_target_reset(1);
        break;

    case RESET_RUN:
        swd_set_target_reset(1);
        osDelay(2);
        swd_set_target_reset(0);
        osDelay(2);
        swd_off();
        break;

    case RESET_PROGRAM:
        if (!swd_init_debug())
        {
            return 0;
        }

        // Enable debug and halt the core (DHCSR <- 0xA05F0003)
        if (!swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN | C_HALT))
        {
            return 0;
        }

        if (!swd_write_ap(AP_TAR, MCUCTRL_SCRATCH0))
        {
            util_assert(0);
            return 0;
        }

        if (!(swd_read_ap(AP_TAR, &scratch0)))
        {
            util_assert(0);
            return 0;
        }

        if (!swd_write_ap(AP_DRW, MCUCTRL_SCRATCH0))
        {
            util_assert(0);
            return 0;
        }

        if (!swd_write_ap(AP_DRW, scratch0 | 0x01))
        {
            util_assert(0);
            return 0;
        }
        //util_assert(0);
        // Wait until core is halted
        do
        {
            if (!swd_read_word(DBG_HCSR, &val))
            {
                return 0;
            }
        } while ((val & S_HALT) == 0);

        // Enable halt on reset
        if (!swd_write_word(DBG_EMCR, VC_CORERESET))
        {
            return 0;
        }

        // Perform a soft reset
        if (!swd_write_word(NVIC_AIRCR, VECTKEY | SYSRESETREQ))
        {
            return 0;
        }

        break;

    case NO_DEBUG:
        if (!swd_write_word(DBG_HCSR, DBGKEY))
        {
            return 0;
        }

        break;

    case DEBUG:
        if (!JTAG2SWD())
        {
            return 0;
        }

        if (!swd_write_dp(DP_ABORT, STKCMPCLR | STKERRCLR | WDERRCLR | ORUNERRCLR))
        {
            return 0;
        }

        // Ensure CTRL/STAT register selected in DPBANKSEL
        if (!swd_write_dp(DP_SELECT, 0))
        {
            return 0;
        }

        // Power up
        if (!swd_write_dp(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ))
        {
            return 0;
        }

        // Enable debug
        if (!swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN))
        {
            return 0;
        }

        break;

    default:
        return 0;
    }

    return 1;
}

const target_family_descriptor_t _g_target_family = {
    .family_id = kAmbiq_ama3b1kk_FamilyID,
    //.default_reset_type = kHardwareReset,
    .default_reset_type = kSoftwareReset,
    // .soft_reset_type = SYSRESETREQ,
    .target_before_init_debug = target_before_init_debug,
    .target_unlock_sequence = target_unlock_sequence,
    .target_set_state = target_set_state,
    .security_bits_set = security_bits_set,
    //.swd_set_target_reset = swd_set_target_reset_ama3b1kk,
    //.target_set_state = swd_set_state_ama3b1kk,
};

const target_family_descriptor_t *g_target_family = &_g_target_family;
