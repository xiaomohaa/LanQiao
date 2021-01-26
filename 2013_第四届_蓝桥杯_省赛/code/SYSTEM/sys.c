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
	static u8 T0count1 = 0;
	
	//100ms定时程序
	T0count1 ++;
	if(T0count1 >= 50)			//100ms
	{
		T0count1 = 0;
		flag_100ms = 1;
	}
	
	if(flag_option == 0)
		Working_control();
	
	Nixie_Scan();
	Key_Scan();
}

void Working_control(void)
{	
	//手动模式继电器控制程序
	if((mode == 0) && (flag_relay == 1))
	{
		Led_illume(0xfe);
		P0 = 0x00;
		P04 = 1;									//打开继电器
		P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
		P2 = P2 & 0x1f;						//关闭所有使能
	}
	else if((mode == 0) && (flag_relay == 0))
	{
		Led_illume(0xff);
		P0 = 0x00;
		P04 = 0;									//关闭继电器
		P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
		P2 = P2 & 0x1f;						//关闭所有使能
	}
	
	//手动模式蜂鸣器控制程序
	if((real_wet < wet_value) && (flag_buzz == 1) && (mode == 0))
	{
		P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
		P06 = 1;									//打开蜂鸣器
		P2 = P2 & 0x1f;						//关闭所有使能
	}
	else if(((real_wet >= wet_value) || (flag_buzz == 0)) && (mode == 0))
	{
		P06 = 0;									//关闭蜂鸣器
		P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
		P2 = P2 & 0x1f;						//关闭所有使能
	}
	
	//自动模式继电器控制程序
	if((real_wet < wet_value) && (mode == 1))
	{
		Led_illume(0xfd);
		P0 = 0x00;
		P04 = 1;									//打开继电器
		P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
		P2 = P2 & 0x1f;						//关闭所有使能
	}
	else if((real_wet >= wet_value) && (mode == 1))
	{
		Led_illume(0xff);
		P0 = 0x00;
		P04 = 0;									//关闭继电器
		P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
		P2 = P2 & 0x1f;						//关闭所有使能
	}
}
