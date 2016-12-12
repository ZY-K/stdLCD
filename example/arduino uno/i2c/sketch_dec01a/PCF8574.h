/*	i2c to gpio v0.1
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

#ifndef PCF8574_H
#define PCF8574_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint8_t PCF8574__read(uint8_t who);
void PCF8574__write(uint8_t who, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif
