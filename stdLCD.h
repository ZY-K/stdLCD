/*	stdLCD Library v0.1
 * 	Copyright (C) 2016, ZY-K
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	Author website:
 *	Author e-mail:
 */

#ifndef STDLCD_H
#define STDLCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define stdLCD_DBG
#ifndef stdLCD_DBG
#define NDEBUG
#endif

/********************
 * typedef
 ********************/
typedef enum
{
	stdLCD_display_16x1 = 1,
	stdLCD_display_16x2 = 2,
	stdLCD_display_16x4 = 3,
	stdLCD_display_20x2 = 4,
	stdLCD_display_20x4 = 5,
	stdLCD_display_40x2 = 6,
	stdLCD_display_40x4 = 7,
} stdLCD_display_size_t;

typedef enum
{
	stdLCD_1line_largeFont = 1,
	stdLCD_1line_smallFont = 2,
	stdLCD_2line_smallFont = 3,
} stdLCD_font_t;

typedef struct
{
	uint8_t instanceNum;
	uint8_t	backLight;
	uint8_t displayControl;
	uint8_t cursorCnt;
	stdLCD_display_size_t displaySize;
} stdLCD_t;

/********************
 * Library API
 ********************/
extern void stdLCD_init(stdLCD_t* _this, uint8_t instanceNum, stdLCD_display_size_t displaySize);
extern void stdLCD_set_display_on(stdLCD_t* _this);
extern void stdLCD_set_display_off(stdLCD_t* _this);
extern void stdLCD_set_cursor_on(stdLCD_t* _this);
extern void stdLCD_set_cursor_off(stdLCD_t* _this);
extern void stdLCD_set_blinks_on(stdLCD_t* _this);
extern void stdLCD_set_blinks_off(stdLCD_t* _this);
extern void stdLCD_set_backLight_on(stdLCD_t* _this);
extern void stdLCD_set_backLight_off(stdLCD_t* _this);
extern void stdLCD_set_font(stdLCD_t* _this, stdLCD_font_t font);
extern void stdLCD_set_cursor_move_to_left_and_display_grow_up_to_left(stdLCD_t* _this);
extern void stdLCD_set_cursor_move_to_right_and_display_grow_up_to_right(stdLCD_t* _this);
extern void stdLCD_set_cursor_fixed_and_display_grow_up_to_right(stdLCD_t* _this);
extern void stdLCD_set_cursor_fixed_and_display_grow_up_to_left(stdLCD_t* _this);
extern void stdLCD_set_cursor_at(stdLCD_t* _this, uint8_t col, uint8_t row);
extern void stdLCD_get_cursor_at(stdLCD_t* _this, uint8_t* col, uint8_t* row);

extern void stdLCD_clear(stdLCD_t* _this);
extern void stdLCD_home(stdLCD_t* _this);
extern void stdLCD_display_shift(stdLCD_t* _this, int8_t n);
extern void stdLCD_cursor_move(stdLCD_t* _this, int8_t n);
extern void stdLCD_putc(stdLCD_t* _this, char c);
extern void stdLCD_puts(stdLCD_t* _this, char *s);
//extern void stdLCD_printf(stdLCD_t* _this, const char * format, ...);

#ifdef __cplusplus
}
#endif

#endif
