/*********************************************************************************************************************/
#include "include/ca51f_config.h"		
#include "include/ca51f3sfr.h"
#include "include/ca51f3xsfr.h"
#include "include/gpiodef_f3.h"
#include "include/system_clock.h"

#include "include/uart.h"
#include "include/adc.h"
#include "include/delay.h"
#include <intrins.h>

#include "include/tm1722.h"
#include "include/keys.h"
#include "include/hot.h"
#include "include/temperature.h"
/*********************************************************************************************************************/
unsigned int current_temp=0;


unsigned char read_inner_trim(void)				
{
	unsigned char value;
	FSCMD = 0x80;	
	PTSH  = 0x00;				
	PTSL  = 0x24;      
	FSCMD = 0x81;						
	value = FSDAT;
	FSCMD = 0;
	return (value&0x3F);
}


void init_temp_adc()
{

/**********设置ADC引脚功能*************************************************************/
//	P10F = P10_ADC0_SETTING;	//设置P10为ADC引脚功能
// 	P11F = P11_ADC1_SETTING;	//设置P11为ADC引脚功能
// 	P12F = P12_ADC2_SETTING;	//设置P12为ADC引脚功能
// 	P13F = P13_ADC3_SETTING;	//设置P13为ADC引脚功能
// 	P14F = P14_ADC4_SETTING;	//设置P14为ADC引脚功能
// 	P15F = P15_ADC5_SETTING;	//设置P15为ADC引脚功能
 	P16F = P16_ADC6_SETTING;	//设置P16为ADC引脚功能
// 	P17F = P17_ADC7_SETTING;	//设置P17为ADC引脚功能
	
// 		P10F = P10_ADC_VREF_SETTING;	//如果ADC参考电压选择为外部参考电压，需要设置P10为外部参考电压引脚，并且ADC0通道不能使用。
/***************************************************************************************/
	

/**********初始化ADC寄存器（设置ADC时钟、选择ADC参考电压、设置ADC运放、加载内部1.5V校准值）***********/
// 	ADCON = AST(0) | ADIE(0) | HTME(7) | VSEL(ADC_REF_INNER);		//设置ADC参考电压为内部1.5V
	ADCON = AST(0) | ADIE(0) | HTME(7) | VSEL(ADC_REF_VDD);				//设置ADC参考电压为VDD
// 	ADCON = AST(0) | ADIE(0) | HTME(7) | VSEL(ADC_REF_Outer);		//设置ADC参考电压为外部参考电压
	
	ADCFGH = AOPS(NO_AMP) | VTRIM(read_inner_trim());							//加载内部1.5V校准值
/**************************************************************************************************/


/**********设置ADC通道和时钟分频*********************************************************************/
//	ADCFGL = ACKD(7) | ADCHS(ADC_CH0);	//选择ADC0通道
// 	ADCFGL = ACKD(7) | ADCHS(ADC_CH1);	//选择ADC1通道
// 	ADCFGL = ACKD(7) | ADCHS(ADC_CH2);	//选择ADC2通道
// 	ADCFGL = ACKD(7) | ADCHS(ADC_CH3);	//选择ADC3通道
// 	ADCFGL = ACKD(7) | ADCHS(ADC_CH4);	//选择ADC4通道
// 	ADCFGL = ACKD(7) | ADCHS(ADC_CH5);	//选择ADC5通道
 	ADCFGL = ACKD(7) | ADCHS(ADC_CH6);	//选择ADC6通道
// 	ADCFGL = ACKD(7) | ADCHS(ADC_CH7);	//选择ADC7通道
/**************************************************************************************************/

}

