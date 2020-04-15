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

#include "common/hardware/spu.h"
#include "openbios/pio/pio.h"
#include "openbios/tty/tty.h"

static void boot(char * systemCnfPath, char * binaryPath);

int main() {
    // RAM size
    *((uint32_t*) 0x60) = 0x02;
    // ??
    *((uint32_t*) 0x64) = 0x00;
    // ??
    *((uint32_t*) 0x68) = 0xff;

    POST = 0x0f;
    muteSpu();

    if (checkExp1PreHookLicense()) runExp1PreHook();
    POST = 0x0e;
    g_installTTY = 0;
    boot("cdrom:SYSTEM.CNF;1", "cdrom:PSX.EXE;1");
}

static void boot(char * systemCnfPath, char * binaryPath) {
    POST = 0x01;
}
