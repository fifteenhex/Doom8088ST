/*-----------------------------------------------------------------------------
 *
 *
 *  Copyright (C) 2025 Frenkel Smeijers
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *      Dummy video code
 *
 *-----------------------------------------------------------------------------*/

#include "compiler.h"

#include "i_system.h"
#include "i_video.h"
#include "m_random.h"
#include "r_defs.h"
#include "v_video.h"
#include "w_wad.h"

#include "globdata.h"

#include "fbdevgl/fbdevgl.h"

static struct fbdevgl_context fbglcntx;

extern const int16_t CENTERY;


void I_ReloadPalette(void)
{
}


void I_InitGraphicsHardwareSpecificCode(void)
{
	int ret;

	ret = fbdevgl_init("/dev/fb0", &fbglcntx);
	if (ret)
		I_Error("Failed to init fbdev");

	fbdevgl_setup_centered_window(&fbglcntx,
						 SCREENWIDTH,
						 SCREENHEIGHT);
}

void I_ShutdownGraphics(void)
{
}


void I_SetPalette(int8_t p)
{
}


void I_FinishUpdate(void)
{
	I_Error("Implement me: I_FinishUpdate: page flip or blit buffer");
}


void I_FinishViewWindow(void)
{
}


void R_DrawColumnSprite(const draw_column_vars_t *dcvars)
{
	I_Error("Implement me: R_DrawColumnSprite");
}


void R_DrawColumnWall(const draw_column_vars_t *dcvars)
{
	R_DrawColumnSprite(dcvars);
}


void R_DrawColumnFlat(uint8_t color, const draw_column_vars_t *dcvars)
{
	for (unsigned y = dcvars->yl; y <dcvars->yh; y++) {
		for (unsigned x = 0; x < 4; x++)
			fbdevgl_window_set_pixel(&fbglcntx, (dcvars->x * 4) +x, y, color);
	}
}


void R_DrawFuzzColumn(const draw_column_vars_t *dcvars)
{
}


void V_ClearViewWindow(void)
{
}


void V_InitDrawLine(void)
{
}


void V_ShutdownDrawLine(void)
{
}


void V_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
}


void V_DrawBackground(int16_t backgroundnum)
{
}

static uint8_t lumpy[9600];

static inline void draw_lump(const uint8_t *lump, size_t lumpLength)
{
	const unsigned int lumpstride = (SCREENWIDTH / 4);
	for (int line = 0; line < ((lumpLength * 4) /SCREENWIDTH); line++) {
			for (int b = 0; b < lumpstride * 4; b += 4) {
				uint8_t byte = lump[(line *lumpstride) + (b / 4)];
				fbdevgl_window_set_pixel(&fbglcntx, b,     line, ~(byte >> 6) & 0x3);
				fbdevgl_window_set_pixel(&fbglcntx, b + 1, line, ~(byte >> 4) & 0x3);
				fbdevgl_window_set_pixel(&fbglcntx, b + 2, line, ~(byte >> 2) & 0x3);
				fbdevgl_window_set_pixel(&fbglcntx, b + 3, line, ~(byte >> 0) & 0x3);
			//memcpy(fbglcntx.fb + (fbglcntx.stride * line), lump + (lumpstride * line), lumpstride);
			}
	}
}

void V_DrawRaw(int16_t num, uint16_t offset)
{
	const uint8_t *lump = W_TryGetLumpByNum(num);
	uint16_t lumpLength = W_LumpLength(num);

	offset = (offset / SCREENWIDTH) * VIEWWINDOWWIDTH;

	//printf("offset 0x%x, lump len %d\n", (unsigned) offset, (int) lumpLength);

	if (lump != NULL)
	{
		draw_lump(lump, lumpLength);
		Z_ChangeTagToCache(lump);
	}
	else {
		W_ReadLumpByNum(num, lumpy);
		draw_lump(lumpy, lumpLength);
	}
}


void ST_Drawer(void)
{
	//printf("%s:%d\n", __func__, __LINE__);
}


void V_DrawPatchNotScaled(int16_t x, int16_t y, const patch_t __far* patch)
{
	//printf("%s:%d\n", __func__, __LINE__);
}


void V_DrawPatchScaled(int16_t x, int16_t y, const patch_t __far* patch)
{
	//printf("%s:%d\n", __func__, __LINE__);
}


void wipe_StartScreen(void)
{
	fbdevgl_window_clear(&fbglcntx);
}


void D_Wipe(void)
{
}
