#include "hsi2c.h"
#include "beaglebone.h"
#include "mmu.h"
#include "cache.h"
#include "interrupt.h"
#include "consoleUtils.h"
#include "soc_AM335x.h"
#include <stdio.h>

#include "../../../stdLCD.h"


/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
*******************************************************************************/



/******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
static void SetupI2C(void);



/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/


/******************************************************************************
**              FUNCTION DEFINITIONS
******************************************************************************/
/*
** Function to setup MMU. This function Maps three regions (1. DDR
** 2. OCMC and 3. Device memory) and enables MMU.
*/
#define START_ADDR_DDR             (0x80000000)
#define START_ADDR_DEV             (0x44000000)
#define START_ADDR_OCMC            (0x40300000)
#define NUM_SECTIONS_DDR           (512)
#define NUM_SECTIONS_DEV           (960)
#define NUM_SECTIONS_OCMC          (1)

#ifdef __TMS470__
#pragma DATA_ALIGN(pageTable, 16384);
static volatile unsigned int pageTable[4*1024];
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=16384
static volatile unsigned int pageTable[4*1024];
#else
static volatile unsigned int pageTable[4*1024] __attribute__((aligned(16*1024)));
#endif
void MMUConfigAndEnable(void)
{
    /*
    ** Define DDR memory region of AM335x. DDR can be configured as Normal
    ** memory with R/W access in user/privileged modes. The cache attributes
    ** specified here are,
    ** Inner - Write through, No Write Allocate
    ** Outer - Write Back, Write Allocate
    */
    REGION regionDdr = {
                        MMU_PGTYPE_SECTION, START_ADDR_DDR, NUM_SECTIONS_DDR,
                        MMU_MEMTYPE_NORMAL_NON_SHAREABLE(MMU_CACHE_WT_NOWA,
                                                         MMU_CACHE_WB_WA),
                        MMU_REGION_NON_SECURE, MMU_AP_PRV_RW_USR_RW,
                        (unsigned int*)pageTable
                       };
    /*
    ** Define OCMC RAM region of AM335x. Same Attributes of DDR region given.
    */
    REGION regionOcmc = {
                         MMU_PGTYPE_SECTION, START_ADDR_OCMC, NUM_SECTIONS_OCMC,
                         MMU_MEMTYPE_NORMAL_NON_SHAREABLE(MMU_CACHE_WT_NOWA,
                                                          MMU_CACHE_WB_WA),
                         MMU_REGION_NON_SECURE, MMU_AP_PRV_RW_USR_RW,
                         (unsigned int*)pageTable
                        };

    /*
    ** Define Device Memory Region. The region between OCMC and DDR is
    ** configured as device memory, with R/W access in user/privileged modes.
    ** Also, the region is marked 'Execute Never'.
    */
    REGION regionDev = {
                        MMU_PGTYPE_SECTION, START_ADDR_DEV, NUM_SECTIONS_DEV,
                        MMU_MEMTYPE_DEVICE_SHAREABLE,
                        MMU_REGION_NON_SECURE,
                        MMU_AP_PRV_RW_USR_RW  | MMU_SECTION_EXEC_NEVER,
                        (unsigned int*)pageTable
                       };

    /* Initialize the page table and MMU */
    MMUInit((unsigned int*)pageTable);

    /* Map the defined regions */
    MMUMemRegionMap(&regionDdr);
    MMUMemRegionMap(&regionOcmc);
    MMUMemRegionMap(&regionDev);

    /* Now Safe to enable MMU */
    MMUEnable((unsigned int*)pageTable);
}

