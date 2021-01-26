#include "sys.h"

void All_init(void)
{
	P2 = (P2 & 0x1f) | 0x80;	//打开Y4C
	P0 = 0xff;		//关闭LED
	P2 = (P2 & 0x1f) | 0xc0;	//打开Y6C
	P0 = 0x00;		//关闭所有数码管
	P2 = (P2 & 0x1f) | 0xa0;	//打开Y5C
	P0 = 0x00;		//关闭蜂鸣器和继电器
	P2 = P2 & 0x1f;						//关闭所有573	
}

void Timer0Init(void)		//2毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x9a;		//设置定时初值
	TH0 = 0xa9;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}

void Timer0(void) interrupt 1
{
	static u16 T0count1 = 0, T0count2 = 0;
	static u8 index = 0, e2_addr = 0x00;
	
	T0count2 ++;
	
	if(Mode_Option > 0)			//800ms数码管闪烁
	{
		T0count1 ++;
		if(T0count1 >= 400)
		{
			T0count1 = 0;
			flag_800ms = ~flag_800ms;
		}
	}
	
	if((T0count2 % 5 == 0) && (T0count2 < 30))	//每10ms时写一次E2PROM
	{
		switch(index)
		{
			case 0: Write_E2PROM(0x00, time_led[e2_addr]); break;
			case 1: Write_E2PROM(0x01, time_led[e2_addr]); break;
			case 2: Write_E2PROM(0x02, time_led[e2_addr]); break;
			case 3: Write_E2PROM(0x03, time_led[e2_addr]); break;
			case 4: Write_E2PROM(0x04, time_led[e2_addr]); break;
			default : break;
		}
		index ++;
		e2_addr ++;
		
		if(index >= 5)
		{
			index = 0;
			e2_addr  = 0;
		}
	}
	else if(T0count2 >= 30)		//60ms时读取一次AD值
	{
		T0count2 = 0;
		pwm = Read_AIN(0x03);
	}

	Key_Scan();
	Smg_show();
	Smg_Scan();
}

void Timer1Init(void)		//100us@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xAE;		//设置定时初值
	TH1 = 0xFB;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1暂停计时
	ET1 = 1;
}

void Timer1(void) interrupt 3
{
	static u8 dat;
	static u16 T1count1 = 0;
	static u8 mode_backup = 0;
	static u8 index = 0, T1count2 = 0;
	static u8 mode_index = 1;
	u16 temp = 0;
	u8 hightime = 0;
	
	T1count1 ++;
	T1count2 ++;
	
	T1count2 &= 0x0f;			//最大计数到15
	
	temp = 1000 * time_led[mode_index];		//流转时间间隔
	hightime = pwm * pwm;								//高电平时间
	
	if(T1count1 >= temp)
	{
		T1count1 = 0;
		
		if(mode_index == 1)
		{
			if(mode_backup != mode_index)
			{
				dat = 0x7f;
				mode_backup = mode_index;
			}
			dat = _crol_(dat, 1);			//模式1
			if(dat == 0x7f)
				mode_index ++;
		}
		
		else if(mode_index == 2)
		{
			if(mode_backup != mode_index)
			{
				dat = 0xfe;
				mode_backup = mode_index;
			}
			dat = _cror_(dat, 1);			//模式2
			if(dat == 0xfe)
				mode_index ++;
		}
		
		else if(mode_index == 3)
		{
			if(mode_backup != mode_index)
			{
				index = 0;
				mode_backup = mode_index;
			}
			switch(index)
			{
				case 0: dat = 0x7e; break;		//0111 1110
				case 1: dat = 0xbd; break;		//1011 1101
				case 2: dat = 0xdb; break;
				case 3: dat = 0xe7; break;
				default : break;
			}
			index ++;
			index &= 0x03;
			if(dat == 0xe7)
				mode_index ++;
		}
		
		else if(mode_index == 4)
		{
			if(mode_backup != mode_index)
			{
				index = 0;
				mode_backup = mode_index;
			}
			switch(index)
			{
				case 0: dat = 0xe7; break;		//1110 0111
				case 1: dat = 0xdb; break;		//1011 1101
				case 2: dat = 0xbd; break;
				case 3: dat = 0x7e; break;
				default : break;
			}
			index ++;
			index &= 0x03;
			if(dat == 0x7e)
				mode_index = 1;
		}
	}
	
	if(T1count2 < hightime)			//PWM
		LED_work(dat);
	else
		LED_work(0xff);
}
