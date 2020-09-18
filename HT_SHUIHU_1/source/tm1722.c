 /*****************************************************************************
 *版权信息：深圳天微电子有限公司
 *文 件 名：TM1722-V1.0
 *当前版本：V1.0
 *MCU 型号：STC12C5608AD
 *开发环境：Keil uVision4
 *晶震频率：11.0592MHZ       
 *完成日期：2013-08-20
 *程序功能：1.LCD 驱动：6位段式LCD显示0~5。
            2.PWM功能 ：上电后PWM0 PWM2亮，PWM3灭，具体LED接法请参考规格书。
 *免责声明：1.此程序为TM1722驱动LCD演示程序，仅作参考之用。
            2.如有直接使用本例程程序造成经济损失的，本公司不承担任何责任。             
********************************************************************************/
#include "include/ca51f_config.h"		
#include "include/ca51f3sfr.h"
#include "include/ca51f3xsfr.h"
#include "include/gpiodef_f3.h"
#include "include/system_clock.h"

#include "include/uart.h"
#include "include/delay.h"
#include <intrins.h>

#include "include/tm1722.h"

#include "intrins.h"


#define uchar unsigned char						  //数据类型宏定义
#define uint unsigned int						  //数据类型宏定义

#define nop  _nop_();_nop_();_nop_();			                  //宏定义

/********************定义控制端口**********************/
#define DIO P10
#define CLK P01
#define STB P30
#define LCD_BACKLIGHT P34

/********************定义数据*************************/

//uchar code Tab0[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //清屏
//uchar code number0_9[16]={0xeb,0x60,0xc7,0xE5,0x6C,0xAD,0xaf,0xE0,0xef,0xed,0xee,0x2f,0x8b,0x67,0x8f,0x8e};  //0~F字型码

uchar Buff[20]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/***************延时函数nms**************/
void delay_nms(unsigned int n)
{
  Delay_ms(n);
}

void lcd_io_init()
{
	P10F = OUTPUT|PU_EN;    //P01设置为输出模式并使能上拉电阻
	P10C &= ~0x20;					//上拉电阻选择为弱上拉
	
	P01F = OUTPUT|PU_EN;    //P01设置为输出模式并使能上拉电阻
	P01C &= ~0x20;					//上拉电阻选择为弱上拉
	
	P30F = OUTPUT|PU_EN;    //P02设置为输出模式并使能上拉电阻
	P30C &= ~0x20;					//上拉电阻选择为弱上拉
	
	P34F = OUTPUT|OP_EN;    //开漏输出
	LCD_BACKLIGHT=0;					//上拉电阻选择为弱上拉
	
	disp_init();
}

/***************发送8bit数据，从低位开始**************/
void write_8bit(uchar dat)
{
uchar i;
for(i=0;i<8;i++)	 //先写低位
{
  CLK=0;
  if(dat&0x01) 
     DIO=1;
  else 
     DIO=0;
	 nop;
  CLK=1;
  dat>>=1;
}
CLK=0;
DIO=0;
}


/******************发送控制命令***********************/
void write_command(uchar Cmd)
{
 STB=1;
 nop;
 STB=0;
 write_8bit(Cmd);
}

/****************显示函数，LCD显示*****************/

void disp_init()
{
 write_command(0x00);   //工作模式设置：开启SEG
 write_command(0x44);   //数据设置：写数据到显示寄存器，采用固定地址 
 write_command(0x97);   //显示控制设置：显示开，1/3偏压，LCD电压通过改变低三位调节

 write_command(0xca);   //向02H写数据，显示0
 write_8bit(0x00);
 write_command(0xcb);   //向03H写数据，显示1
 write_8bit(0x00);
 write_command(0xce);   //向06H写数据，显示2
 write_8bit(0x00);
	
 STB=1;
}

void refresh_screen()
{
	write_command(0xca);   //
	write_8bit(Buff[0]|(Buff[1]<<1)|(Buff[2]<<2)|(Buff[3]<<3));
	write_command(0xcb);   //
	write_8bit(Buff[4]|(Buff[5]<<1)|(Buff[6]<<2)|(Buff[7]<<3)|(Buff[8]<<4)|(Buff[9]<<5)|(Buff[10]<<6)|(Buff[11]<<7));
	write_command(0xce);   //
	write_8bit(Buff[12]|(Buff[13]<<1)|(Buff[14]<<2)|(Buff[15]<<3)|(Buff[16]<<4)|(Buff[17]<<5)|(Buff[18]<<6)|(Buff[19]<<7));
}