unsigned int get_temp_adc()
{
		unsigned int AD_Value;
		ADCON |= AST(1);																//启动ADC转换
		while(!(ADCON & ADIF));													//等待ADC转换完成
		ADCON |= ADIF;																	//清除ADC中断标志
		AD_Value = ADCDH*256 + ADCDL;										//读取AD值
		AD_Value >>= 4;		
		uart_printf ("ADC Value = 0x%x\n\r",AD_Value);		//打印AD值	
		return AD_Value;
}
unsigned int code Temp_Table[90]={	
  //10    9     8     7     6      5     4     3     2     1
  740,  742,  743,  745,  750,   755,  760,  790,  820,  850,  //90-10
  880,  885,  890,  895,  900,   918,  936,  954,  972,  990,  //80
 1200, 1250,  1300, 1350, 1400, 1450, 1500, 1550, 1600, 1650,  //70
 1700, 1750,  1800, 1880, 1960, 2040, 2120, 2200, 2300, 2400,  //60
 2500, 2600,  2700, 2800, 2900, 3000, 3180, 3270, 3360, 3560,  //50
 3730, 3910,  4100, 4250, 4400, 4550, 4700, 4850, 5000, 5600,	 //40
 6200, 6300,  6400, 6500, 6850,  7120, 7580, 7960, 8340, 8710,  //30
 9100, 10400, 11700, 13000,14400,15800, 17200, 18600, 20000, 21400,  //20
 23000, 25000, 28000, 31000,35000,38000,41000,46000,51000,56000  //10


};
unsigned int zhong_zhi_lv_bo( unsigned int adcx)
{
	int i,j;
	static unsigned int adc[5]={0};
	unsigned int adc_cpy[5]={0};
	unsigned int temp=0;
		
	adc[4]=adc[3];
	adc[3]=adc[2];
	adc[2]=adc[1];
	adc[1]=adc[0];
	adc[0]=adcx;
	
	adc_cpy[0]=adc[0];
	adc_cpy[1]=adc[1];
	adc_cpy[2]=adc[2];
	adc_cpy[3]=adc[3];
	adc_cpy[4]=adc[4];
	
	for(j=0;j<4;j++)
	{
		for(i=0;i<4-j;i++)
		{
			if(adc_cpy[i]<adc_cpy[i+1])
			{
				temp=adc_cpy[i];
				adc_cpy[i]=adc_cpy[i+1];
				adc_cpy[i+1]=temp;
				
			}
		}
		
	}
	
	return adc_cpy[2];
	
}

unsigned int get_resistance(unsigned int adc6)
{
	unsigned int res=0;
	
	if(adc6>10)
	{
		res=409600/adc6 -100;
	}
	
	//uart_printf ("res Value =%d\n\r",res);	
	return res;
	
}

void check_temp()
{
	unsigned int n=0;
	unsigned int res=0;
		
	res=get_resistance(zhong_zhi_lv_bo(get_temp_adc()));
	
	if(res==0 || res<300)
	{
		current_temp=0;
		
	}
	else
	{
			for(n=0;n<90;n++)
			{
				if(Temp_Table[n]>res)
				{
					break;
				}
				
			}
				
			current_temp=100-n;
		
	}

	
}
unsigned int get_temp()
{
	
	return current_temp;
//  return 65;
	
}
void display_temp()
{
	static unsigned int last_temp=0;
	unsigned int real_temp=0;
	unsigned int jia_re_wen_du=0;
	if(get_xuan_niu_ing())
	{
		return ;
	}
	
	real_temp=get_temp();
	
	if(real_temp>last_temp)
	{
		if(real_temp-last_temp>=2)
		{
			last_temp=real_temp-1;
		}
	}
	else if(real_temp<last_temp)
	{
		if(last_temp-real_temp>=2)
		{
			last_temp=real_temp+1;
		}
	}	
	
	if(real_temp==100)
	{
		last_temp=100;
	}
	
	jia_re_wen_du=get_jia_re_wen_du();
	
	if(jia_re_wen_du>last_temp && jia_re_wen_du-last_temp<2)
	{
		show_temp(jia_re_wen_du);
	}
	else if(jia_re_wen_du<last_temp && last_temp-jia_re_wen_du<2)
	{
		show_temp(jia_re_wen_du);
	
	}
	else
	{
		show_temp(last_temp);
	}
		
		
	
}



























