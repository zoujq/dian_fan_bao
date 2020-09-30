
#ifndef _MAIN_C_
#define _MAIN_C_
/*********************************************************************************************************************/
#include "include/ca51f_config.h"		
#include "include/ca51f3sfr.h"
#include "include/ca51f3xsfr.h"
#include "include/gpiodef_f3.h"
#include "include/system_clock.h"

#include "include/uart.h"
#include "include/delay.h"
#include <intrins.h>

#include "include/tm1722.h"
#include "include/keys.h"
#include "include/hot.h"
#include "include/temperature.h"

/*********************************************************************************************************************
	本例程以P00为例，设置P00为不同模式，所有IO用法与P00相同。
*********************************************************************************************************************/

struct {
	char shi_pu_num;
	int work_time;
	int success_time;
	char is_bao_wen;
	char state;
	
}device_info;

#define BAO_WEN_LIGHT P34
#define PENG_REN_LIGHT P07
#define DEVICE_SWITCH P06

void set_device_info(char state,char shi_pu_num,int work_time,int wait_time,char is_bao_wen);
void key_check()
{
	static char lask_key=-1;
	
	if(lask_key!=P06)
	{
		lask_key=P06;
		if(lask_key==1)
		{
			set_device_info(1,1,25,0,1);
		}
		else
		{
			if(device_info.success_time==0 && device_info.state==1)
			{
				set_device_info(3,1,25,480,1);
			}
			else
			{
				set_device_info(0,0,0,0,0);
			}
			
		}

	}
}
void led_light_set()
{
	static char last_state=-1;
	if(last_state!=device_info.state)
	{
		last_state=device_info.state;
		if(last_state==0)
		{
			BAO_WEN_LIGHT=1;//为1关灯
			PENG_REN_LIGHT=1;//为1关灯
		}
		else if(last_state==1)
		{
			BAO_WEN_LIGHT=1;//为1关灯
			PENG_REN_LIGHT=0;//为1关灯
		}
		else if(last_state==2)
		{
			BAO_WEN_LIGHT=1;//为1关灯
			PENG_REN_LIGHT=1;//为1关灯
		}
		else if(last_state==3)
		{
			BAO_WEN_LIGHT=0;//为1关灯
			PENG_REN_LIGHT=1;//为1关灯
		}
	}
	
}

void ble_check()
{
	if(uart0_rev.head>0)
	{
		Delay_ms(10);
		if(uart0_rx_buf[1]==0xC8)
		{
			if(uart0_rx_buf[2]==1)
			{
				char state;
				char shi_pu_num;
				int work_time;
				int wait_time;
				char is_bao_wen;

				shi_pu_num=uart0_rx_buf[3];
				work_time=(uart0_rx_buf[4]<<8) + uart0_rx_buf[5];
				wait_time=(uart0_rx_buf[6]<<8) + uart0_rx_buf[7];
				is_bao_wen=uart0_rx_buf[8];
				if(wait_time==0)
				{
					state=1;
				}
				else{
					state=2;
				}
				set_device_info(state,shi_pu_num,work_time,wait_time,is_bao_wen);
			}
			else if(uart0_rx_buf[2]==2)
			{
				Uart0_PutChar(0xC8);
				Uart0_PutChar(2);
				Uart0_PutChar(device_info.state);
				Uart0_PutChar(device_info.shi_pu_num);
				Uart0_PutChar(device_info.success_time);
				Uart0_PutChar(device_info.is_bao_wen);
				Uart0_PutChar(0);

			}
			else if(uart0_rx_buf[2]==3)
			{
				set_device_info(0,0,0,0,0);
			}
		}
		uart0_rev.head=0;
	}		
	
}

void set_device_info(char state,char shi_pu_num,int work_time,int wait_time,char is_bao_wen)
{
	device_info.state=state;
	device_info.shi_pu_num=shi_pu_num;
	device_info.work_time=work_time;
	device_info.success_time=wait_time+work_time;
	device_info.is_bao_wen=is_bao_wen;
}
void check_hot()
{
	static char last_state=-1;
	if(device_info.success_time>0)
	{
		if(device_info.state==1)
		{
			set_hot_power(100);
		}
		else if(device_info.state==2)
		{
			if(device_info.work_time>device_info.success_time)
			{
				device_info.state=1;
			}			
			set_hot_power(0);
		}
		else if(device_info.state==3)
		{
			set_hot_power(20);
		}
		else
		{
			set_hot_power(0);
			set_device_info(0,0,0,0,0);
		}
		
	}
	else
	{
		if(device_info.state==1)
		{
			if(device_info.is_bao_wen==1)
			{
				set_device_info(3,0,0,480,0);
			}
		}
		else
		{
			set_device_info(0,0,0,0,0);
		}

	}
	

}
void main(void)
{
	int t0=0;
	int i=0;
#if (SYSCLK_SRC == PLL)
	Sys_Clk_Set_PLL(PLL_Multiple);	//设置系统时钟为PLL，PLL_Multiple为倍频倍数
#endif
	
#ifdef UART0_EN
	Uart0_Initial(UART0_BAUTRATE);	//初始化UART0
#endif
	
#ifdef UART1_EN
	Uart1_Initial(UART1_BAUTRATE);	//初始化UART1
#endif

	EA = 1;													//开全局中断

#ifdef PRINT_EN
	uart_printf("GPIO Demo Code\n");
#endif

/**************************************************************************************************/
	Uart0_Initial(UART0_BAUTRATE);
	
	P34F = OUTPUT|OP_EN;    //P34保温灯
	P07F = OUTPUT|OP_EN;    //P07煮饭灯
	

	P16F = P16_ADC6_SETTING;// NTC
	P06F =INPUT | PD_EN; //检测煮饭switch

	P06C &= ~0x10;//弱下拉
	
	//	P15F = OUTPUT|PD_EN; //HOT 加热输出
	//	P15C &= ~0x10;//弱下拉
	init_pwm();//P15为加热输出
	set_device_info(0,0,0,0,0);
	while(1)
	{
		t0++;
		key_check();
		ble_check();
		led_light_set();
		if(t0%3000==0)
		{
			check_hot();
		}	
		Delay_ms(1);
	}
/***********************************************************************************/		


}

#endif
























