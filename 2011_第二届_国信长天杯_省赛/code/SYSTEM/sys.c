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

void Timer0Init(void)		//100us@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xAE;		//设置定时初值
	TH0 = 0xFB;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}

void Timer0(void) interrupt 1
{
	static u16 T0count1 = 0;
	static u8 Tocount2 = 0;
	static u8 index = 0;
	
	index ++;
	T0count1 ++;
	Tocount2 ++;
	if(T0count1 >= 2000)	//200ms
	{
		T0count1 = 0;
		flag_200ms = 1;
	}
	
	if(Tocount2 >= 20)		//2ms
	{
		Tocount2 = 0;
		KeyScan();
		TubeScan();
	}
	
	//P34输出PWM控制程序
	if(flag_pwm == 1)
	{
		if(index < 3)
			PWM = 1;
		else if(index >= 3)
		{
			PWM = 0;
			if(index >= 10)
				index = 0;
		}		
	}
	else
		index = 0;
	
	//继电器控制程序
	if(flag_relay)
	{
		P0 = 0x00;
		P2 = (P2 & 0x1f) | 0xa0;
		P04 = 1;
		P2 = P2 & 0x1f;
	}
	else
	{
		P0 = 0x00;
		P2 = (P2 & 0x1f) | 0xa0;
		P04 = 0;
		P2 = P2 & 0x1f;
	}
}