stdLCD_t lcd0;
int main(void)
{
    //MMUConfigAndEnable();
    //CacheEnable(CACHE_ALL);

    /* Initialize console for communication with the Host Machine */
    ConsoleUtilsInit();

    /* Select the console type based on compile time check */
    ConsoleUtilsSetType(CONSOLE_UART);


    DMTimer7ModuleClkConfig();
    /*
    ** Configures I2C to Master mode to generate start
    ** condition on I2C bus and to transmit data at a
    ** speed of 100khz
    */
    SetupI2C();



    volatile int i;
    stdLCD_init(&lcd0, 0, stdLCD_display_16x2);

    stdLCD_puts(&lcd0, "hello world!!\n\r");
    stdLCD_puts(&lcd0, "hello world!!");
    for(i=0; i<1000000; i++);
    stdLCD_set_backLight_off(&lcd0);
    for(i=0; i<1000000; i++);
    stdLCD_set_backLight_on(&lcd0);
    for(i=0; i<1000000; i++);
    stdLCD_set_backLight_off(&lcd0);
    for(i=0; i<1000000; i++);
    stdLCD_set_backLight_on(&lcd0);
    for(i=0; i<1000000; i++);

    while(1)
    {
        stdLCD_display_shift(&lcd0, -1);
        for(i=0; i<1000000; i++);
        stdLCD_display_shift(&lcd0, -1);
        for(i=0; i<1000000; i++);
        stdLCD_display_shift(&lcd0, -1);
        for(i=0; i<1000000; i++);
        stdLCD_display_shift(&lcd0, -1);
        for(i=0; i<1000000; i++);
        stdLCD_display_shift(&lcd0, 1);
        for(i=0; i<1000000; i++);
        stdLCD_display_shift(&lcd0, 1);
        for(i=0; i<1000000; i++);
        stdLCD_display_shift(&lcd0, 1);
        for(i=0; i<1000000; i++);
        stdLCD_display_shift(&lcd0, 1);
        for(i=0; i<1000000; i++);
    }
#if 0
    while(1)
    {
    	stdLCD_putc(&lcd0, 'a');
    	//for(i=0; i<3000; i++);
    	stdLCD_putc(&lcd0, 'b');
    	//for(i=0; i<3000; i++);
    	stdLCD_putc(&lcd0, 'c');
    	//for(i=0; i<3000; i++);
    	//stdLCD_set_cursor(&lcd0, 5, 1);
    	//stdLCD_set_cursor_auto_move_to_left(&lcd0);
    	//for(i=0; i<3000; i++);
    	stdLCD_putc(&lcd0, 'd');
    	//for(i=0; i<3000; i++);
    	stdLCD_putc(&lcd0, 'e');
    	//for(i=0; i<3000; i++);
    	stdLCD_putc(&lcd0, 'f');
    	stdLCD_putc(&lcd0, 'g');
    	stdLCD_putc(&lcd0, 'h');
    	stdLCD_putc(&lcd0, 'i');
    	stdLCD_putc(&lcd0, 'j');
    	stdLCD_putc(&lcd0, 'k');
    	stdLCD_putc(&lcd0, 'l');
    	stdLCD_putc(&lcd0, 'm');
    	stdLCD_putc(&lcd0, 'n');
    	stdLCD_putc(&lcd0, 'o');
    	stdLCD_putc(&lcd0, 'p');
    	//for(i=0; i<3000; i++);
    	//stdLCD_set_window_auto_shift_and_cursor_move_to_left(&lcd0);
    	//for(i=0; i<3000; i++);
    	stdLCD_putc(&lcd0, '0');
    	stdLCD_putc(&lcd0, '1');
    	stdLCD_putc(&lcd0, '2');
    	stdLCD_putc(&lcd0, '3');
    	stdLCD_putc(&lcd0, '4');
    	stdLCD_putc(&lcd0, '5');
    	stdLCD_putc(&lcd0, '6');
    	stdLCD_putc(&lcd0, '7');
    	stdLCD_putc(&lcd0, '8');
    	stdLCD_putc(&lcd0, '9');
    	stdLCD_putc(&lcd0, 'A');
    	stdLCD_putc(&lcd0, 'B');
    	stdLCD_putc(&lcd0, 'C');
    	stdLCD_putc(&lcd0, 'D');
    	stdLCD_putc(&lcd0, 'E');
    	stdLCD_putc(&lcd0, 'F');
    }
    stdLCD_display_shift(&lcd0, 2);
    stdLCD_display_shift(&lcd0, -2);
    stdLCD_set_cursor_at(&lcd0, 0, 1);
    uint8_t col, row;
    stdLCD_get_last_printed_char_at(&lcd0, &col, &row);
    stdLCD_set_cursor_at(&lcd0, 0, 0);
    stdLCD_puts(&lcd0, "hello !!\n\r\\\'\"");
#endif
}


static void SetupI2C(void)
{
    /* Enable the clock for I2C0 */
    I2C1ModuleClkConfig();

    I2CPinMuxSetup(1);

    /* Put i2c in reset/disabled state */
    I2CMasterDisable(SOC_I2C_1_REGS);

    I2CSoftReset(SOC_I2C_1_REGS);

    /* Disable auto Idle functionality */
    I2CAutoIdleDisable(SOC_I2C_1_REGS);

    /* Configure i2c bus speed to 100khz */
    I2CMasterInitExpClk(SOC_I2C_1_REGS, 48000000, 12000000, 100000);

    /* Bring I2C out of reset */
    I2CMasterEnable(SOC_I2C_1_REGS);

    while(!I2CSystemStatusGet(SOC_I2C_1_REGS));
}

