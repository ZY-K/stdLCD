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

#include "PCF8574.h"

/* user porting head file include start... */
#include "hsi2c.h"
#include "soc_AM335x.h"
/* user porting head file include end..... */

#define I2C_INTERRUPT_FLAG_TO_CLR I2C_INT_ARBITRATION_LOST    \
								| I2C_INT_NO_ACK              \
								| I2C_INT_ADRR_READY_ACESS    \
								| I2C_INT_RECV_READY          \
								| I2C_INT_TRANSMIT_READY      \
								| I2C_INT_GENERAL_CALL        \
								| I2C_INT_START               \
								| I2C_INT_ACCESS_ERROR        \
								| I2C_INT_STOP_CONDITION      \
								| I2C_INT_ADRR_SLAVE          \
								| I2C_INT_TRANSMIT_UNDER_FLOW \
								| I2C_INT_RECV_OVER_RUN       \
								| I2C_INT_RECV_DRAIN          \
								| I2C_INT_TRANSMIT_DRAIN      \
								| 0x1000

uint8_t PCF8574__read(uint8_t who)
{
    /* Set i2c slave address */
    I2CMasterSlaveAddrSet(SOC_I2C_1_REGS, who);

    /* Configure I2C controller in Master Transmitter mode */
    I2CMasterControl(SOC_I2C_1_REGS, I2C_CFG_MST_TX);

    I2CMasterIntClearEx(SOC_I2C_1_REGS,  I2C_INTERRUPT_FLAG_TO_CLR);

    /* Configure I2C controller in Master Receiver mode */
    I2CMasterControl(SOC_I2C_1_REGS, I2C_CFG_MST_RX);

    /* Generate re-start Condition over I2C bus */
    I2CMasterStart(SOC_I2C_1_REGS);
    while(0==I2CMasterBusBusy(SOC_I2C_1_REGS));
    while(!I2CMasterIntRawStatusEx(SOC_I2C_1_REGS, I2C_INT_RECV_READY));

    /* Receive data from data receive register */
    uint8_t value = I2CMasterDataGet(SOC_I2C_1_REGS);

    I2CMasterStop(SOC_I2C_1_REGS);
    while(!I2CMasterIntRawStatusEx(SOC_I2C_1_REGS, I2C_INT_STOP_CONDITION));
    I2CMasterIntClearEx(SOC_I2C_1_REGS, I2C_INT_STOP_CONDITION);

    return value;
}

void PCF8574__write(uint8_t who, uint8_t data)
{
    /* Set i2c slave address */
    I2CMasterSlaveAddrSet(SOC_I2C_1_REGS, who);

    /* Configure I2C controller in Master Transmitter mode */
    I2CMasterControl(SOC_I2C_1_REGS, I2C_CFG_MST_TX);

    /* Data Count specifies the number of bytes to be transmitted */
    I2CSetDataCount(SOC_I2C_1_REGS, 0x01);

    I2CMasterIntClearEx(SOC_I2C_1_REGS,  I2C_INTERRUPT_FLAG_TO_CLR);


    /* Generate Start Condition over I2C bus */
    I2CMasterStart(SOC_I2C_1_REGS);
    while(0==I2CMasterBusBusy(SOC_I2C_1_REGS));

    /* Put data to data transmit register of i2c */
    while(!I2CMasterIntRawStatusEx(SOC_I2C_1_REGS, I2C_INT_TRANSMIT_READY));
    I2CMasterDataPut(SOC_I2C_1_REGS, data);
    I2CMasterIntClearEx(SOC_I2C_1_REGS, I2C_INT_TRANSMIT_READY);

    I2CMasterStop(SOC_I2C_1_REGS);
    while(!I2CMasterIntRawStatusEx(SOC_I2C_1_REGS, I2C_INT_STOP_CONDITION));
    I2CMasterIntClearEx(SOC_I2C_1_REGS, I2C_INT_STOP_CONDITION);

    return;
}
