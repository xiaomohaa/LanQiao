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
	static u16 t00 = 0;
	static u8 t01 = 0, t02 = 0;
	u16 temp;		//存储采集时间上限
	
	t00 ++;
	t01 ++;
	t02 ++;
	
	//读取DS1302
	if(t02 >= 5)		//10ms
	{
		t02 = 0;
		flag_10ms = 1;
	}
	
	
	//采集温度时间间隔
	temp = Time_temper[time_index] * 500;		//时间上限
	if(mode == 1)
	{
		if(t00 >= temp)
		{
			t00 = 0;
			flag_temper = 1;
		}
	}
	else
		t00 = 0;
	
	//LED闪烁
	if(flag_led)
	{
		if(t01 < 100)		//200ms
			LEDLight(0xfe);
		else
		{
			LEDLight(0xff);
			if(t01 >= 200)	//400ms
				t01 = 0;
		}
	}
	else
	{
		t01 = 0;
		LEDLight(0xff);
	}
	
	KeyScan();
	TubeShow();
	TubeScan();
}

