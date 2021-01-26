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
	static u16 t00 = 0, t01 = 0;
	u16 ledtime;
	static u8 leddat = 0xff, ledtemp = 0;
	
	t00 ++;
	t01 ++;
	
	if(t00 >= 1000)
	{
		t00 = 0;
		flag_1s = 1;
	}
	
	//错误指示灯L2
	if(flag_error == 1)
		leddat = leddat & 0xfd;
	else
		leddat = leddat | 0x02;
	
	//模式指示灯L1
	switch(temp_mode)
	{
		case 0: ledtime = 800; break;
		case 1: ledtime = 400; break;
		case 2: ledtime = 200; break;
	}
	
	if(t01 >= ledtime)
	{
		t01 = 0;
		ledtemp = ~ledtemp;
		if(ledtemp)
			leddat = leddat & 0xfe;
		else
			leddat = leddat | 0x01;
	}
	
	if(temp_mode == 2)
	{
		P0 = 0x00;
		P2 = (P2 & 0x1f) | 0xa0;
		P04 = 1;			//打开继电器
		P2 = P2 & 0x1f;
	}
	else
	{
		P0 = 0x00;
		P2 = (P2 & 0x1f) | 0xa0;
		P04 = 0;			//关闭继电器
		P2 = P2 & 0x1f;
	}
	
	LEDLight(leddat);
	TubeScan();
	KeyScan();
}
