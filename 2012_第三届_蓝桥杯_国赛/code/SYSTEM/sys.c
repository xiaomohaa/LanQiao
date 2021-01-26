#include "sys.h"

#define sonic_nop {_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();};

void Delay5ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 59;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}

void Check_data(void)
{
	if(Read_E2PROM(0x00) == 0x18)
	{
		Keyword[0] = Read_E2PROM(0x01);
		Keyword[1] = Read_E2PROM(0x02);
		Keyword[2] = Read_E2PROM(0x03);
		Keyword[3] = Read_E2PROM(0x04);
		Keyword[4] = Read_E2PROM(0x05);
		Keyword[5] = Read_E2PROM(0x06);
	}
	else
	{
		 Write_E2PROM(0x00, 0x18); Delay5ms();
		 Write_E2PROM(0x01, Keyword[0]); Delay5ms();
		 Write_E2PROM(0x02, Keyword[1]); Delay5ms();
		 Write_E2PROM(0x03, Keyword[2]); Delay5ms();
		 Write_E2PROM(0x04, Keyword[3]); Delay5ms();
		 Write_E2PROM(0x05, Keyword[4]); Delay5ms();
		 Write_E2PROM(0x06, Keyword[5]);
	}
}

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
	static u8 t00 = 0;
	static u16 t01 = 0, t02 = 0, t03 = 0;
	
	t00 ++;
	t03 ++;
	
	if(t00 % 5 == 0)
		flag_5ms = 1;
	if(t00 >= 10)
	{
		t00 = 0;
		flag_100ms = 1;
	}
	
	if(t03 >= 100)
	{
		t03 = 0;
		flag_125ms = 1;
	}
	
	//继电器工作时间控制
	if(flag_relay)
	{
		t01 ++;
		
		if(t01 >= 5000)		//5s
		{
			t01 = 0;
			flag_relay = 0;
		}
	}
	
	//蜂鸣器工作时间控制
	if(flag_beep)
	{
		t02 ++;
		
		if(t02 >= 3000)		//3s
		{
			t02 = 0;
			flag_beep = 0;
		}
		
		error_times = 0;
	}

	KeyScan();
	TubeShow();
	TubeScan();
	BeepWork(flag_beep);
	RelayWork(flag_relay);
}

void Timer1Init(void)		//0微秒@11.0592MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0;		//设置定时初值
	TH1 = 0;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;
}

void SendSonic(void)
{
	unsigned char i = 8;
	
	while(i --)
	{
		TX = 1;
		sonic_nop; sonic_nop; sonic_nop; sonic_nop; sonic_nop;
		sonic_nop; sonic_nop; sonic_nop; sonic_nop; sonic_nop;
		TX = 0;
		sonic_nop; sonic_nop; sonic_nop; sonic_nop; sonic_nop;
		sonic_nop; sonic_nop; sonic_nop; sonic_nop; sonic_nop;
	}
}




