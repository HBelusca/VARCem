/*
 * VARCem	Virtual ARchaeological Computer EMulator.
 *		An emulator of (mostly) x86-based PC systems and devices,
 *		using the ISA,EISA,VLB,MCA  and PCI system buses, roughly
 *		spanning the era between 1981 and 1995.
 *
 *		This file is part of the VARCem Project.
 *
 *		Implementation of the Intel 430LX and 430NX PCISet chips.
 *
 * Version:	@(#)m_at_430lx_nx.c	1.0.6	2018/05/06
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
#include "../rom.h"
#include "../device.h"
#include "../devices/system/pci.h"
#include "../devices/system/memregs.h"
#include "../devices/system/intel.h"
#include "../devices/system/intel_flash.h"
#include "../devices/system/intel_sio.h"
#include "../devices/input/keyboard.h"
#include "../devices/sio/sio.h"
#include "machine.h"


static uint8_t card_i430_lx_nx[256];


static void i430lx_nx_map(uint32_t addr, uint32_t size, int state)
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


static void i430lx_nx_write(int func, int addr, uint8_t val, void *priv)
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
                val &= 0x42;
                val |= 0x04;
                break;
                case 0x05:
                val &= 0x01;
                break;
                
                case 0x06: /*Status*/
                val = 0;
                break;
                case 0x07:
                val = 0x02;
                break;
                
                case 0x59: /*PAM0*/
                if ((card_i430_lx_nx[0x59] ^ val) & 0xf0)
                {
                        i430lx_nx_map(0xf0000, 0x10000, val >> 4);
                        shadowbios = (val & 0x10);
                }
                pclog("i430lx_write : PAM0 write %02X\n", val);
                break;
                case 0x5a: /*PAM1*/
                if ((card_i430_lx_nx[0x5a] ^ val) & 0x0f)
                        i430lx_nx_map(0xc0000, 0x04000, val & 0xf);
                if ((card_i430_lx_nx[0x5a] ^ val) & 0xf0)
                        i430lx_nx_map(0xc4000, 0x04000, val >> 4);
                break;
                case 0x5b: /*PAM2*/
		if (romset == ROM_REVENGE)
		{
	                if ((card_i430_lx_nx[0x5b] ^ val) & 0x0f)
        	                i430lx_nx_map(0xc8000, 0x04000, val & 0xf);
	                if ((card_i430_lx_nx[0x5b] ^ val) & 0xf0)
        	                i430lx_nx_map(0xcc000, 0x04000, val >> 4);
		}
                break;
                case 0x5c: /*PAM3*/
                if ((card_i430_lx_nx[0x5c] ^ val) & 0x0f)
                        i430lx_nx_map(0xd0000, 0x04000, val & 0xf);
                if ((card_i430_lx_nx[0x5c] ^ val) & 0xf0)
                        i430lx_nx_map(0xd4000, 0x04000, val >> 4);
                break;
                case 0x5d: /*PAM4*/
                if ((card_i430_lx_nx[0x5d] ^ val) & 0x0f)
                        i430lx_nx_map(0xd8000, 0x04000, val & 0xf);
                if ((card_i430_lx_nx[0x5d] ^ val) & 0xf0)
                        i430lx_nx_map(0xdc000, 0x04000, val >> 4);
                break;
                case 0x5e: /*PAM5*/
                if ((card_i430_lx_nx[0x5e] ^ val) & 0x0f)
                        i430lx_nx_map(0xe0000, 0x04000, val & 0xf);
                if ((card_i430_lx_nx[0x5e] ^ val) & 0xf0)
                        i430lx_nx_map(0xe4000, 0x04000, val >> 4);
                pclog("i430lx_write : PAM5 write %02X\n", val);
                break;
                case 0x5f: /*PAM6*/
                if ((card_i430_lx_nx[0x5f] ^ val) & 0x0f)
                        i430lx_nx_map(0xe8000, 0x04000, val & 0xf);
                if ((card_i430_lx_nx[0x5f] ^ val) & 0xf0)
                        i430lx_nx_map(0xec000, 0x04000, val >> 4);
                pclog("i430lx_write : PAM6 write %02X\n", val);
                break;
        }
                
        card_i430_lx_nx[addr] = val;
}


