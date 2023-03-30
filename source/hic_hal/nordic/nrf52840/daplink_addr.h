/**
 * DAPLink Interface Firmware
 * Copyright (c) 2021 Arm Limited, All Rights Reserved
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

#ifndef DAPLINK_ADDR_H
#define DAPLINK_ADDR_H

/* Device sizes */

#define DAPLINK_ROM_START               0x00000000
#define DAPLINK_ROM_SIZE                0x00040000 // 256 KiB


#define DAPLINK_RAM_START               0x20000000
#define DAPLINK_RAM_SIZE                0x00010000 // 64 KiB

/* ROM sizes */

#if defined(THINGY91)

#define DAPLINK_ROM_BL_START            0x00000000
#define DAPLINK_ROM_BL_SIZE             0x00013000

#define DAPLINK_ROM_IF_START            0x00013000
#define DAPLINK_ROM_IF_SIZE             0x0002D000

#define DAPLINK_ROM_CONFIG_USER_START   0x000FE000
#define DAPLINK_ROM_CONFIG_USER_SIZE    0x00002000

#elif defined(NRF52840DONGLE)

#define DAPLINK_ROM_BL_START            0x00000000
#define DAPLINK_ROM_BL_SIZE             0x00001000

#define DAPLINK_ROM_IF_START            0x00001000
#define DAPLINK_ROM_IF_SIZE             0x0003E000

#define DAPLINK_ROM_CONFIG_USER_START   0x0003F000
#define DAPLINK_ROM_CONFIG_USER_SIZE    0x00001000

#else

#define DAPLINK_ROM_BL_START            0x00000000
#define DAPLINK_ROM_BL_SIZE             0x00010000 // 64 KiB bootloader

#define DAPLINK_ROM_IF_START            0x00010000
#define DAPLINK_ROM_IF_SIZE             0x0002F000

#define DAPLINK_ROM_CONFIG_USER_START   0x0003F000
#define DAPLINK_ROM_CONFIG_USER_SIZE    0x00001000

#endif

/* RAM sizes */

#define DAPLINK_RAM_APP_START           0x20000000
#define DAPLINK_RAM_APP_SIZE            0x0000FF00

#define DAPLINK_RAM_SHARED_START        0x2000FF00
#define DAPLINK_RAM_SHARED_SIZE         0x00000100

/* Flash Programming Info */

#define DAPLINK_SECTOR_SIZE             0x00001000
#define DAPLINK_MIN_WRITE_SIZE          0x00000100

/* Current build */

#if defined(DAPLINK_BL)

#define DAPLINK_ROM_APP_START            DAPLINK_ROM_BL_START
#define DAPLINK_ROM_APP_SIZE             DAPLINK_ROM_BL_SIZE
#define DAPLINK_ROM_UPDATE_START         DAPLINK_ROM_IF_START
#define DAPLINK_ROM_UPDATE_SIZE          DAPLINK_ROM_IF_SIZE

#elif defined(DAPLINK_IF)

#define DAPLINK_ROM_APP_START            DAPLINK_ROM_IF_START
#define DAPLINK_ROM_APP_SIZE             DAPLINK_ROM_IF_SIZE
#define DAPLINK_ROM_UPDATE_START         DAPLINK_ROM_BL_START
#define DAPLINK_ROM_UPDATE_SIZE          DAPLINK_ROM_BL_SIZE

#else

#error "Build must be either bootloader or interface"

#endif

#endif