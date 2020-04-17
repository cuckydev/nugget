/***************************************************************************
 *   Copyright (C) 2019 PCSX-Redux authors                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.           *
 ***************************************************************************/

#pragma once

#include "common/compiler/stdint.h"

#define HW_U8(x) (*(volatile uint8_t *)(x))
#define HW_U16(x) (*(volatile uint16_t *)(x))
#define HW_U32(x) (*(volatile uint32_t *)(x))
#define HW_S8(x) (*(volatile int8_t *)(x))
#define HW_S16(x) (*(volatile int16_t *)(x))
#define HW_S32(x) (*(volatile int32_t *)(x))

#define SPU_MVOL_L HW_U16(0x1f801d80)
#define SPU_MVOL_R HW_U16(0x1f801d82)
#define SPU_REVERB_L HW_U16(0x1f801d84)
#define SPU_REVERB_R HW_U16(0x1f801d86)

#define SIO1_DATA HW_U8(0x1f801050)
#define SIO1_STAT HW_U16(0x1f801054)
#define SIO1_MODE HW_U16(0x1f801058)
#define SIO1_CTRL HW_U16(0x1f80105a)
#define SIO1_BAUD HW_U16(0x1f80105e)

#define POST HW_U8(0xbf802041)

#define IREG HW_U32(0x1f801070)
#define IMASK HW_U32(0x1f801074)

#define DPCR HW_U32(0x1f8010f0)
#define DICR HW_U32(0x1f8010f4)

#define GPU_DATA HW_U32(0x1f801810)
#define GPU_STATUS HW_U32(0x1f801814)

#define ATCONS_STAT HW_U8(0x1f802000)
#define ATCONS_FIFO HW_U8(0x1f802002)
#define ATCONS_IRQ  HW_U8(0x1f802030)
#define ATCONS_IRQ2 HW_U8(0x1f802032)
