/*
 * VARCem	Virtual ARchaeological Computer EMulator.
 *		An emulator of (mostly) x86-based PC systems and devices,
 *		using the ISA,EISA,VLB,MCA  and PCI system buses, roughly
 *		spanning the era between 1981 and 1995.
 *
 *		This file is part of the VARCem Project.
 *
 *		Implementation of standard IBM PC/XT class machine.
 *
 * Version:	@(#)m_xt.c	1.0.10	2018/05/06
 *
 * Authors:	Fred N. van Kempen, <decwiz@yahoo.com>
 *		Miran Grca, <mgrca8@gmail.com>
 *		Sarah Walker, <tommowalker@tommowalker.co.uk>
 *
 *		Copyright 2017,2018 Fred N. van Kempen.
 *		Copyright 2016-2018 Miran Grca.
 *		Copyright 2008-2018 Sarah Walker.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free  Software  Foundation; either  version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is  distributed in the hope that it will be useful, but
 * WITHOUT   ANY  WARRANTY;  without  even   the  implied  warranty  of
 * MERCHANTABILITY  or FITNESS  FOR A PARTICULAR  PURPOSE. See  the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the:
 *
 *   Free Software Foundation, Inc.
 *   59 Temple Place - Suite 330
 *   Boston, MA 02111-1307
 *   USA.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>
#include "../emu.h"
#include "../mem.h"
#include "../device.h"
#include "../devices/system/nmi.h"
#include "../devices/system/pit.h"
#include "../devices/input/keyboard.h"
#include "../devices/floppy/fdd.h"
#include "../devices/floppy/fdc.h"
#include "machine.h"


static int	rom_basic = 0;			/* is ROM BASIC enabled? */


static const device_config_t xt_config[] = {
    {
	"rom_basic", "ROM BASIC", CONFIG_SELECTION, "", 0,
	{
		{
			"Disabled", 0
		},
		{
			"Enabled", 1
		},
		{
			""
		}
	}
    },
    {
	"", "", -1
    }
};


void
machine_xt_init(const machine_t *model, void *arg)
{
    machine_common_init(model, arg);

    /* Check if we support a BASIC ROM. */
    if (model->device != NULL) {
	pclog("This (%s) machine supports a BASIC ROM.\n", model->name);

	rom_basic = machine_get_config_int("rom_basic");
	pclog("ROM BASIC is currently %sabled.\n", (rom_basic)?"en":"dis");
    }

    pit_set_out_func(&pit, 1, pit_refresh_timer_xt);

    device_add(&keyboard_xt_device);

    device_add(&fdc_xt_device);

    nmi_init();
}


const device_t m_xt_device = {
    "PC/XT",
    0, 0,
    NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    xt_config
};
