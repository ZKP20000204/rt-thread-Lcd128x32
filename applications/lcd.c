#include <stdio.h>
#include <stdint.h>
#include "font.h"//字符库
#include "lcd.h"
#include "spi.h"
void gpio_Init(void)
{
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_GPIOE, ENABLE);
    GPIO_InitType        GPIO_InitStructure;	
    // 配置 RST (PE5) 和 DC (PE4),
	GPIO_InitStructure.Pin = GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(GPIOE, &GPIO_InitStructure);
    /* PS(PD0),C86(PD1) */	
    GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(GPIOD, &GPIO_InitStructure);	
}
/**
 * @brief  写命令到 LCD 模块*
 */
void transfer_command_lcd(uint16_t command) 
{ 
//	uint8_t i;
	CS_0;
	DC_0;
//	for(i=0;i<8;i++)
//	{
//		SCL_0;
//		delay_us(1);
//		if((command&0x80))SDA_1;
//		else SDA_0;
//		SCL_1;
//		delay_us(1);
//		command=command<<1;
//	}
	SPI_output(command);
	CS_1;
}
/**
 * @brief  写数据到 LCD 模块*
 */
void transfer_data_lcd(uint16_t data) 
{ 
//	uint8_t j;	
	CS_0;
	DC_1;
//	for(j=0;j<8;j++)
//	{
//		SCL_0;
//		if((data&0x80))SDA_1;
//		else SDA_0;
//		SCL_1;
//		data=data<<1;
//	}
	SPI_output(data);
	CS_1; 
} 
/**
 * @brief  清屏
 */
void clear_screen(void) 
{ 
	 uchar i,j; 
	 for(j=0;j<4;j++) 
	 { 
		 transfer_command_lcd(0xb0+j); 
		 transfer_command_lcd(0x10); 
		 transfer_command_lcd(0x00); 
		 for(i=0;i<132;i++) 
		 { 
			transfer_data_lcd(0x00); 
		 } 
	 } 
}

 //长延时 
void delay(int i) 
{ 
	 uint j,k;
	 for(j=0;j<i;j++) 
	 for(k=0;k<500;k++); 
} 
//短延时 
void delay_us(int i) 
{ 
	 uint j,k; 
	 for(j=0;j<i;j++) 
	 for(k=0;k<10;k++); 
} 

void lcd_address(uint page,uint column) 
{ 
	 page=page-1; 
	 column=column-1; 
	 transfer_command_lcd(0xb0+page); 
	 transfer_command_lcd(0x10+((column>>4)&0x0f)); 
	 transfer_command_lcd(column&0x0f); 
} 

void display_test(uchar data_left,uchar data_right) 
{ 
	 int i,j; 
	 for(j=0;j<4;j++) 
	 { 
	 lcd_address(j,0); 
	 for(i=0;i<128;i++) 
	 { 
	 transfer_data_lcd(data_left); 
	 transfer_data_lcd(data_right); 
	 } 
	 } 
} 
/**
 * @brief  显示128X32的图片
 */
void display_graphic_128x32(uint page,uint column,uchar *dp) 
{ 
	 uint i,j; 
	 for(j=0;j<4;j++) 
	 { 
		lcd_address(page+j,column); 
		for(i=0;i<128;i++) 
		{ 
			transfer_data_lcd(*dp); 
			dp++; 
		} 
	 } 
}
/**
 * @brief  16x16文字显示.
 */
void display_graphic_16x16(uint page,uint column,uint reverse,uchar *dp) 
{ 
	 uint i,j; 
	 for(j=0;j<2;j++) 
	 { 
		lcd_address(page+j,column); 
		for(i=0;i<16;i++) 
		{ 
			if(reverse==1) transfer_data_lcd(~*dp); 
			else transfer_data_lcd(*dp); 
			dp++; 
		} 
	 } 
}
/**
 * @brief  显示一串 8x16 点阵的字符串括号里的参数分别为（页，列，是否反显，数据指针） 
 */
void display_string_8x16(uint page,uint column,uchar reverse,uchar *text) 
{ 
	 uint i=0,j,k,n,data1; 
	 while(text[i]>0x00) 
	 { 
		if((text[i]>=0x20)&&(text[i]<=0x7e)) 
		{ 
			j=text[i]-0x20; 
			for(n=0;n<2;n++) 
		{ 
		lcd_address(page+n,column); 
		for(k=0;k<8;k++) 
		{ 
			if(reverse==1) data1=~ascii_table_8x16[j][k+8*n]; 
			else data1=ascii_table_8x16[j][k+8*n]; 
			transfer_data_lcd(data1); 
		} 
		} 
		i++; 
		column+=8; 
		 
		 } 
			else 
			i++; 
	 } 
}
/**
 * @brief  初始化
 */
void initial_lcd(void) 
{ 
	 RSTB_0; //Reset the chip when reset=0 
	 delay(20); 
	 RSTB_1; 
	 transfer_command_lcd(0xe2); /*软复位*/ 
	 delay(20); 
	 transfer_command_lcd(0x2c); /*升压步聚 1*/ 
	 delay(5); 
	 transfer_command_lcd(0x2e); /*升压步聚 2*/ 
	 delay(5);
	 transfer_command_lcd(0x2f); /*升压步聚 3*/ 
	 delay(5); 
	 transfer_command_lcd(0x23); /*粗调对比度，可设置范围 20～27*/ 
	 transfer_command_lcd(0x81); /*微调对比度*/ 
	 transfer_command_lcd(0x20); /*微调对比度的值，可设置范围 0x00～0x3f*/ 
	 delay(25); 
	 transfer_command_lcd(0xa2); /*1/9 偏压比（bias）*/ 
	 transfer_command_lcd(0xc8); /*行扫描顺序：从上到下*/ 
	 transfer_command_lcd(0xa0); /*列扫描顺序：从左到右*/ 
	 transfer_command_lcd(0x40); /*起始行：从第一行开始*/ 
	 transfer_command_lcd(0xaf); /*开显示*/
}
