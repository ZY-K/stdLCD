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

#include "stdLCD.h"
#include "stdlcd_io.h"
#include <assert.h>
#include <stdlib.h>


//RS = 0:cmd reg, RS = 1:data reg
//R/W = 0:write, R/W = 1:read¡C
//E = 0:LCD disable, E = 1:LCD enable¡C
#define DAT_WRITE(u)   ((u)|0x01)
#define CMD_WRITE(u)   (u)
#define DAT_READ(u)    ((u)|0x03)
#define CMD_READ(u)    ((u)|0x02)
#define En_TURN_ON(u)  ((u)|0x04)
#define En_TURN_OFF(u) ((u)&0xFB)


static inline void _stdlcd_w_nib(stdLCD_t* _this, volatile uint8_t data)
{
	data |= _this->backLight;
	stdlcd_io_w(_this, data);

	stdlcd_delay_1us(1);
	stdlcd_io_w(_this, En_TURN_ON(data));
	stdlcd_delay_1us(1);
	stdlcd_io_w(_this, En_TURN_OFF(data));
	stdlcd_delay_1us(300);
}

static inline uint8_t _stdlcd_bfaddr(stdLCD_t* _this)
{
	assert(_this!=NULL);
	stdlcd_io_w(_this, CMD_READ(0)|_this->backLight);

	uint8_t bfaddr=(stdlcd_io_r(_this)<<4) | (stdlcd_io_r(_this));
	return(bfaddr);
}
#define _lcd_address(_this) (_stdlcd_bfaddr(_this)&0x7F)
#define _lcd_busy(_this) (_stdlcd_bfaddr(_this)&0x80)


static inline void stdLCD_wcmd(stdLCD_t* _this, uint8_t cmd)
{
	assert(_this!=NULL);
	while(_lcd_busy(_this));
	uint8_t hi_nib=cmd&0xf0;
	uint8_t lo_nib=(cmd<<4);
	_stdlcd_w_nib(_this, CMD_WRITE(hi_nib));
	_stdlcd_w_nib(_this, CMD_WRITE(lo_nib));
}

static inline void stdLCD_wdat(stdLCD_t* _this, int8_t data)
{
	assert(_this!=NULL);
	while(_lcd_busy(_this));
	uint8_t hi_nib=data&0xf0;
	uint8_t lo_nib=(data<<4);
	_stdlcd_w_nib(_this, DAT_WRITE(hi_nib));
	_stdlcd_w_nib(_this, DAT_WRITE(lo_nib));
}

void stdLCD_init(stdLCD_t* _this, uint8_t instanceNum, stdLCD_display_size_t displaySize)
{
	assert(_this!=NULL);
	assert((stdLCD_display_16x1<=displaySize) && (displaySize<=stdLCD_display_40x4));

	_this->instanceNum = instanceNum;
	_this->backLight = 0x08;
	_this->displayControl = 0x08;
	_this->displaySize = displaySize;

	// initial start........
	// wait least 40ms, base on data sheet.
	stdlcd_delay_1us(42000);

	// set 4 bit mode - 1 step
	_stdlcd_w_nib(_this, (0x03 << 4));
	stdlcd_delay_1us(4200); // wait min 4.1ms

	// set 4 bit mode - 2 step
	_stdlcd_w_nib(_this, (0x03 << 4));
	stdlcd_delay_1us(150); // wait min 100us

	// set 4 bit mode - 3 step
	_stdlcd_w_nib(_this, (0x03 << 4));

	// set 4 bit mode - 4 step
	_stdlcd_w_nib(_this, (0x02 << 4));

	// now lcd is in 4 bit mode.

	// set 2 lines, 5*8 dot font etc.
	stdLCD_set_font(_this, stdLCD_2line_smallFont);

	// Display off
	stdLCD_set_display_off(_this);
	stdLCD_set_cursor_off(_this);
	stdLCD_set_blinks_off(_this);

	// clear it off
	stdLCD_clear(_this);

	// set the entry mode
	// cursor move to right, and string length will grow up to right, after the lcd prints every character.
	stdLCD_set_cursor_move_to_right_and_display_grow_up_to_right(_this);

	// initial ending........


	// move to home
	stdLCD_home(_this);

	// turn the display on with no cursor or blinking default
	stdLCD_set_display_on(_this);
	stdLCD_set_cursor_on(_this);
	stdLCD_set_blinks_on(_this);
	return;
}

