#ifndef _CA51F3_XRAM_H_
#define _CA51F3_XRAM_H_

/*********************************************************************************************************************/
#ifdef _MAIN_C_
#define XRAM_U8										volatile unsigned char xdata
#define XRAM_U16									volatile unsigned int xdata
#define XRAM_AT_(x)									_at_ x		
#else
#define XRAM_U8										extern volatile unsigned char xdata
#define XRAM_U16									extern volatile unsigned int xdata
#define XRAM_AT_(x)
#endif

XRAM_U8 P00F        XRAM_AT_(0x8000);
XRAM_U8 P01F        XRAM_AT_(0x8001);
XRAM_U8 P02F        XRAM_AT_(0x8002);
XRAM_U8 P03F        XRAM_AT_(0x8003);
XRAM_U8 P04F        XRAM_AT_(0x8004);
XRAM_U8 P05F        XRAM_AT_(0x8005);
XRAM_U8 P06F        XRAM_AT_(0x8006);
XRAM_U8 P07F        XRAM_AT_(0x8007);

XRAM_U8 P10F        XRAM_AT_(0x8008);
XRAM_U8 P11F        XRAM_AT_(0x8009);
XRAM_U8 P12F        XRAM_AT_(0x800a);
XRAM_U8 P13F        XRAM_AT_(0x800b);
XRAM_U8 P14F        XRAM_AT_(0x800c);
XRAM_U8 P15F        XRAM_AT_(0x800d);
XRAM_U8 P16F        XRAM_AT_(0x800e);
XRAM_U8 P17F        XRAM_AT_(0x800f);

XRAM_U8 P20F        XRAM_AT_(0x8010);
XRAM_U8 P21F        XRAM_AT_(0x8011);

XRAM_U8 P30F        XRAM_AT_(0x8018);
XRAM_U8 P31F        XRAM_AT_(0x8019);
XRAM_U8 P32F        XRAM_AT_(0x801a);
XRAM_U8 P33F        XRAM_AT_(0x801b);
XRAM_U8 P34F        XRAM_AT_(0x801c);
XRAM_U8 P35F        XRAM_AT_(0x801d);
XRAM_U8 P36F        XRAM_AT_(0x801e);
XRAM_U8 P37F        XRAM_AT_(0x801f);

XRAM_U8 ADCALL      XRAM_AT_(0x8020);
XRAM_U8 ADCALH      XRAM_AT_(0x8021);

XRAM_U8 SPMAX      	XRAM_AT_(0x8100);
XRAM_U8 I2CIOS      XRAM_AT_(0x8101);

XRAM_U8 TKPWC       XRAM_AT_(0x8103);
XRAM_U8 TLEN        XRAM_AT_(0x8106);
XRAM_U8 TLDAT       XRAM_AT_(0x8107);
XRAM_U8 TLCON       XRAM_AT_(0x8108);
XRAM_U8 TLFLG       XRAM_AT_(0x8109);
XRAM_U8 TLCKS       XRAM_AT_(0x810a);
XRAM_U8 TLCNTKL     XRAM_AT_(0x810b);
XRAM_U8 TLCNTKH     XRAM_AT_(0x810c);
XRAM_U8 TLCNTLL     XRAM_AT_(0x810d);
XRAM_U8 TLCNTLH     XRAM_AT_(0x810e);
XRAM_U8 TLCNTD      XRAM_AT_(0x810f);
XRAM_U8 TLCOMS      XRAM_AT_(0x8110);
XRAM_U8 LXCAD       XRAM_AT_(0x8117);
XRAM_U8 FTCTL       XRAM_AT_(0x811e);
XRAM_U8 TPCTL       XRAM_AT_(0x811f);
XRAM_U8 P00C        XRAM_AT_(0x8120);
XRAM_U8 P01C        XRAM_AT_(0x8121);
XRAM_U8 P02C        XRAM_AT_(0x8122);
XRAM_U8 P03C        XRAM_AT_(0x8123);
XRAM_U8 P04C        XRAM_AT_(0x8124);
XRAM_U8 P05C        XRAM_AT_(0x8125);
XRAM_U8 P06C        XRAM_AT_(0x8126);
XRAM_U8 P07C        XRAM_AT_(0x8127);
XRAM_U8 P10C        XRAM_AT_(0x8128);
XRAM_U8 P11C        XRAM_AT_(0x8129);
XRAM_U8 P12C        XRAM_AT_(0x812a);
XRAM_U8 P13C        XRAM_AT_(0x812b);
XRAM_U8 P14C        XRAM_AT_(0x812c);
XRAM_U8 P15C        XRAM_AT_(0x812d);
XRAM_U8 P16C        XRAM_AT_(0x812e);
XRAM_U8 P17C        XRAM_AT_(0x812f);
XRAM_U8 P20C        XRAM_AT_(0x8130);
XRAM_U8 P21C        XRAM_AT_(0x8131);
XRAM_U8 P30C        XRAM_AT_(0x8138);
XRAM_U8 P31C        XRAM_AT_(0x8139);
XRAM_U8 P32C        XRAM_AT_(0x813a);
XRAM_U8 P33C        XRAM_AT_(0x813b);
XRAM_U8 P34C        XRAM_AT_(0x813c);
XRAM_U8 P35C        XRAM_AT_(0x813d);
XRAM_U8 P36C        XRAM_AT_(0x813e);
XRAM_U8 P37C        XRAM_AT_(0x813f);





XRAM_U8 MECON       XRAM_AT_(0xfc00);
XRAM_U8 FSCMD       XRAM_AT_(0xfc01);
XRAM_U8 FSDAT       XRAM_AT_(0xfc02);
XRAM_U8 LOCK        XRAM_AT_(0xfc03);
XRAM_U8 PADRD       XRAM_AT_(0xfc04);
XRAM_U8 PTSL        XRAM_AT_(0xfc05);
XRAM_U8 PTSH        XRAM_AT_(0xfc06);
XRAM_U8 REPSET      XRAM_AT_(0xfc07);
XRAM_U8 MEMPTEC     XRAM_AT_(0xfc0d);
XRAM_U8 PADRM       XRAM_AT_(0xfc0f);




/*********************************************************************************************************************/

#endif