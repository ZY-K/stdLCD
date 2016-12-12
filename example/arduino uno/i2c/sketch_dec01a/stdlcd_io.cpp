/*  stdLCD Library v0.1
 *  Copyright (C) 2016, ZY-K
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author website:
 *  Author e-mail:
 */

#include "stdlcd_io.h"
/* user porting head file include start... */
#include "Arduino.h"
#include "PCF8574.h"
/* user porting head file include end..... */


extern void stdlcd_delay_1us(uint32_t n)
{
  delayMicroseconds(n);
}

/* The porting rule :
 * This lib is drive LCD in 4bit mode.
 * The lowest level function for this lib are "_lcd_io_w" and "_lcd_io_r",
 * these function parm 'io_value', must be follow below rule,
 * please base your platform to porting.
 *
 * io_value's bit7 is LCD pin D7
 * io_value's bit6 is LCD pin D6
 * io_value's bit5 is LCD pin D5
 * io_value's bit4 is LCD pin D4
 * io_value's bit3 is LCD pin back light control (option, by your hardware).
 * io_value's bit2 is LCD pin E
 * io_value's bit1 is LCD pin RW
 * io_value's bit0 is LCD pin RS
 */
void stdlcd_io_w(stdLCD_t* _this, uint8_t ioValue)
{
	switch(_this->instanceNum)
	{
		default: break;
		case 0:
		{
			/* put your io control function */
			PCF8574__write(0x27, ioValue);
			break;
		}
		case 1:
		{

			break;
		}
	}
}
uint8_t stdlcd_io_r(stdLCD_t* _this)
{
	switch(_this->instanceNum)
	{
		default: break;
		case 0:
		{
			/* put your io control function */
			return PCF8574__read(0x27);
			break;
		}
		case 1:
		{

			break;
		}
	}
	return 0;
}
