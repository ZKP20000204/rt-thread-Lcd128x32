#include <reg52.h>
#include <intrins.h>
#include <test.h>
//***************************************************
#define uchar unsigned char
#define uint  unsigned int
//***************************************************
// P/S=0,C86=1,serial interface,4SPI
sbit A0	 	=P3^7;
//sbit WR_RW=P3^6;
//sbit RD_E	=P3^5;        
sbit RST 	=P3^4;
sbit CSB 	=P3^3;

sbit SDA	=P1^0;//D1~D3
sbit SCL	=P1^1;//D0
//***************************************************
void lcd_initial(void);
void wr_cmd(uchar cmd);
void wr_data(uchar wrdata);
void Clear_DDRAM(void);
void display_all(void);
void display_bmp(uchar *s);
void delay_ms(uint ms_value);
void delay_us(uchar us_value);
//***************************************************
#define Bmp_Max 3

uchar code seg=128;
uchar code com=32;
uchar contrast=31;  //7.6V	//initialize the contrast
//***************************************************


void main()
{
	lcd_initial();
	delay_ms(50);
	while(1)
	{
		display_all();
		delay_ms(2000);
		Clear_DDRAM();
		delay_ms(2000);
		display_bmp(test);
		delay_ms(2000);
	}
}

//*******************************************************************************
void lcd_initial()
{
	CSB=0;			  //Enable the IC
	delay_ms(30);       //Waiting for the chip select signal is stabilized 
	RST=0;			  //Reset
	delay_ms(10);       //Waiting for the power is stabilized
	RST=1;			  //Release the reset state
	delay_ms(10);       //Waiting reset circuit stabilized

	wr_cmd(0xae);       //display off
	wr_cmd(0xa2);		  //Set 1/6 bias
	wr_cmd(0xa0);       //set seg0---seg131,adc normal
	wr_cmd(0xc8);       //set com63---com0,Common output mode select
	wr_cmd(0x40);       //display start line
	wr_cmd(0xa6);       //display normal

	wr_cmd(0x23);		  //IRS=1,set (1+Rb/Ra)=4.5
	wr_cmd(0x81);       //Enable the Electronic Volume Mode Set
	wr_cmd(31);   //Electronic volume register set(contrast:0~63),contrast=31,VOP=4.5*(99+31)/162*2.1=7.58V

	wr_cmd(0xf8);       //Enable The Booster Ratio
	wr_cmd(0x00);       //4x

	wr_cmd(0x2c);	      //Booster circuit:ON,Voltage regulator circuit:OFF,Voltage follower circuit:OFF
	delay_ms(100);
	wr_cmd(0x2e);	      //Booster circuit:ON,Voltage regulator circuit:ON,Voltage follower circuit:OFF
	delay_ms(100);
	wr_cmd(0x2f);		  //Booster circuit:ON,Voltage regulator circuit:ON,Voltage follower circuit:ON
	delay_ms(100);

	Clear_DDRAM();      //Clear display
	delay_ms(10);
	wr_cmd(0xaf);       //display on
}

/*****************************************************/
void wr_cmd(uchar cmd) 
{
	uchar i;
// 	PSB=1;	
// 	C86=0;	
	CSB=0;
	A0=0;
	SCL=1;
	for(i=0;i<8;i++)
	{
		if((cmd&0x80)==0x80)SDA=1;
		else SDA=0;
		SCL=0;
		SCL=1;
		cmd=cmd<<1;
	}
	CSB=1;
}

void wr_data(uchar dat)
{
	uchar j;
// 	PSB=1;	
// 	C86=0;
	CSB=0;
	A0=1;
	SCL=1;
	for(j=0;j<8;j++)
	{
		if((dat&0x80)==0x80)SDA=1;
		else SDA=0;
		SCL=0;
		SCL=1;
		dat=dat<<1;
	}
	CSB=1;
}


//********************************************
void Clear_DDRAM(void)
{
	uint i,j;
	for(i=0;i<com/8;i++)//Set the display all points off
	{
		wr_cmd(0xb0+i);  //set  page address
		wr_cmd(0x10);    //Set the 4 high bits of column address,seg127--seg0
		wr_cmd(0x00);    //Set the 4 low bits of column address
		for(j=0;j<seg;j++)
		{
			wr_data(0x00); //Clear display
		}
	}
}


//********************************************
void display_all(void)
{
      uchar i,j;
      for(i=0;i<com/8;i++)
	  {
	        wr_cmd(0xb0+i);   //Set  page address
			wr_cmd(0x10);	  //Set the 4 high bits of column address
	        wr_cmd(0x00);     //Set the 4 low bits of column address  
	        for(j=0;j<seg;j++)
			{
			      wr_data(0xff);    				 
			}
	  }
}

//********************************************
void display_bmp(uchar *s)
{
      uchar i,j;
      for(i=0;i<com/8;i++)
	  {
	        wr_cmd(0xb0+i);     //set the address of last page 
			wr_cmd(0x10);  		//Set the 4 high bits of column address
	        wr_cmd(0x00);       //Set the 4 high bits of column address  
	        for(j=0;j<seg;j++)
			{
			       wr_data(*s);
				   s++;
			}
	  }
}

//========================================================================
void delay_us(uchar us_value)	//delay about 22 us
{
	while(us_value--)
	{
		_nop_();   
		_nop_();
		_nop_();
	}
}
//========================================================================
void delay_ms(uint ms_value)	//delay about 1 ms
{
	uint k;
	for(k=0;k<ms_value;k++)
	{
		delay_us(90);
	}
}