
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
void main(void)
{
	int t0=0;
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
	
//	lcd_io_init();
//	init_keys_gpio();
//	init_temp_adc();
//	show_temp(0);
//	init_pwm();
	
//	set_hot_power(0);
	
	P34F = OUTPUT|OP_EN;    //P34保温灯
	P07F = OUTPUT|OP_EN;    //P07煮饭灯
	
	P15F = OUTPUT|PD_EN; //HOT 加热输出
	P16F = P16_ADC6_SETTING;// NTC
	P06F =INPUT | PD_EN; //检测煮饭switch

	P15C &= ~0x10;//弱下拉

	P06C &= ~0x10;//弱下拉
//	P34=1;
//	P07=0;
//	
	while(1)
	{
		
		if(P06==1){
			P15=1;
			P07=0;
			P34=1;
		}
		else
		{
			P15=0;
			P07=1;
			P34=0;
		}
//		t0++;
//		
//		if((t0%100)==0)
//		{
//			refresh_screen();
//			On_keys();
//			On_xuan_niu_key();
//		}
//		if((t0%50)==0)
//		{
//			On_xuan_niu();
//		}
//		if((t0%500)==0)
//		{			
//			check_temp();
//			check_hot();
//			display_temp();
//			
//		}
//		if(t0>10000)
//		{			
//			t0=1;			
//		}
		Delay_ms(1);
	}
/***********************************************************************************/		


}
#endif