static uint8_t i430lx_nx_read(int func, int addr, void *priv)
{
        if (func)
                return 0xff;

        return card_i430_lx_nx[addr];
}


static void i430lx_nx_reset_common(void)
{
        memset(card_i430_lx_nx, 0, 256);
        card_i430_lx_nx[0x00] = 0x86; card_i430_lx_nx[0x01] = 0x80; /*Intel*/
        card_i430_lx_nx[0x02] = 0xa3; card_i430_lx_nx[0x03] = 0x04; /*82434LX/NX*/
        card_i430_lx_nx[0x04] = 0x06; card_i430_lx_nx[0x05] = 0x00;
        card_i430_lx_nx[0x06] = 0x00; card_i430_lx_nx[0x07] = 0x02;
        card_i430_lx_nx[0x09] = 0x00; card_i430_lx_nx[0x0a] = 0x00; card_i430_lx_nx[0x0b] = 0x06;
        card_i430_lx_nx[0x57] = 0x31;
        card_i430_lx_nx[0x60] = card_i430_lx_nx[0x61] = card_i430_lx_nx[0x62] = card_i430_lx_nx[0x63] = card_i430_lx_nx[0x64] = 0x02;
}


static void i430lx_reset(void)
{
	i430lx_nx_reset_common();
        card_i430_lx_nx[0x08] = 0x03; /*A3 stepping*/
        card_i430_lx_nx[0x50] = 0x80;
        card_i430_lx_nx[0x52] = 0x40; /*256kb PLB cache*/
}


static void i430nx_reset(void)
{
	i430lx_nx_reset_common();
        card_i430_lx_nx[0x08] = 0x10; /*A0 stepping*/
        card_i430_lx_nx[0x50] = 0xA0;
        card_i430_lx_nx[0x52] = 0x44; /*256kb PLB cache*/
	card_i430_lx_nx[0x66] = card_i430_lx_nx[0x67] = 0x02;
}


static void i430lx_nx_pci_reset(void)
{
	i430lx_nx_write(0, 0x59, 0x00, NULL);
}


static void i430lx_init(void)
{
        pci_add_card(0, i430lx_nx_read, i430lx_nx_write, NULL);

	i430lx_reset();

	pci_reset_handler.pci_master_reset = i430lx_nx_pci_reset;
}


static void i430nx_init(void)
{
        pci_add_card(0, i430lx_nx_read, i430lx_nx_write, NULL);

	i430nx_reset();

	pci_reset_handler.pci_master_reset = i430lx_nx_pci_reset;
}


static void
machine_at_premiere_common_init(const machine_t *model, void *arg)
{
        machine_at_common_init(model, arg);
	device_add(&keyboard_ps2_ami_device);

	memregs_init();
        pci_init(PCI_CONFIG_TYPE_2);
	pci_register_slot(0x00, PCI_CARD_SPECIAL, 0, 0, 0, 0);
	pci_register_slot(0x01, PCI_CARD_SPECIAL, 0, 0, 0, 0);
	pci_register_slot(0x06, PCI_CARD_NORMAL, 3, 2, 1, 4);
	pci_register_slot(0x0E, PCI_CARD_NORMAL, 2, 1, 3, 4);
	pci_register_slot(0x0C, PCI_CARD_NORMAL, 1, 3, 2, 4);
	pci_register_slot(0x02, PCI_CARD_SPECIAL, 0, 0, 0, 0);
 	sio_init(2);
        fdc37c665_init();
        intel_batman_init();

        device_add(&intel_flash_bxt_ami_device);
}


void
machine_at_batman_init(const machine_t *model, void *arg)
{
	machine_at_premiere_common_init(model, arg);

        i430lx_init();
}


void
machine_at_plato_init(const machine_t *model, void *arg)
{
	machine_at_premiere_common_init(model, arg);

        i430nx_init();
}