void stdLCD_clear(stdLCD_t* _this)
{
	assert(_this!=NULL);
	stdLCD_wcmd(_this, 0x01);
}

void stdLCD_home(stdLCD_t* _this)
{
	assert(_this!=NULL);
	_this->cursorCnt = 0;
	stdLCD_wcmd(_this, 0x02);
}

void stdLCD_set_cursor_at(stdLCD_t* _this, uint8_t col, uint8_t row)
{
	assert(_this!=NULL);
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	switch(_this->displaySize)
	{
		case stdLCD_display_16x1: {if(col>0x4F)col=0x4F; row=0; _this->cursorCnt = col; break;}

		case stdLCD_display_16x2:
		case stdLCD_display_20x2:
		case stdLCD_display_40x2: {if(col>0x27)col=0x27; row &= 0x01; _this->cursorCnt = row*40u + col; break;}

		case stdLCD_display_16x4: {if(col>0x0F)col=0x0F; row &= 0x03; _this->cursorCnt = row*16u + col; break;}
		case stdLCD_display_20x4: {if(col>0x13)col=0x13; row &= 0x03; _this->cursorCnt = row*20u + col; break;}
		case stdLCD_display_40x4: {if(col>0x27)col=0x27; row &= 0x03; _this->cursorCnt = row*40u + col; break;}
	}
	stdLCD_wcmd(_this, 0x80 | (col + row_offsets[row]));
}

void stdLCD_get_cursor_at(stdLCD_t* _this, uint8_t* col, uint8_t* row)
{
	assert(_this!=NULL);
	switch(_this->displaySize)
	{
		case stdLCD_display_16x1: {*row=0; *col = _this->cursorCnt; break;}
		case stdLCD_display_16x2:
		case stdLCD_display_20x2:
		case stdLCD_display_40x2: {*row = _this->cursorCnt/40u; *col = _this->cursorCnt - *row*40u; break;}

		case stdLCD_display_16x4: {*row = _this->cursorCnt/16u; *col = _this->cursorCnt - *row*16u; break;}
		case stdLCD_display_20x4: {*row = _this->cursorCnt/20u; *col = _this->cursorCnt - *row*20u; break;}
		case stdLCD_display_40x4: {*row = _this->cursorCnt/40u; *col = _this->cursorCnt - *row*40u; break;}
	}
}

void stdLCD_set_display_on(stdLCD_t* _this)
{
	assert(_this!=NULL);
	_this->displayControl |= 0x04;
	stdLCD_wcmd(_this, _this->displayControl);
}
void stdLCD_set_display_off(stdLCD_t* _this)
{
	assert(_this!=NULL);
	_this->displayControl &= 0xFB;
	stdLCD_wcmd(_this, _this->displayControl);
}

void stdLCD_set_cursor_on(stdLCD_t* _this)
{
	assert(_this!=NULL);
	_this->displayControl |= 0x02;
	stdLCD_wcmd(_this, _this->displayControl);
}
void stdLCD_set_cursor_off(stdLCD_t* _this)
{
	assert(_this!=NULL);
	_this->displayControl &= 0xFD;
	stdLCD_wcmd(_this, _this->displayControl);
}

void stdLCD_set_blinks_on(stdLCD_t* _this)
{
	assert(_this!=NULL);
	_this->displayControl |= 0x01;
	stdLCD_wcmd(_this, _this->displayControl);
}
void stdLCD_set_blinks_off(stdLCD_t* _this)
{
	assert(_this!=NULL);
	_this->displayControl &= 0xFE;
	stdLCD_wcmd(_this, _this->displayControl);
}

void stdLCD_set_backLight_on(stdLCD_t* _this)
{
	assert(_this!=NULL);
	_this->backLight = 0x08;
	stdlcd_io_w(_this, _this->backLight);
}
void stdLCD_set_backLight_off(stdLCD_t* _this)
{
	assert(_this!=NULL);
	_this->backLight = 0x00;
	stdlcd_io_w(_this, _this->backLight);
}



void stdLCD_set_cursor_move_to_left_and_display_grow_up_to_left(stdLCD_t* _this)
{
	assert(_this!=NULL);
	stdLCD_wcmd(_this, 0x04);
}
void stdLCD_set_cursor_move_to_right_and_display_grow_up_to_right(stdLCD_t* _this)
{
	assert(_this!=NULL);
	stdLCD_wcmd(_this, 0x06);
}
void stdLCD_set_cursor_fixed_and_display_grow_up_to_right(stdLCD_t* _this)
{
	assert(_this!=NULL);
	stdLCD_wcmd(_this, 0x05);
}
void stdLCD_set_cursor_fixed_and_display_grow_up_to_left(stdLCD_t* _this)
{
	assert(_this!=NULL);
	stdLCD_wcmd(_this, 0x07);
}

