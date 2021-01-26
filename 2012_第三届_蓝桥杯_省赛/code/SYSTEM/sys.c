#include "sys.h"

void All_Init(void)
{
	P2 = (P2 & 0x1f) | 0x80;	//打开Y4C（LED）
	P0 = 0xff;			//关闭LED
	P2 = (P2 & 0x1f) | 0xe0;	//打开Y7C（数码管）
	P0 = 0xff;			//关闭数码管
	P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
	P0 = 0x00;			//关闭蜂鸣器、继电器
	P2 = P2 & 0x1f;			//关闭所有使能
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
	static u8 T0count1 = 0, T0count2 = 0;
	
	//控制接水程序
	if(mode == 1)
	{
		T0count1 ++;
		if(T0count1 >= 50)
		{
			T0count1 = 0;
			volume ++;
		}
	}
	
	//100ms计时控制程序
	T0count2 ++;
	if(T0count2 >= 50)
	{
		T0count2 = 0;
		flag_100ms = 1;
	}
	
	//继电器控制程序
	if(mode == 1)
	{
		if(volume < 9999)
		{
			P0 = 0x00;
			P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
			P04 = 1;
			P2 = P2 & 0x1f;			//关闭所有使能
		}
		else if(volume >= 9999)
		{
			mode = 0;
			money = volume / 2;
			volume = 0;
		}
	}
	else
	{
		P0 = 0x00;
		P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
		P04 = 0;
		P2 = P2 & 0x1f;			//关闭所有使能
	}
	
	Key_Scan();
	Nixie_Scan();
}