#include "sys.h"

void AllInit(void)
{
	P2 = (P2 & 0x1f) | 0x80;	//open Y4C
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;	//open Y6C
	P0 = 0x00;
	P2 = (P2 & 0x1f) | 0xa0;	//open Y5C
	P0 = 0x00;
	P2 = P2 & 0x1f;
}

void Timer0Init(void)		//100微秒@11.0592MHz
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
	static u8 T0count1 = 0, T0count3 = 0;
	static u16 T0count2 = 0, T0count4 = 0;
	u8 temp;
	
	T0count1 ++;
	
	if(flag_temper)
		T0count4 ++;
	else
		T0count4 = 0;
	
	if(flag_fan)
	{
		T0count2 ++;
		T0count3 ++;
	}
		
	if(T0count1 >= 20)		//2ms
	{
		T0count1 = 0;
		flag_2ms = 1;
	}
	
	if(T0count4 >= 2000)	//500ms	show temperture
	{
		T0count4 = 0;
		flag_500ms = 1;
	}
	
	if(flag_fan)
	{
		switch(modebackup)
		{
			case 1: temp = 2; break;
			case 2: temp = 3; break;
			case 3: temp = 7; break;
		}
		
		if(T0count3 <= temp)
			PWM = 1;
		else
		{
			PWM = 0;
			if(T0count3 >= 10)
				T0count3 = 0;
		}
	}
	else
		T0count3 = 0;
	
	if(T0count2 >= 10000)								//1s
	{
		T0count2 = 0;
		sec --;
		if(sec < 0)				//sec
		{
			sec = 59;
			min --;
			if(min < 0)			//min
			{
				min = 0;			//stop
				min = 0;			//stop
				flag_fan = 0;
			}
		}
	}
	LEDWork();
	TubeScan();
}