void stdLCD_display_shift(stdLCD_t* _this, int8_t n)
{
	assert(_this!=NULL);

	if(n==0) return;
	if(n>0) //shift left
	{
		for(; n>0; n--) stdLCD_wcmd(_this, 0x18);
	}
	else  //shift right
	{
		for(; n<0; n++) stdLCD_wcmd(_this, 0x1C);
	}
}

void stdLCD_cursor_move(stdLCD_t* _this, int8_t n)
{
	assert(_this!=NULL);

	if(n==0) return;
	if(n>0) //move right
	{
		for(; n>0; n--) stdLCD_wcmd(_this, 0x14);
	}
	else //move left
	{
		for(; n<0; n++) stdLCD_wcmd(_this, 0x10);
	}
}

void stdLCD_set_font(stdLCD_t* _this, stdLCD_font_t font)
{
	assert(_this!=NULL);
	assert((stdLCD_1line_largeFont<=font) && (font<=stdLCD_2line_smallFont));

	switch(font)
	{
		case stdLCD_1line_largeFont: {stdLCD_wcmd(_this, 0x24); break;}
		case stdLCD_1line_smallFont: {stdLCD_wcmd(_this, 0x20); break;}
		case stdLCD_2line_smallFont: {stdLCD_wcmd(_this, 0x28); break;}
	}
}


void stdLCD_putc(stdLCD_t* _this, char c)
{
	assert(_this!=NULL);

	switch(c)
	{
		case '\0': {return;}
		case '\n':
		{
			uint8_t col, row;
			stdLCD_get_cursor_at(_this, &col, &row);
			stdLCD_set_cursor_at(_this, col, row+1);
			return;
		}
		case '\b':
		{
			stdLCD_cursor_move(_this, -1);
			return;
		}
		case '\r':
		{
			uint8_t col, row;
			stdLCD_get_cursor_at(_this, &col, &row);
			stdLCD_set_cursor_at(_this, 0, row);
			return;
		}
		case '\t':
		{
			uint8_t col, row;
			stdLCD_get_cursor_at(_this, &col, &row);
			stdLCD_set_cursor_at(_this, col+3, row);
			return;
		}
		default:
		{
			switch(_this->displaySize)
			{
				case stdLCD_display_16x1:
				case stdLCD_display_16x2:
				case stdLCD_display_20x2:
				case stdLCD_display_40x2: { break; }

				case stdLCD_display_16x4:
				{
					switch(_this->cursorCnt)
					{
						case 15: { stdLCD_set_cursor_at(_this, 0, 1); break; }
						case 31: { stdLCD_set_cursor_at(_this, 0, 2); break; }
						case 47: { stdLCD_set_cursor_at(_this, 0, 3); break; }
						default: { break; }
					}
				}
				case stdLCD_display_20x4:
				{
					switch(_this->cursorCnt)
					{
						case 19: { stdLCD_set_cursor_at(_this, 0, 1); break; }
						case 39: { stdLCD_set_cursor_at(_this, 0, 2); break; }
						case 59: { stdLCD_set_cursor_at(_this, 0, 3); break; }
						default: { break; }
					}
				}
				case stdLCD_display_40x4:
				{
					switch(_this->cursorCnt)
					{
						case 39: { stdLCD_set_cursor_at(_this, 0, 1); break; }
						case 79: { stdLCD_set_cursor_at(_this, 0, 2); break; }
						case 119:{ stdLCD_set_cursor_at(_this, 0, 3); break; }
						default: { break; }
					}
				}
			}

			_this->cursorCnt++;
			stdLCD_wdat(_this, (int8_t)c);
			break;
		}
	}
}

void stdLCD_puts(stdLCD_t* _this, char *s)
{
	assert(_this!=NULL);
	assert(s!=NULL);
	while(*s!='\0')
	{
		stdLCD_putc(_this, (uint8_t)*s++);
	}
}
/*
void stdLCD_printf(stdLCD_t* _this, const char * format, ...)
{
	assert(_this!=NULL);
}
*/

