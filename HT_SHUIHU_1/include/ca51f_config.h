#ifndef CA51F_CONFIG_H
#define CA51F_CONFIG_H
/**************************************************************************************************************/
#define IRCH		0
#define IRCL		1
#define PLL			2
#define XOSCL		3

/**************************************************************************************************************/
#define SYSCLK_SRC			IRCH			//оƬϵͳʱ��ѡ��
/**************************************************************************************************************/


/************ϵͳʱ��Ƶ�ʶ��壬��Ҫ��������UART������**********************************************************/
#if (SYSCLK_SRC == IRCH)
	#define FOSC		(3686400)
#elif (SYSCLK_SRC == PLL)
	#define PLL_Multiple			7			//PLL��Ƶ����
	#define FOSC		(3686400*PLL_Multiple)
#else
	#define FOSC		(3686400)
#endif
/***************************************************************************************************************/


/*************************UART���ܿ��غ궨��********************************************************************/
//#define UART0_EN			//���ʹ��UART0���򿪴˺궨��
#define UART1_EN			//���ʹ��UART1���򿪴˺궨��

#define PRINT_EN				//ʹ��uart_printf������ӡʹ��

#ifdef PRINT_EN
//	#define UART0_PRINT		//���ʹ��UART0��ӡ���򿪴˺궨��
	#define UART1_PRINT		//���ʹ��UART1��ӡ���򿪴˺궨��
			
	#ifdef UART0_PRINT
		#define UART0_EN
	#elif defined  UART1_PRINT
		#define UART1_EN
	#endif
#endif
#ifdef UART0_EN
	#define UART0_BAUTRATE		115200
#endif
#ifdef UART1_EN
	#define UART1_BAUTRATE		115200
#endif
/*********************************************************************************************************************/

#endif										
