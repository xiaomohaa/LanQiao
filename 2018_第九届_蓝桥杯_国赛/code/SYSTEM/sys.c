#include "sys.h"

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
	if(Read_E2PROM(0x00) == 0x16)
	{
		H_Fre = ((u16)(Read_E2PROM(0x01) << 8)) | (Read_E2PROM(0x02));
		H_Temper_int = ((u16)(Read_E2PROM(0x03) << 8)) | (Read_E2PROM(0x04));
		H_Temper_dec = ((u16)(Read_E2PROM(0x05) << 8)) | (Read_E2PROM(0x06));
		H_Vol = Read_E2PROM(0x07);
	}
	else
	{
		 Write_E2PROM(0x00, 0x16); Delay5ms();
		 Write_E2PROM(0x01, 0); Delay5ms();
		 Write_E2PROM(0x02, 0); Delay5ms();
		 Write_E2PROM(0x03, 0); Delay5ms();
		 Write_E2PROM(0x04, 0); Delay5ms();
		 Write_E2PROM(0x05, 0); Delay5ms();
		 Write_E2PROM(0x06, 0); Delay5ms();
		 Write_E2PROM(0x07, 0);
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

void Timer0Init(void)
{
	AUXR &= 0x7F;							//定时器时钟12T模式
  TMOD = 0x04;                    //设置定时器0为16位自动重装载外部记数模式
  TH0 = TL0 = 0x00;               //设置定时器0初始值
  TR0 = 1;                        //定时器0开始工作
  ET0 = 1;                        //开定时器0中断
}

void Timer1Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x20;		//设置定时初值
	TH1 = 0xD1;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;
}

void Timer1(void) interrupt 3
{
	static u16 t00 = 0, t01 = 0, t03 = 0;
	static u8 t02 = 0;
	static u8 leddat = 0;
	static bit temp = 1;
	u8 ledtemp;
	
	t00 ++; t01 ++; t02 ++;
	
	if(flag_keep)
	{
		if(t02 % 5 == 0)
			flag_5ms = 1;
	}
	
	if(t02 >= 200)
	{
		t02 = 0;
		flag_200ms = 1;
		
		if(Vol > VTH)
			temp = ~temp;		//实现L8闪烁
	}
	if(t01 >= 500)
	{
		t01 = 0;
		flag_500ms = 1;
	}
	if(t00 >= 1000)
	{
		t00 = 0;
		flag_1s = 1;
	}
	
	//S6长按键800ms检测程序
	if(flag_S6_Press)
	{
		t03 ++;
		if(t03 >= 800)
		{
			t03 = 0;
			flag_S6_LongPress = 1;
		}
		if(flag_S6_LongPress)
		{
			if(t03 >= 100)
			{
				t03 = 0;
				VTH += 1;
				if(VTH > 50)
					VTH = 1;
			}
		}
	}
	else
	{
		t03 = 0;
		flag_S6_LongPress = 0;
	}
	
	if(mode == 3)				//温度测量 L1亮
		leddat = 0xfe;
	else if(mode == 2)	//频率测量 L2亮
		leddat = 0xfd;
	else if(mode == 1)	//电压测量 L3亮
		leddat = 0xfb;
	else
		leddat = 0xff;
	
	//实现L8闪烁程序
	if(mode != 4)
	{
		if(Vol > VTH)
		{
			ledtemp = (u8)temp << 7;
			ledtemp = 0xff - ledtemp;
			leddat &= ledtemp;
		}
	}
	
	KeyScan();
	TubeShow();
	TubeScan();
	LedWork(leddat);	
}







