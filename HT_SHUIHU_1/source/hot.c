
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
	����������PWM0���Ƶ��Ϊ1KHZ��ռ�ձ�50%��PWM�ź�
*********************************************************************************************************************/
#define PWMDIV_V				(3686400/500)		//��PWMʱ��Ϊ����ʱ��Ƶ��ʱ������Ӧ�޸Ĳ���
#define PWMDUT_V				PWMDIV_V/2			//ռ�ձ�Ϊ50%

char hot_status=0;

void init_pwm()
{
	
	CKCON |= IHCKE;					//��IRCHʱ��
	
	P15F = P15_PWM0_SETTING;								//����P15ΪPWM���Ź���

	INDEX = PWM_CH0;												//����INDEXֵ��ӦPWM0
	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | CKS_IH ;		  //����PWMʱ��ԴΪIRCH  
// 	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | CKS_IL ;		  //����PWMʱ��ԴΪIRCL
// 	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | CKS_XL ;		  //����PWMʱ��ԴΪXOSCL  
// 	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | CKS_TF ;		  //����PWMʱ��ԴΪTFRC 
// 	PWMCON = TIE(0) | ZIE(0) | PIE(0) | NIE(0) | MS(0) | CKS_PLL ;		//����PWMʱ��ԴΪPLL  
	PWMCFG = TOG(0) | 100;																								
	
	//����PWMDIV��PWMDUT
	PWMDIVH	= (unsigned char)(PWMDIV_V>>8);			
	PWMDIVL	= (unsigned char)(PWMDIV_V);			
//	PWMDUTH	=	(unsigned char)(PWMDUT_V>>8);		
//	PWMDUTL	= (unsigned char)(PWMDUT_V);	

// 	PWMUPD = (1<<PWM_CH0);									//PWMDIV��PWMDUT����ʹ��
//	while(PWMUPD);													//�ȴ��������
// 	PWMEN = (1<<PWM_CH0);										//PWM0ʹ��
//	

}
void set_hot_power(unsigned char n)
{		
	static unsigned char p=1;
	if(p!=n)
	{
		PWMDUTH	=	(unsigned char)(PWMDIV_V*n/100>>8);		
		PWMDUTL	= (unsigned char)(PWMDIV_V*n/100);	

		PWMUPD = (1<<PWM_CH0);									//PWMDIV��PWMDUT����ʹ��
		while(PWMUPD);													//�ȴ��������
		PWMEN = (1<<PWM_CH0);										//PWM0ʹ��
		p=n;
	}
	
}	





