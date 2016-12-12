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

#ifndef STDLCD_IO_H
#define STDLCD_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdLCD.h"
#include <stdint.h>

extern void stdlcd_delay_1us(uint32_t n);
extern void stdlcd_io_w(stdLCD_t* _this, uint8_t ioValue);
extern uint8_t stdlcd_io_r(stdLCD_t* _this);

#ifdef __cplusplus
}
#endif

#endif
