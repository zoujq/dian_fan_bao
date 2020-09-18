 /*****************************************************************************
 *��Ȩ��Ϣ��������΢�������޹�˾
 *�� �� ����TM1722-V1.0
 *��ǰ�汾��V1.0
 *MCU �ͺţ�STC12C5608AD
 *����������Keil uVision4
 *����Ƶ�ʣ�11.0592MHZ       
 *������ڣ�2013-08-20
 *�����ܣ�1.LCD ������6λ��ʽLCD��ʾ0~5��
            2.PWM���� ���ϵ��PWM0 PWM2����PWM3�𣬾���LED�ӷ���ο�����顣
 *����������1.�˳���ΪTM1722����LCD��ʾ���򣬽����ο�֮�á�
            2.����ֱ��ʹ�ñ����̳�����ɾ�����ʧ�ģ�����˾���е��κ����Ρ�             
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


#define uchar unsigned char						  //�������ͺ궨��
#define uint unsigned int						  //�������ͺ궨��

#define nop  _nop_();_nop_();_nop_();			                  //�궨��

/********************������ƶ˿�**********************/
#define DIO P10
#define CLK P01
#define STB P30
#define LCD_BACKLIGHT P34

/********************��������*************************/

//uchar code Tab0[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //����
//uchar code number0_9[16]={0xeb,0x60,0xc7,0xE5,0x6C,0xAD,0xaf,0xE0,0xef,0xed,0xee,0x2f,0x8b,0x67,0x8f,0x8e};  //0~F������

uchar Buff[20]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/***************��ʱ����nms**************/
void delay_nms(unsigned int n)
{
  Delay_ms(n);
}

void lcd_io_init()
{
	P10F = OUTPUT|PU_EN;    //P01����Ϊ���ģʽ��ʹ����������
	P10C &= ~0x20;					//��������ѡ��Ϊ������
	
	P01F = OUTPUT|PU_EN;    //P01����Ϊ���ģʽ��ʹ����������
	P01C &= ~0x20;					//��������ѡ��Ϊ������
	
	P30F = OUTPUT|PU_EN;    //P02����Ϊ���ģʽ��ʹ����������
	P30C &= ~0x20;					//��������ѡ��Ϊ������
	
	P34F = OUTPUT|OP_EN;    //��©���
	LCD_BACKLIGHT=0;					//��������ѡ��Ϊ������
	
	disp_init();
}

/***************����8bit���ݣ��ӵ�λ��ʼ**************/
void write_8bit(uchar dat)
{
uchar i;
for(i=0;i<8;i++)	 //��д��λ
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


/******************���Ϳ�������***********************/
void write_command(uchar Cmd)
{
 STB=1;
 nop;
 STB=0;
 write_8bit(Cmd);
}

/****************��ʾ������LCD��ʾ*****************/

void disp_init()
{
 write_command(0x00);   //����ģʽ���ã�����SEG
 write_command(0x44);   //�������ã�д���ݵ���ʾ�Ĵ��������ù̶���ַ 
 write_command(0x97);   //��ʾ�������ã���ʾ����1/3ƫѹ��LCD��ѹͨ���ı����λ����

 write_command(0xca);   //��02Hд���ݣ���ʾ0
 write_8bit(0x00);
 write_command(0xcb);   //��03Hд���ݣ���ʾ1
 write_8bit(0x00);
 write_command(0xce);   //��06Hд���ݣ���ʾ2
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

//////////////��ʾ����////////////////////////
///position=1,������һλ��position=2,�����ڶ�λ��position=3,��������λ��=4 ���¶���ʾ��=5 ����ʾ�¶�
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
///////////////////��ʾͼ��///////////////
//icon=1-�¶ȣ�2-��ƿ��3-���棬4-�¶ȵ�λ��5-���硣on_off=0,1
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

