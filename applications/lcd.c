#include <stdio.h>
#include <stdint.h>
#include "font.h"//�ַ���
#include "lcd.h"
#include "spi.h"
void gpio_Init(void)
{
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_GPIOE, ENABLE);
    GPIO_InitType        GPIO_InitStructure;	
    // ���� RST (PE5) �� DC (PE4),
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
 * @brief  д��� LCD ģ��*
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
 * @brief  д���ݵ� LCD ģ��*
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
 * @brief  ����
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

 //����ʱ 
void delay(int i) 
{ 
	 uint j,k;
	 for(j=0;j<i;j++) 
	 for(k=0;k<500;k++); 
} 
//����ʱ 
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
 * @brief  ��ʾ128X32��ͼƬ
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
 * @brief  16x16������ʾ.
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
 * @brief  ��ʾһ�� 8x16 ������ַ���������Ĳ����ֱ�Ϊ��ҳ���У��Ƿ��ԣ�����ָ�룩 
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
 * @brief  ��ʼ��
 */
void initial_lcd(void) 
{ 
	 RSTB_0; //Reset the chip when reset=0 
	 delay(20); 
	 RSTB_1; 
	 transfer_command_lcd(0xe2); /*��λ*/ 
	 delay(20); 
	 transfer_command_lcd(0x2c); /*��ѹ���� 1*/ 
	 delay(5); 
	 transfer_command_lcd(0x2e); /*��ѹ���� 2*/ 
	 delay(5);
	 transfer_command_lcd(0x2f); /*��ѹ���� 3*/ 
	 delay(5); 
	 transfer_command_lcd(0x23); /*�ֵ��Աȶȣ������÷�Χ 20��27*/ 
	 transfer_command_lcd(0x81); /*΢���Աȶ�*/ 
	 transfer_command_lcd(0x20); /*΢���Աȶȵ�ֵ�������÷�Χ 0x00��0x3f*/ 
	 delay(25); 
	 transfer_command_lcd(0xa2); /*1/9 ƫѹ�ȣ�bias��*/ 
	 transfer_command_lcd(0xc8); /*��ɨ��˳�򣺴��ϵ���*/ 
	 transfer_command_lcd(0xa0); /*��ɨ��˳�򣺴�����*/ 
	 transfer_command_lcd(0x40); /*��ʼ�У��ӵ�һ�п�ʼ*/ 
	 transfer_command_lcd(0xaf); /*����ʾ*/
}
