/*
 * VARCem	Virtual Archaelogical Computer EMulator.
 *		An emulator of (mostly) x86-based PC systems and devices,
 *		using the ISA,EISA,VLB,MCA  and PCI system buses, roughly
 *		spanning the era between 1981 and 1995.
 *
 *		This file is part of the VARCem Project.
 *
 *		Definitions for the video controller module.
 *
 * Version:	@(#)video.h	1.0.2	2018/02/22
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
#ifndef EMU_VIDEO_H
# define EMU_VIDEO_H


#define makecol(r, g, b)    ((b) | ((g) << 8) | ((r) << 16))
#define makecol32(r, g, b)  ((b) | ((g) << 8) | ((r) << 16))


enum {
    GFX_NONE = 0,
    GFX_INTERNAL,
    GFX_CGA,
    GFX_COMPAQ_CGA,		/* Compaq CGA */
    GFX_COMPAQ_CGA_2,		/* Compaq CGA 2 */
    GFX_COLORPLUS,		/* Plantronics ColorPlus */
    GFX_WY700,			/* Wyse 700 */
    GFX_MDA,
    GFX_GENIUS,			/* MDSI Genius */
    GFX_HERCULES,
    GFX_HERCULESPLUS,
    GFX_INCOLOR,		/* Hercules InColor */
    GFX_EGA,			/* Using IBM EGA BIOS */
    GFX_COMPAQ_EGA,		/* Compaq EGA */
    GFX_SUPER_EGA,		/* Using Chips & Technologies SuperEGA BIOS */
    GFX_VGA,        		/* IBM VGA */
    GFX_TVGA,			/* Using Trident TVGA8900D BIOS */
    GFX_ET4000,			/* Tseng ET4000 */
    GFX_ET4000W32_CARDEX_VLB,	/* Tseng ET4000/W32p (Cardex) VLB */
    GFX_ET4000W32_CARDEX_PCI,	/* Tseng ET4000/W32p (Cardex) PCI */
#if defined(DEV_BRANCH) && defined(USE_STEALTH32)
    GFX_ET4000W32_VLB,		/* Tseng ET4000/W32p (Diamond Stealth 32) VLB */
    GFX_ET4000W32_PCI,		/* Tseng ET4000/W32p (Diamond Stealth 32) PCI */
#endif
    GFX_BAHAMAS64_VLB,		/* S3 Vision864 (Paradise Bahamas 64) VLB */
    GFX_BAHAMAS64_PCI,		/* S3 Vision864 (Paradise Bahamas 64) PCI */
    GFX_N9_9FX_VLB,		/* S3 764/Trio64 (Number Nine 9FX) VLB */
    GFX_N9_9FX_PCI,		/* S3 764/Trio64 (Number Nine 9FX) PCI */
    GFX_TGUI9400CXI,   		/* Trident TGUI9400CXi VLB */
    GFX_TGUI9440_VLB,   	/* Trident TGUI9440 VLB */
    GFX_TGUI9440_PCI,   	/* Trident TGUI9440 PCI */
    GFX_VGA88,  		/* ATI VGA-88 (18800-1) */
    GFX_VGAEDGE16,  		/* ATI VGA Edge-16 (18800-1) */
    GFX_VGACHARGER, 		/* ATI VGA Charger (28800-5) */
    GFX_VGAWONDER,		/* Compaq ATI VGA Wonder (18800) */
    GFX_VGAWONDERXL,		/* Compaq ATI VGA Wonder XL (28800-5) */
#if defined(DEV_BRANCH) && defined(USE_XL24)
    GFX_VGAWONDERXL24,		/* Compaq ATI VGA Wonder XL24 (28800-6) */
#endif
    GFX_MACH64GX_ISA,		/* ATI Graphics Pro Turbo (Mach64) ISA */
    GFX_MACH64GX_VLB,		/* ATI Graphics Pro Turbo (Mach64) VLB */
    GFX_MACH64GX_PCI,		/* ATI Graphics Pro Turbo (Mach64) PCI */
    GFX_MACH64VT2,  		/* ATI Mach64 VT2 */
    GFX_CL_GD5428,  		/* Diamond SpeedStar PRO (Cirrus Logic CL-GD 5428) */
    GFX_CL_GD5429,  		/* Cirrus Logic CL-GD 5429 */
    GFX_OTI067,     		/* Oak OTI-067 */
    GFX_OTI077,     		/* Oak OTI-077 */
    GFX_PVGA1A,			/* Paradise PVGA1A Standalone */
    GFX_WD90C11,		/* Paradise WD90C11-LR Standalone */
    GFX_WD90C30,		/* Paradise WD90C30-LR Standalone */
    GFX_PHOENIX_TRIO32_VLB, 	/* S3 732/Trio32 (Phoenix) VLB */
    GFX_PHOENIX_TRIO32_PCI, 	/* S3 732/Trio32 (Phoenix) PCI */
    GFX_PHOENIX_TRIO64_VLB, 	/* S3 764/Trio64 (Phoenix) VLB */
    GFX_PHOENIX_TRIO64_PCI, 	/* S3 764/Trio64 (Phoenix) PCI */
    GFX_VIRGE_VLB,      	/* S3 Virge VLB */
    GFX_VIRGE_PCI,      	/* S3 Virge PCI */
    GFX_VIRGEDX_VLB,    	/* S3 Virge/DX VLB */
    GFX_VIRGEDX_PCI,    	/* S3 Virge/DX PCI */
    GFX_VIRGEDX4_VLB,		/* S3 Virge/DX (VBE 2.0) VLB */
    GFX_VIRGEDX4_PCI,		/* S3 Virge/DX (VBE 2.0) PCI */
    GFX_VIRGEVX_VLB,		/* S3 Virge/VX VLB */
    GFX_VIRGEVX_PCI,		/* S3 Virge/VX PCI */
    GFX_STEALTH64_VLB,		/* S3 Vision864 (Diamond Stealth 64) VLB */
    GFX_STEALTH64_PCI,		/* S3 Vision864 (Diamond Stealth 64) PCI */
    GFX_PHOENIX_VISION864_VLB,	/* S3 Vision864 (Phoenix) VLB */
    GFX_PHOENIX_VISION864_PCI,	/* S3 Vision864 (Phoenix) PCI */
