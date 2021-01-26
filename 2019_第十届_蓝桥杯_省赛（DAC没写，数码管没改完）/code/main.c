#include "sys.h"

bit flag200ms = 0, flag500ms = 0;
bit flagval = 0, flagled = 1, flagtube = 1;
unsigned char mode = 2;
unsigned int FRE, VAL;
unsigned char LEDDAT = 0xff, led = 0xff;

void main(void)
{
	All_Init();
	Timer1Init();
	Timer0Init();
	EA = 1;
	while(1)
	{
		//200ma读取一次AD
		if(flag200ms)
		{
			flag200ms = 0;
			VAL = Read_AIN(0x03);		//读取AIN3的数据
		}
		//500ms测频一次
		if(flag500ms)
		{
			flag500ms = 0;
			TR0 = 0;								//关闭定时器0计数
			FRE = TH0 * 256 + TL0;	//统计500ms内脉冲数
			FRE *= 2;								//乘以2就是1s内脉冲数，就是频率
			TH0 = TL0 = 0;					//清除定时器0计数数据
			TR0 = 1; 								//定时器0重新开始计数
		}

		//如果S5按下，val就是固定的2.0V
		if(flagval == 0)
			VAL = 200;

		//如果LED显示打开
		if(flagled)
		{
			switch(mode)
			{
				case 1: LEDDAT = (LEDDAT | 0x03) & 0xfd; break;			//模式1为测频模式
				case 2: LEDDAT = (LEDDAT | 0x03) & 0xfe; break;			//模式2为测电压模式
			}

			//如果是测频模式，根据范围选择点亮LED
			if(mode == 1)		
			{
				LEDDAT = LEDDAT | 0x10;		//关闭L5
				LEDDAT = LEDDAT | 0x04;		//关闭L3					
				if(FRE < 1000)
					LEDDAT = LEDDAT | 0x08;	//关闭L4
				else if((FRE >= 1000) && (FRE < 5000))
					LEDDAT = (LEDDAT | 0x08) & 0xf7;	//打开L4
				else if((FRE >= 5000) && (FRE < 10000))
					LEDDAT = LEDDAT | 0x08;		//关闭L4
				else if(FRE > 10000)
					LEDDAT = (LEDDAT | 0x08) & 0xf7;	//打开L4
			}
			//如果是测电压模式，根据电压范围选择点亮LED
			else if(mode == 2)
			{
				//不是固定输出2.0V状态
				if(flagval)	
				{
					LEDDAT = LEDDAT & 0xef;		//打开L5
					LEDDAT = LEDDAT | 0x08;		//关闭L4
					if(VAL < 150)
						LEDDAT = LEDDAT | 0x04;	//关闭L3
					else if((VAL >= 150) && (VAL < 250))
						LEDDAT = (LEDDAT | 0x04) & 0xfb;	//打开L3
					else if((VAL > 250) && (VAL < 350))
						LEDDAT = LEDDAT | 0x04;	//关闭L3
					else if(VAL > 350)
						LEDDAT = (LEDDAT | 0x04) & 0xfb;		//打开L3
				}
				//固定输出2.0V状态
				else 
				{
					LEDDAT = LEDDAT | 0x10;			//关闭L5
					LEDDAT = LEDDAT | 0x04;			//关闭L3
				}
			}
		}
		//LED关闭状态，LED全灭
		else
			LEDDAT = 0xff;
		
		led = LEDDAT;

		KeyPress();
		TubeScan();
	}
}

