/*
 * VARCem	Virtual ARchaeological Computer EMulator.
 *		An emulator of (mostly) x86-based PC systems and devices,
 *		using the ISA,EISA,VLB,MCA  and PCI system buses, roughly
 *		spanning the era between 1981 and 1995.
 *
 *		This file is part of the VARCem Project.
 *
 *		Implementation of the Intel 440FX PCISet chip.
 *
 * Version:	@(#)m_at_440fx.c	1.0.6	2018/05/06
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
#include "../io.h"
#include "../mem.h"
#include "../device.h"
#include "../devices/system/pci.h"
#include "../devices/system/memregs.h"
#include "../devices/system/intel_piix.h"
#include "../devices/system/intel_flash.h"
#include "../devices/sio/sio.h"
#include "../devices/input/keyboard.h"
#include "machine.h"


static uint8_t card_i440fx[256];


static void i440fx_map(uint32_t addr, uint32_t size, int state)
{
	switch (state & 3)
	{
		case 0:
		mem_set_mem_state(addr, size, MEM_READ_EXTERNAL | MEM_WRITE_EXTERNAL);
		break;
		case 1:
		mem_set_mem_state(addr, size, MEM_READ_INTERNAL | MEM_WRITE_EXTERNAL);
		break;
		case 2:
		mem_set_mem_state(addr, size, MEM_READ_EXTERNAL | MEM_WRITE_INTERNAL);
		break;
		case 3:
		mem_set_mem_state(addr, size, MEM_READ_INTERNAL | MEM_WRITE_INTERNAL);
		break;
	}
	flushmmucache_nopc();	
}


static void i440fx_write(int func, int addr, uint8_t val, void *priv)
{
	if (func)
	   return;

	if ((addr >= 0x10) && (addr < 0x4f))
		return;

	switch (addr)
	{
		case 0x00: case 0x01: case 0x02: case 0x03:
		case 0x08: case 0x09: case 0x0a: case 0x0b:
		case 0x0c: case 0x0e:
		return;

		case 0x04: /*Command register*/
		val &= 0x02;
		val |= 0x04;
		break;
		case 0x05:
		val = 0;
		break;

		case 0x06: /*Status*/
		val = 0;
		break;
		case 0x07:
		val &= 0x80;
		val |= 0x02;
		break;

		case 0x59: /*PAM0*/
		if ((card_i440fx[0x59] ^ val) & 0xf0)
		{
			i440fx_map(0xf0000, 0x10000, val >> 4);
			shadowbios = (val & 0x10);
		}
		break;
		case 0x5a: /*PAM1*/
		if ((card_i440fx[0x5a] ^ val) & 0x0f)
			i440fx_map(0xc0000, 0x04000, val & 0xf);
		if ((card_i440fx[0x5a] ^ val) & 0xf0)
			i440fx_map(0xc4000, 0x04000, val >> 4);
		break;
		case 0x5b: /*PAM2*/
		if ((card_i440fx[0x5b] ^ val) & 0x0f)
			i440fx_map(0xc8000, 0x04000, val & 0xf);
		if ((card_i440fx[0x5b] ^ val) & 0xf0)
			i440fx_map(0xcc000, 0x04000, val >> 4);
		break;
		case 0x5c: /*PAM3*/
		if ((card_i440fx[0x5c] ^ val) & 0x0f)
			i440fx_map(0xd0000, 0x04000, val & 0xf);
		if ((card_i440fx[0x5c] ^ val) & 0xf0)
			i440fx_map(0xd4000, 0x04000, val >> 4);
		break;
		case 0x5d: /*PAM4*/
		if ((card_i440fx[0x5d] ^ val) & 0x0f)
			i440fx_map(0xd8000, 0x04000, val & 0xf);
		if ((card_i440fx[0x5d] ^ val) & 0xf0)
			i440fx_map(0xdc000, 0x04000, val >> 4);
		break;
		case 0x5e: /*PAM5*/
		if ((card_i440fx[0x5e] ^ val) & 0x0f)
			i440fx_map(0xe0000, 0x04000, val & 0xf);
		if ((card_i440fx[0x5e] ^ val) & 0xf0)
			i440fx_map(0xe4000, 0x04000, val >> 4);
		break;
		case 0x5f: /*PAM6*/
		if ((card_i440fx[0x5f] ^ val) & 0x0f)
			i440fx_map(0xe8000, 0x04000, val & 0xf);
		if ((card_i440fx[0x5f] ^ val) & 0xf0)
			i440fx_map(0xec000, 0x04000, val >> 4);
		break;
		case 0x72: /*SMRAM*/
		if ((card_i440fx[0x72] ^ val) & 0x48)
			i440fx_map(0xa0000, 0x20000, ((val & 0x48) == 0x48) ? 3 : 0);
		break;
	}

	card_i440fx[addr] = val;
}