//////////////显示数字////////////////////////
///position=1,正数第一位。position=2,正数第二位。position=3,正数第三位。=4 关温度显示。=5 不显示温度
void show_number(uchar position ,uchar n)
{
	if(position==1)
	{
		if(n==1)
		{
			Buff[11]=1;
		}
		else
		{
			Buff[11]=0;
		}		
		
	}
	else if(position==2)
	{
		switch(n)
		{
			case 0:
				Buff[9]=0;Buff[12]=1;Buff[13]=1;Buff[14]=1;Buff[15]=1;Buff[10]=1;Buff[8]=1;
				break;
			case 1:
				Buff[9]=0;Buff[12]=0;Buff[13]=0;Buff[14]=0;Buff[15]=0;Buff[10]=1;Buff[8]=1;
				break;
			case 2:
				Buff[9]=1;Buff[12]=1;Buff[13]=0;Buff[14]=1;Buff[15]=1;Buff[10]=0;Buff[8]=1;
				break;
			case 3:
				Buff[9]=1;Buff[12]=1;Buff[13]=0;Buff[14]=0;Buff[15]=1;Buff[10]=1;Buff[8]=1;
				break;
			case 4:
				Buff[9]=1;Buff[12]=0;Buff[13]=1;Buff[14]=0;Buff[15]=0;Buff[10]=1;Buff[8]=1;
				break;
			case 5:
				Buff[9]=1;Buff[12]=1;Buff[13]=1;Buff[14]=0;Buff[15]=1;Buff[10]=1;Buff[8]=0;
				break;
			case 6:
				Buff[9]=1;Buff[12]=1;Buff[13]=1;Buff[14]=1;Buff[15]=1;Buff[10]=1;Buff[8]=0;
				break;
			case 7:
				Buff[9]=0;Buff[12]=1;Buff[13]=0;Buff[14]=0;Buff[15]=0;Buff[10]=1;Buff[8]=1;
				break;
			case 8:
				Buff[9]=1;Buff[12]=1;Buff[13]=1;Buff[14]=1;Buff[15]=1;Buff[10]=1;Buff[8]=1;
				break;
			case 9:
				Buff[9]=1;Buff[12]=1;Buff[13]=1;Buff[14]=0;Buff[15]=1;Buff[10]=1;Buff[8]=1;
				break;
			default:
				break;
		}
	}
	else if(position==3)
	{
		switch(n)
		{
			case 0:
				Buff[1]=0;Buff[4]=1;Buff[5]=1;Buff[6]=1;Buff[7]=1;Buff[2]=1;Buff[0]=1;
				break;
			case 1:
				Buff[1]=0;Buff[4]=0;Buff[5]=0;Buff[6]=0;Buff[7]=0;Buff[2]=1;Buff[0]=1;
				break;
			case 2:
				Buff[1]=1;Buff[4]=1;Buff[5]=0;Buff[6]=1;Buff[7]=1;Buff[2]=0;Buff[0]=1;
				break;
			case 3:
				Buff[1]=1;Buff[4]=1;Buff[5]=0;Buff[6]=0;Buff[7]=1;Buff[2]=1;Buff[0]=1;
				break;
			case 4:
				Buff[1]=1;Buff[4]=0;Buff[5]=1;Buff[6]=0;Buff[7]=0;Buff[2]=1;Buff[0]=1;
				break;
			case 5:
				Buff[1]=1;Buff[4]=1;Buff[5]=1;Buff[6]=0;Buff[7]=1;Buff[2]=1;Buff[0]=0;
				break;
			case 6:
				Buff[1]=1;Buff[4]=1;Buff[5]=1;Buff[6]=1;Buff[7]=1;Buff[2]=1;Buff[0]=0;
				break;
			case 7:
				Buff[1]=0;Buff[4]=1;Buff[5]=0;Buff[6]=0;Buff[7]=0;Buff[2]=1;Buff[0]=1;
				break;
			case 8:
				Buff[1]=1;Buff[4]=1;Buff[5]=1;Buff[6]=1;Buff[7]=1;Buff[2]=1;Buff[0]=1;
				break;
			case 9:
				Buff[1]=1;Buff[4]=1;Buff[5]=1;Buff[6]=0;Buff[7]=1;Buff[2]=1;Buff[0]=1;
				break;
			default:
				break;
		}
	}
	else if(position==4)
	{
		Buff[11]=0;
		Buff[9]=0;Buff[12]=0;Buff[13]=0;Buff[14]=0;Buff[15]=0;Buff[10]=0;Buff[8]=0;
		Buff[1]=0;Buff[4]=0;Buff[5]=0;Buff[6]=0;Buff[7]=0;Buff[2]=0;Buff[0]=0;
	}
	else if(position==5)
	{
		Buff[11]=0;
		Buff[9]=1;Buff[12]=0;Buff[13]=0;Buff[14]=0;Buff[15]=0;Buff[10]=0;Buff[8]=0;
		Buff[1]=1;Buff[4]=0;Buff[5]=0;Buff[6]=0;Buff[7]=0;Buff[2]=0;Buff[0]=0;
	}
	else
	{
		
	}
	

}
///////////////////显示图标///////////////
//icon=1-温度，2-奶瓶，3-煮面，4-温度单位，5-火苗。on_off=0,1
void set_icon(uchar icon ,uchar on_off)
{
	switch(icon)
	{
		case 1:
			Buff[18]=on_off;
		
			break;
		case 2:
			Buff[17]=on_off;
			break;
		case 3:
			Buff[16]=on_off;
			break;
		case 4:
			Buff[3]=on_off;
			break;
		case 5:
			Buff[19]=on_off;
			break;
		default:
			break;
	
	}
		
	
}
void show_temp(char temp)
{
	if(temp>100)
	{
		temp=100;
	}
	if(temp==100)
	{
		show_number(1,1);
		show_number(2,0);
		show_number(3,0);
	}
	else if(temp<=10)
	{
		show_number(1,0);
		show_number(5,0);
		show_number(5,0);
	}	
	else 
	{
		show_number(1,0);
		show_number(2,temp/10);
		show_number(3,temp%10);
	}
		
	set_icon(4,1);
	
}
void un_diplay_temp()
{
	show_number(4,0);
	set_icon(4,0);
}
void close_display()
{
	LCD_BACKLIGHT=1;
	disp_init();
}
void open_display()
{
	LCD_BACKLIGHT=0;
	show_temp(0);
}
void display_icon_wendu()
{
	set_icon(1,1);
}
void un_display_icon_wendu()
{
	set_icon(1,0);
}
void display_icon_huomiao()
{
	set_icon(5,1);
}
void un_display_icon_huomiao()
{
	set_icon(5,0);
}

