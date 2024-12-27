#ifndef __LCD_H
#define __LCD_H

#include "n32g45x.h"
 
static	uint8_t LCD_GRAM[144][8];
 
#define		SCL_0		GPIO_ResetBits(GPIOB,GPIO_PIN_13)
#define		SCL_1		GPIO_SetBits(GPIOB,GPIO_PIN_13)

#define		SDA_0		GPIO_ResetBits(GPIOB,GPIO_PIN_15)
#define		SDA_1		GPIO_SetBits(GPIOB,GPIO_PIN_15)
 
#define		RSTB_0		GPIO_ResetBits(GPIOE,GPIO_PIN_5)
#define		RSTB_1		GPIO_SetBits(GPIOE,GPIO_PIN_5)
 
#define		DC_0		GPIO_ResetBits(GPIOE,GPIO_PIN_4)
#define		DC_1		GPIO_SetBits(GPIOE,GPIO_PIN_4)
 
#define		CS_0		GPIO_ResetBits(GPIOB,GPIO_PIN_12)
#define		CS_1		GPIO_SetBits(GPIOB,GPIO_PIN_12)
 
#define		PS_0		GPIO_ResetBits(GPIOD,GPIO_PIN_0)
#define		PS_1		GPIO_SetBits(GPIOD,GPIO_PIN_0)

#define		C86_0		GPIO_ResetBits(GPIOD,GPIO_PIN_1)
#define		C86_1		GPIO_SetBits(GPIOD,GPIO_PIN_1)
 
 
#define uchar unsigned char 
#define uint unsigned int 
#define ulong unsigned long
void gpio_Init(void);
void transfer_command_lcd(uint16_t command);
void transfer_data_lcd(uint16_t data);
void clear_screen(void);
void delay(int i);
void delay_us(int i);
void lcd_address(uint page,uint column);
void display_test(uchar data_left,uchar data_right);
void display_graphic_128x32(uint page,uint column,uchar *dp);
void initial_lcd(void);
void display_graphic_16x16(uint page,uint column,uint reverse,uchar *dp);
void display_string_8x16(uint page,uint column,uchar reverse,uchar *text);

#endif

