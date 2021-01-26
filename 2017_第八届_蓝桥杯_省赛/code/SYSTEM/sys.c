#include "sys.h"

//关闭所有外设
void All_Init(void)
{
	P2 = (P2 & 0x1f) | 0x80;	//打开Y4C（LED）
	P0 = 0xff;					//关闭LED
	P2 = (P2 & 0x1f) | 0xe0;	//打开Y7C（数码管）
	P0 = 0xff;					//关闭数码管
	P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
	P0 = 0x00;					//关闭蜂鸣器、继电器
	P2 = P2 & 0x1f;
}

//定时器0初始化
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

//定时器0中断程序
void Time0(void) interrupt 1
{
	static unsigned char t01 = 0, t02 = 0;
	static unsigned int t03 = 0, t04 = 0;
	
	t01 ++;
	t03 ++;
	if(t01 >= 50)
	{
		flag_100ms = 1;
		t01 = 0;
		
		if(mode == 0)
			TimeBackup(&bufTime);					//备份时间
	}
	
	if(flag_led == 1)
	{
		t02 ++;
		if(t02 <= 100)
			Led_dat = 0xfe;
		else
		{
			Led_dat = 0xff;
			if(t02 >= 200)
				t02 = 0;
		}
	}
	else if(flag_led == 0)
	{
		t02 = 0;
		Led_dat = 0xff;
	}
	
	if(t03 >= 500)
	{
		t03 = 0;
		flag_1s  = ~flag_1s;
	}
	
	if(flag_led == 1)
	{
		t04 ++;
		
		if(t04 >= 2500)
		{
			t04 = 0;
			flag_led = 0;
		}
	}
	else
		t04 = 0;
	
	KeyScan();

	Nixie_Scan();
}

void IsAlarm(u8 *time)
{
	if((time[5] == AlarmTime[2]))
	{
		if(time[4] == AlarmTime[1])
		{
			if(time[3] == AlarmTime[0])
			{
				if(flag_alarm)
					flag_led = 1;
			}
		}
	}
	else
		flag_alarm = 1;
}


