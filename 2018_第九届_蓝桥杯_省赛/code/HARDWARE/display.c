#include "sys.h"

u8 code Nixie[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
										0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e,
										0xff, 0xbf};		//16 17
u8 Nixiebuff[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
u8 smg1, smg2, smg3, smg4, smg5, smg6, smg7, smg8;

//数码管扫描
void Smg_Scan(void)
{
	static u8 index = 0;
	
	P2 = (P2 & 0x1f) | 0xc0;	//打开Y6C
	P0 = 0x01 << index;
	
	P2 = (P2 & 0x1f) | 0xe0;	//打开Y7C
	P0 = 0xff;
	P0 = Nixiebuff[index];
	
	index ++;
	index &= 0x07;
}

//更新数码管值
void Smg_show()
{
	if(Mode_Option == 0)		//不是设置模式不显示
	{
		if(display_pwm == 0)
			smg1 = smg2 = smg3 = smg4 = smg5 = smg6 = smg7 = smg8 = 16;
		else
		{
			smg1 = smg2 = smg3 = smg4 = smg5 = smg6 = 16;
			smg7 = 17;
			smg8 = pwm % 10;
		}
	}
	else if(Mode_Option == 1)
	{
		if(flag_800ms == 1)		//显示
		{
			smg1 = 17;
			smg2 = Mode_led % 10;
			smg3 = 17;
			smg4 = 16;
			smg5 = time_led[Mode_led] / 10;
			smg6 = time_led[Mode_led] % 10;
			smg7 = 0;
			smg8 = 0;
		}
		else				//不显示，实现闪烁
		{
			smg1 = 16;
			smg2 = 16;
			smg3 = 16;
			smg4 = 16;
			smg5 = time_led[Mode_led] / 10;
			smg6 = time_led[Mode_led] % 10;
			smg7 = 0;
			smg8 = 0;
		}
	}
	else if(Mode_Option == 2)
	{
		if(flag_800ms == 1)		//显示
		{
			smg1 = 17;
			smg2 = Mode_led % 10;
			smg3 = 17;
			smg4 = 16;
			smg5 = time_led[Mode_led] / 10;
			smg6 = time_led[Mode_led] % 10;
			smg7 = 0;
			smg8 = 0;
		}
		else				//不显示，实现闪烁
		{
			smg1 = 17;
			smg2 = Mode_led % 10;
			smg3 = 17;
			smg4 = 16;
			smg5 = 16;
			smg6 = 16;
			smg7 = 16;
			smg8 = 16;
		}
	}

	
	Nixiebuff[3] = Nixie[smg1];
	Nixiebuff[2] = Nixie[smg2];
	Nixiebuff[1] = Nixie[smg3];
	Nixiebuff[0] = Nixie[smg4];
	Nixiebuff[7] = Nixie[smg5];
	Nixiebuff[6] = Nixie[smg6];
	Nixiebuff[5] = Nixie[smg7];
	Nixiebuff[4] = Nixie[smg8];
}


//LED显示
void LED_work(u8 dat)
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = dat;
	P2 = P2 & 0x1f;
}