static uint8_t i440fx_read(int func, int addr, void *priv)
{
	if (func)
	   return 0xff;

	return card_i440fx[addr];
}


static void i440fx_reset(void)
{
	memset(card_i440fx, 0, 256);
	card_i440fx[0x00] = 0x86; card_i440fx[0x01] = 0x80; /*Intel*/
	card_i440fx[0x02] = 0x37; card_i440fx[0x03] = 0x12; /*82441FX*/
	card_i440fx[0x04] = 0x03; card_i440fx[0x05] = 0x01;
	card_i440fx[0x06] = 0x80; card_i440fx[0x07] = 0x00;
	card_i440fx[0x08] = 0x02; /*A0 stepping*/
	card_i440fx[0x09] = 0x00; card_i440fx[0x0a] = 0x00; card_i440fx[0x0b] = 0x06;
	card_i440fx[0x0d] = 0x00;
	card_i440fx[0x0f] = 0x00;
	card_i440fx[0x2c] = 0xf4;
	card_i440fx[0x2d] = 0x1a;
	card_i440fx[0x2e] = 0x00;
	card_i440fx[0x2f] = 0x11;
	card_i440fx[0x50] = 0x00;
	card_i440fx[0x51] = 0x01;
	card_i440fx[0x52] = card_i440fx[0x54] = card_i440fx[0x55] = card_i440fx[0x56] = 0x00;
	card_i440fx[0x53] = 0x80;
	card_i440fx[0x57] = 0x01;
	card_i440fx[0x58] = 0x10;
	card_i440fx[0x5a] = card_i440fx[0x5b] = card_i440fx[0x5c] = card_i440fx[0x5d] = card_i440fx[0x5e] = 0x11;
	card_i440fx[0x5f] = 0x31;
	card_i440fx[0x72] = 0x02;
}


static void i440fx_pci_reset(void)
{
	i440fx_write(0, 0x59, 0x00, NULL);
	i440fx_write(0, 0x72, 0x02, NULL);
}


static void i440fx_init(void)
{
	pci_add_card(0, i440fx_read, i440fx_write, NULL);

	i440fx_reset();

	pci_reset_handler.pci_master_reset = i440fx_pci_reset;
}


void
machine_at_i440fx_init(const machine_t *model, void *arg)
{
	machine_at_ps2_init(model, arg);

	memregs_init();
	pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_SPECIAL, 0, 0, 0, 0);
	pci_register_slot(0x0E, PCI_CARD_NORMAL, 1, 2, 3, 4);
	pci_register_slot(0x0D, PCI_CARD_NORMAL, 2, 3, 4, 1);
	pci_register_slot(0x0C, PCI_CARD_NORMAL, 3, 4, 1, 2);
	pci_register_slot(0x0B, PCI_CARD_NORMAL, 4, 1, 2, 3);
	pci_register_slot(0x0A, PCI_CARD_NORMAL, 1, 2, 3, 4);
	pci_register_slot(0x07, PCI_CARD_SPECIAL, 0, 0, 0, 0);
	i440fx_init();
	piix3_init(7);
	fdc37c665_init();

	device_add(&intel_flash_bxt_device);
}


void
machine_at_s1668_init(const machine_t *model, void *arg)
{
	machine_at_common_init(model, arg);
	device_add(&keyboard_ps2_ami_device);

	memregs_init();
	pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_SPECIAL, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SPECIAL, 0, 0, 0, 0);
	pci_register_slot(0x0E, PCI_CARD_NORMAL, 1, 2, 3, 4);
	pci_register_slot(0x0D, PCI_CARD_NORMAL, 2, 3, 4, 1);
	pci_register_slot(0x0C, PCI_CARD_NORMAL, 3, 4, 1, 2);
	pci_register_slot(0x0B, PCI_CARD_NORMAL, 4, 1, 2, 3);
	pci_register_slot(0x0A, PCI_CARD_NORMAL, 1, 2, 3, 4);
	i440fx_init();
	piix3_init(7);
	fdc37c665_init();

	device_add(&intel_flash_bxt_device);
}
