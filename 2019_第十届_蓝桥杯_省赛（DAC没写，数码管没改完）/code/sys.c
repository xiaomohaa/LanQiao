#include "sys.h"

void All_Init(void)
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;				
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;			
	P2 = (P2 & 0x1f) | 0xa0;	
	P0 = 0x00;				
	P2 = P2 & 0x1f;
}

//定时器1初始化
void Timer1Init(void)		//2毫秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x30;		//设置定时初值
	TH1 = 0xF8;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;
}

//定时器1中断服务程序
void Timer1(void) interrupt 3
{
	static unsigned char t00 = 0, t01 = 0;

	t00 ++;		//200ms
	t01 ++;		//500ms

	if(t00 >= 100)
	{
		t00 = 0;
		flag200ms = 1;
	}

	if(t01 >= 250)
	{
		t01 = 0;
		flag500ms = 1;
	} 

	LedLight();
	KeyScan();
	TubeShow();
}

//定时器0初始化为计数模式
void Timer0Init(void)
{
	AUXR &= 0x7F;					//定时器时钟12T模式
  TMOD = 0x04;          //设置定时器0为16位自动重装载外部记数模式
  TH0 = TL0 = 0;     		//设置定时器0初始值
	TR0 = 1;              //定时器0开始工作
}