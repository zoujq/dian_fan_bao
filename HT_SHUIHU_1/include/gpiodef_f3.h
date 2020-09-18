#ifndef _GPIODEF_H_
#define _GPIODEF_H_
enum
{
	HIGH_Z		= 0,
	INPUT		= 1,
	OUTPUT		= 2,

	P00_COM0_SETTING		= 3,
	P01_COM1_SETTING		= 3,
	P02_COM2_SETTING		= 3,
	P03_COM3_SETTING		= 3,
	P04_COM4_SETTING		= 3,
	P17_SEG0_SETTING		= 3,
	P37_SEG1_SETTING		= 3,
	P36_SEG2_SETTING		= 3,
	P35_SEG3_SETTING		= 3,
	P34_SEG4_SETTING		= 3,
	P07_SEG5_SETTING		= 3,
	P06_SEG6_SETTING		= 3,
	P05_SEG7_SETTING		= 3,
	P04_SEG8_SETTING		= 5,
	P03_SEG9_SETTING		= 5,

	P10_TK0_SETTING					= 4,
	P11_TK1_SETTING					= 4,
	P12_TK2_SETTING					= 4,
	P13_TK3_SETTING					= 4,
	P14_TK4_SETTING					= 4,
	P15_TK5_SETTING					= 4,
	P16_TK6_SETTING					= 4,
	P17_TK7_SETTING					= 4,
	P37_TK8_SETTING					= 4,
	P36_TK9_SETTING					= 4,
	P35_TK10_SETTING					= 4,
	P34_TK11_SETTING					= 4,
	P07_TK12_SETTING					= 4,
	P06_TK13_SETTING					= 4,
	P05_TK14_SETTING					= 4,
	P04_TK15_SETTING					= 4,
	P03_TK16_SETTING					= 4,
	P02_TK17_SETTING					= 4,
	P01_TK18_SETTING					= 4,
	P00_TK19_SETTING					= 4,

	P15_PWM0_SETTING					= 6,
	P16_PWM1_SETTING					= 6,
	P35_PWM2_SETTING					= 6,
	P34_PWM3_SETTING					= 6,
	P07_PWM4_SETTING					= 6,
	P06_PWM5_SETTING					= 6,
    
	P10_ADC0_SETTING					= 5,
	P11_ADC1_SETTING					= 5,
	P12_ADC2_SETTING					= 5,
	P13_ADC3_SETTING					= 5,
	P14_ADC4_SETTING					= 5,
	P15_ADC5_SETTING					= 5,
	P16_ADC6_SETTING					= 5,
	P17_ADC7_SETTING					= 5,
	P10_ADC_VREF_SETTING				= 0,

	P30_UART0_TX_SETTING				= 3,
	P31_UART0_RX_SETTING				= 3,
	P11_UART1_TX_SETTING				= 3,
	P12_UART1_RX_SETTING				= 3,
        
	P30_I2C_SDA_SETTING				= 4,
	P31_I2C_SCL_SETTING				= 4,
      
	P11_I2C_SDA_SETTING				= 6,
	P12_I2C_SCL_SETTING				= 6,
 
	P13_T0_SETTING						= 1,
	P14_T1_SETTING						= 1,
	P15_T2_SETTING						= 1,

	P20_RESET_SETTING					= 3,
	P21_CAP_SETTING						= 3,
	P32_XOSC_IN_SETTING					= 3,
	P33_XOSC_OUT_SETTING				= 3,
	P36_T2CP_SETTING					= 5,
	P37_T2EX_SETTING					= 1,

	P00_ERC_SETTING						= 5,

	PU_EN				= 0x80,
	PD_EN				= 0x40,
	OP_EN				= 0x20,
};	
#define  GPIO_Init(reg,val)	reg = val
/******************************************************************************/

#endif
