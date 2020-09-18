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

#define KEY P00
#define SPEAKER P31
#define XUAN_NIU_KEY P06
#define XUAN_NIU_ZHENG P07
#define XUAN_NIU_NI P35

char system_status=1;
char bao_wen_status=0;
char jia_re_status=0;
char jia_re_wen_du=100;
char xuan_niu_ing=0;
char xuan_niu_deng_dai=0;
void init_keys_gpio()
{
		P00F = INPUT|PU_EN;    //KEY
  	P00C &= ~0x20;	
	
		P31F = OUTPUT|PD_EN;   //SPEAKER
		P31C &= ~0x10;					
	
		P06F = INPUT|PU_EN;    //XUAN_NIU_KEY
  	P06C &= ~0x20;	
	
		//XUAN_NIU_ZHENG
		P07F = 1; //设置 P07 为输入引脚   
		IT0CON = 7; //选择 P07 作为中断 0 引脚，7 为 P07 对应的索引编号
		EX0 = 1; //INT0 中断使能
		IE0 = 1; //外部中断 0 使能
		IT0 = 1; //设置为下降沿中断
		PX0 = 1; //设置 INT0 为高优先级
	
//		//XUAN_NIU_NI

		P35F = INPUT; //设置 P35 为输入引脚
		IT1CON = 29; //选择 P35 作为中断 35 引脚，29 为 P35 对应的索引编号
		EX1 = 1; //INT1 中断使能
		IE1 = 1; //外部中断 1 使能
		IT1 = 1; //设置为下降沿中断
		PX1 = 1; //设置 INT1 为高优先级
		
		SPEAKER=0;
}
unsigned char get_keys()
{
	return KEY;
	
}
void On_keys()
{
	static char lask_key=1;
	static char times=0;
	if(KEY==0)
	{
		if(lask_key==1)
		{
			if(system_status==1)
			{
				set_speaker();
			}			
			lask_key=0;
		}
		times++;
		if(times==30)
		{
			if(system_status==1)
			{
				system_status=0;
				close_display();
				jia_re_status=0;
				un_display_icon_wendu();
			}
			else
			{
				system_status=1;
				open_display();
				set_speaker();

			}
		}
		if(times>30)
		{
			times=31;
		}
	}
	else 
	{	
		lask_key=1;
		times=0;
	
	}
	
}
void On_xuan_niu_key()
{
	static char lask_key=1;
	static char times=0;
	if(system_status==1)
	{
		if(XUAN_NIU_KEY==0)
		{
			if(lask_key==1)
			{			
				set_speaker();		
				lask_key=0;
			}
			times++;
			if(times==30)
			{			
				if(bao_wen_status==1)
				{
					un_display_icon_wendu();
					set_speaker();
					bao_wen_status=0;
				}
				else
				{
					display_icon_wendu();
					set_speaker();
					bao_wen_status=1;
				}				
			}
			if(times>30)
			{
				times=31;
			}
		}
		else 
		{	
			if(lask_key==0 && times< 20 )
			{
				if(jia_re_status==0)
				{
					jia_re_status=1;
					display_icon_huomiao();
				}
				else
				{
					jia_re_status=0;
					un_display_icon_huomiao();
				}
				
			}
			lask_key=1;
			times=0;
		
		}
	}

}
void INT0_ISR (void) interrupt 0
{
//外部中断 0 中断服务程序
	if(system_status==1 && xuan_niu_deng_dai==0)
	{		
		if(XUAN_NIU_ZHENG==0 && XUAN_NIU_NI==1)
		{	
			Delay_ms(10);
			if(XUAN_NIU_ZHENG==0 && XUAN_NIU_NI==1)
			{	
				jia_re_wen_du++;
				if(jia_re_wen_du>100)
				{
					jia_re_wen_du=100;
				}
			}
		}
		xuan_niu_deng_dai=1;
	}

}
void INT1_ISR (void) interrupt 2
{
	//外部中断 1 中断服务程序
	if(system_status==1 && xuan_niu_deng_dai==0)
	{		
		if(XUAN_NIU_ZHENG==1 && XUAN_NIU_NI==0)
		{		
			Delay_ms(10);
			if(XUAN_NIU_ZHENG==1 && XUAN_NIU_NI==0)
			{	
				jia_re_wen_du--;
				if(jia_re_wen_du<11)
				{
					jia_re_wen_du=11;
				}		
			}
		}
		xuan_niu_deng_dai=1;
	}

}
void On_xuan_niu()
{
	static unsigned int last_jia_re_wen_du=100;
	static int times=0;
	static char setting=0;

	if(jia_re_wen_du!=last_jia_re_wen_du || xuan_niu_deng_dai==1)
	{			
		last_jia_re_wen_du=jia_re_wen_du;
		show_temp(jia_re_wen_du);
		times=0;
		setting=1;
		xuan_niu_ing=1;
	}
	else 
	{	
		if(setting==1)
		{		
			times++;				
			if(times==25)
			{
				un_diplay_temp();
			}
			else if(times==30)
			{
				show_temp(jia_re_wen_du);
			}
			else if(times==35)
			{
				un_diplay_temp();
			}
			else if(times==40)
			{
				show_temp(jia_re_wen_du);
			}
			else if(times==45)
			{
				un_diplay_temp();
			}
			else if(times==50)
			{
				show_temp(jia_re_wen_du);					
				set_speaker();
				setting=0;		
				times=0;		
				jia_re_status=1;
				display_icon_huomiao();	
				xuan_niu_ing=0;					
			}
		}
		
	}
	xuan_niu_deng_dai=0;
}
void set_speaker()
{
	long i=0;

	for(i=0;i<300;i++)
	{
		SPEAKER=1;
		Delay_50us(3);
		SPEAKER=0;
		Delay_50us(3);
	}
}
char get_xuan_niu_ing()
{
	return xuan_niu_ing;

}
char get_jia_re_wen_du()
{
	return jia_re_wen_du;
}	
char get_jia_re_status()
{
	return jia_re_status;
}	

char get_bao_wen_status()
{
	return bao_wen_status;
}	

void guan_bi_jia_re()
{
	jia_re_status=0;
	un_display_icon_huomiao();
}





