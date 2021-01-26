#include "sys.h"

void AllInit(void)
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x00;
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 = P2 & 0x1f;
}


void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}

void Timer0(void) interrupt 1
{
	static u16 T0count1 = 0;
	static u16 T0count2 = 0;
	u16 time = 0;
	static bit flag_led = 0;
	u8 leddat;
	
	T0count1 ++;
	T0count2 ++;
	
	if(T0count1 >= 400)	//200ms
	{
		T0count1 = 0;
		flag_200ms = 1;
	}
	
	if(mode == 0)
	{
		time = 800;
		P2 = (P2 & 0x1f) | 0xa0;
		P0 = 0x00;
		P2 = P2 & 0x1f;
	}
	else if(mode == 1)
	{
		time = 400;
		P2 = (P2 & 0x1f) | 0xa0;
		P0 = 0x00;
		P2 = P2 & 0x1f;
	}
	else if(mode == 2)
	{
		time = 200;
		P0 = 0x00;
		P2 = (P2 & 0x1f) | 0xa0;
		P0 = 0x10;
		P2 = P2 & 0x1f;
	}
		
	if(T0count2 >= time)
	{
		T0count2 = 0;	
		flag_led = ~flag_led;
		if(flag_led == 1)
			leddat = 0xfe;
		else
			leddat = 0xff;
	}
	
	if(flag_option == 0)
		LEDLight(leddat);
	else if((flag_option = 1) && (flag_right == 1))
		LEDLight(0xff);
	else if((flag_option = 1) && (flag_right == 0))
		LEDLight(0xfd);
	
	TubeScan();
	KeyScan();
}
