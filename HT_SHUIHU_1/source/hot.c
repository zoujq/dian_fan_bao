
/*********************************************************************************************************************/
#include "include/ca51f_config.h"		
#include "include/ca51f3sfr.h"
#include "include/ca51f3xsfr.h"
#include "include/gpiodef_f3.h"
#include "include/system_clock.h"

#include "include/uart.h"
#include "include/pwm.h"
#include "include/delay.h"
#include <intrins.h>
#include "include/temperature.h"
/*********************************************************************************************************************
	本例程设置PWM0输出频率为1KHZ、占空比50%的PWM信号
*********************************************************************************************************************/
#define PWMDIV_V				(3686400/500)		//当PWM时钟为其他时钟频率时，需相应修改参数
#define PWMDUT_V				PWMDIV_V/2			//占空比为50%

char hot_status=0;

void init_pwm()
{
	
	CKCON |= IHCKE;					//打开IRCH时钟
	
	P15F = P15_PWM0_SETTING;								//设置P15为PWM引脚功能

	INDEX = PWM_CH0;												//设置INDEX值对应PWM0
	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | CKS_IH ;		  //设置PWM时钟源为IRCH  
// 	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | CKS_IL ;		  //设置PWM时钟源为IRCL
// 	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | CKS_XL ;		  //设置PWM时钟源为XOSCL  
// 	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | CKS_TF ;		  //设置PWM时钟源为TFRC 
// 	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | CKS_PLL ;		//设置PWM时钟源为PLL  
	PWMCFG = TOG(0) | 100;																								
	
	//设置PWMDIV、PWMDUT
	PWMDIVH	= (unsigned char)(PWMDIV_V>>8);			
	PWMDIVL	= (unsigned char)(PWMDIV_V);			
//	PWMDUTH	=	(unsigned char)(PWMDUT_V>>8);		
//	PWMDUTL	= (unsigned char)(PWMDUT_V);	

// 	PWMUPD = (1<<PWM_CH0);									//PWMDIV、PWMDUT更新使能
//	while(PWMUPD);													//等待更新完成
// 	PWMEN = (1<<PWM_CH0);										//PWM0使能
//	

}
void set_hot_power(unsigned char n)
{		
	static unsigned char p=1;
	if(p!=n)
	{
		PWMDUTH	=	(unsigned char)(PWMDIV_V*n/100>>8);		
		PWMDUTL	= (unsigned char)(PWMDIV_V*n/100);	

		PWMUPD = (1<<PWM_CH0);									//PWMDIV、PWMDUT更新使能
		while(PWMUPD);													//等待更新完成
		PWMEN = (1<<PWM_CH0);										//PWM0使能
		p=n;
	}
	
}	


void check_hot()
{
	unsigned int current_temp=get_temp();
	char jia_re_wen_du=get_jia_re_wen_du();
	char jia_re_status= get_jia_re_status();
	char bao_wen_status= get_bao_wen_status();
	
	if(jia_re_status || bao_wen_status)
	{
		if(jia_re_wen_du>current_temp)
		{
			if(jia_re_wen_du-current_temp>10)
			{
				set_hot_power(100);
			}
			else if(jia_re_wen_du-current_temp>5)
			{
				set_hot_power(30);
			}
			else 
			{
				if(jia_re_wen_du>90 || bao_wen_status>90 )
				{
					set_hot_power(30);
				}
				else
				{
					set_hot_power(10);
				}
				
			}
			hot_status=1;
		}
		else
		{
			guan_bi_jia_re();
			hot_status=0;
			set_hot_power(0);
		}
		
	}
	else
	{
		set_hot_power(0);
	}
	

}