#if defined(DEV_BRANCH) && defined(USE_TI)
    GFX_TICF62011,  		/* TI CF62011 */
#endif

    GFX_MAX
};

#define MDA	( (gfxcard >= GFX_MDA) && \
		  (gfxcard <  GFX_EGA) && \
		 ((romset  <  ROM_TANDY) || \
		  (romset  >= ROM_AMI286)))

#define EGA	((gfxcard >= GFX_EGA) && \
		 (gfxcard <  GFX_VGA))

#define VGA	((gfxcard >= GFX_VGA))

#define EGA_VGA	( (EGA || VGA) && \
		 ((romset   < ROM_TANDY) || \
		  (romset  >= ROM_AMI286)))

enum {
    FULLSCR_SCALE_FULL = 0,
    FULLSCR_SCALE_43,
    FULLSCR_SCALE_SQ,
    FULLSCR_SCALE_INT,
    FULLSCR_SCALE_KEEPRATIO
};


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    int		type;
    int		write_b, write_w, write_l;
    int		read_b, read_w, read_l;
} video_timings_t;

typedef struct {
    int		w, h;
    uint8_t	*dat;
    uint8_t	*line[];
} bitmap_t;

typedef struct {
    uint8_t	r, g, b;
} rgb_t;

typedef rgb_t PALETTE[256];


extern int	gfx_present[GFX_MAX];
extern int	egareads,
		egawrites;
extern int	changeframecount;

extern bitmap_t	*screen,
		*buffer,
		*buffer32;
extern PALETTE	cgapal,
		cgapal_mono[6];
extern uint32_t	pal_lookup[256];
extern int	video_fullscreen,
		video_fullscreen_scale,
		video_fullscreen_first;
extern int	fullchange;
extern uint8_t	fontdat[2048][8];
extern uint8_t	fontdatm[2048][16];
extern uint32_t	*video_6to8,
		*video_15to32,
		*video_16to32;
extern int	xsize,ysize;
extern int	enable_overscan;
extern int	overscan_x,
		overscan_y;
extern int	force_43;
extern int	video_timing_read_b,
		video_timing_read_w,
		video_timing_read_l;
extern int	video_timing_write_b,
		video_timing_write_w,
		video_timing_write_l;
extern int	video_speed;
extern int	video_res_x,
		video_res_y,
		video_bpp;
extern int	vid_resize;
extern int	cga_palette;
extern int	vid_cga_contrast;
extern int	video_grayscale;
extern int	video_graytype;

extern float	cpuclock;
extern int	emu_fps,
		frames;
extern int	readflash;


/* Function handler pointers. */
extern void	(*video_recalctimings)(void);


/* Table functions. */
extern int	video_card_available(int card);
extern char	*video_card_getname(int card);
#ifdef EMU_DEVICE_H
extern device_t	*video_card_getdevice(int card);
#endif
extern int	video_card_has_config(int card);
extern video_timings_t	*video_card_gettiming(int card);
extern int	video_card_getid(char *s);
extern int	video_old_to_new(int card);
extern int	video_new_to_old(int card);
extern char	*video_get_internal_name(int card);
extern int	video_get_video_from_internal_name(char *s);


extern void	video_setblit(void(*blit)(int,int,int,int,int,int));
extern void	video_blit_memtoscreen(int x, int y, int y1, int y2, int w, int h);
extern void	video_blit_memtoscreen_8(int x, int y, int y1, int y2, int w, int h);
extern void	video_blit_complete(void);
extern void	video_wait_for_blit(void);
extern void	video_wait_for_buffer(void);

extern bitmap_t	*create_bitmap(int w, int h);
extern void	destroy_bitmap(bitmap_t *b);
extern void	cgapal_rebuild(void);
extern void	hline(bitmap_t *b, int x1, int y, int x2, uint32_t col);
extern void	updatewindowsize(int x, int y);

extern void	video_init(void);
extern void	video_close(void);
extern void	video_reset(int card);
extern uint8_t	video_force_resize_get(void);
extern void	video_force_resize_set(uint8_t res);
extern void	video_update_timing(void);

extern void	loadfont(wchar_t *s, int format);

extern int	get_actual_size_x(void);
extern int	get_actual_size_y(void);

#ifdef ENABLE_VRAM_DUMP
extern void	svga_dump_vram(void);
#endif

#ifdef __cplusplus
}
#endif


#endif	/*EMU_VIDEO_H*/