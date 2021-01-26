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

void Timer1Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xCD;		//设置定时初值
	TH1 = 0xD4;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;
}

void Time1(void) interrupt 3
{
	static u16 T1count1 = 0, T1count2 = 0;
	
	T1count1 ++;
	
	if(T1count1 % 2 == 0)		//2ms
		TubeScan();
	if(T1count1 % 5 == 0)		//5ms
		flag_5ms = 1;
	if(T1count1 % 100 == 0)	//100ms
		flag_100ms = 1;
	if(T1count1 >= 500)			//500ms
	{
		T1count1 = 0;
		if(flag_fretext)
			flag_500ms = 1;
	}
	
	if(mode_time || mode_val || (mode == 3))
	{
		T1count2 ++;
		if(T1count2 >= 1000)	//1s
		{
			T1count2 = 0;
			flag_1s = ~flag_1s;
		}
	}
	else
		T1count2 = 0;
	
	KeyScan();
}

void Timer0Init(void)
{
	AUXR &= 0x7F;					//定时器时钟12T模式
  TMOD = 0x04;          //设置定时器0为16位自动重装载外部记数模式
  TH0 = TL0 = 0;     		//设置定时器0初始值
	TR0 = 1;              //定时器0开始工作
}

