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


void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9A;		//设置定时初值
	TH0 = 0xA9;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}

void Timer0(void) interrupt 1
{
	static u8 T0count1 = 0;
	static u16 T0count2 = 0;
	u16 time_temp = 0;
	
	T0count1 ++;
	T0count2 ++;
	time_temp = Temper_time * 500;
	
	if(T0count1 >= 25)
	{
		T0count1 = 0;
		flag_50ms = 1;
	}
	
	if(flag_start)
	{
		if(T0count2 >= time_temp)
		{
			T0count2 = 0;
			flag_temper = 1;
		}
	}
	else
	{
		T0count2 = 0;
		flag_temper = 0;
	}

	KeyScan();
	TubeShow();
	TubeScan();
}

