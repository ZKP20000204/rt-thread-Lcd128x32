/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-19     Nations      first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "lcd.h"
#include "Photo.h"
#include "drv_spi.h"
#include "spi.h"


int main(void)
{
	SPI_init();
	gpio_Init();

	C86_1;
	PS_0;
	initial_lcd();
    while (1)
    {
		clear_screen(); 
		//display_graphic_16x16(1,30,1,zhuang);
		display_graphic_16x16(3,100,1,tai);
		//display_graphic_128x32(1,1,code_test);
		//display_graphic_128x32(1,1,gImage_222);	
		display_string_8x16(2,60,0,"5");
    }
}


