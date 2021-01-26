#include "sys.h"

u8 code Nixie[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff,0xbf};	//共阳数码管码字
u8 NixieBuff[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};	//数码管显示缓冲区，初值0xff确保启动时都不亮

u8 smg1,smg2,smg3,smg4,smg5,smg6,smg7,smg8;

//数码管显示
void Nixie_Scan(void)
{
	static u8 index = 0;
	
	P2 = (P2 & 0x1f) | 0xe0;	//数码管消隐
	P0 =0xff;
	
	P2 = (P2 & 0x1f) | 0xc0;	//数码管片选
	P0 = 0x01 << index;
	
	P2 = (P2 & 0x1f) | 0xe0;	//数码管段选
	P0 = NixieBuff[index];
	P2 &= 0x1f;
	index ++;
	index &= 0x07;
}

//数码管显示
void Nixie_Show(void)
{	
	NixieBuff[0] = Nixie[smg1];
	NixieBuff[1] = Nixie[smg2];
	NixieBuff[2] = Nixie[smg3];
	NixieBuff[3] = Nixie[smg4];
	NixieBuff[4] = Nixie[smg5];
	NixieBuff[5] = Nixie[smg6];
	NixieBuff[6] = Nixie[smg7];
	NixieBuff[7] = Nixie[smg8];
}

unsigned char BCDToNum(unsigned char bcd)
{
	unsigned char a, b;
	
	a = (bcd >> 4);
	b = bcd & 0x0f;
	
	return (a * 10 + b);
}

unsigned char NumToBCD(unsigned char num)
{
	unsigned char a, b, bcd;
	
	a = (num % 10) & 0x0f;
	b = ((num / 10) << 4) & 0xf0;
	
	bcd = a | b;
	
	return bcd;
}

void Num_deal(int temp_int)
{
	smg1 = smg2 = smg3 = smg4 = smg5 = 16;
	smg6 = (temp_int % 100) / 10; 
	smg7 = temp_int % 10;
	smg8 = 12;
}	

void RefreshTime(unsigned char *time)
{
	if(mode == 0)
	{
		smg1 = BCDToNum(time[3]) / 10;		//时钟十位
		smg2 = BCDToNum(time[3]) % 10;		//时钟个位	
		smg4 = BCDToNum(time[4]) / 10;		//分钟十位
		smg5 = BCDToNum(time[4]) % 10;		//分钟个位	
		smg7 = BCDToNum(time[5]) / 10;		//秒针十位
		smg8 = BCDToNum(time[5]) % 10;		//秒针个位
		
		if(smg8 % 2 == 0)
			smg3 = smg6 = 16;
		else
			smg3 = smg6 = 17;
	}
	else if(mode == 1)
	{
		smg1 = OptionTime[0] / 10;		//设置时钟十位
		smg2 = OptionTime[0] % 10;		//设置时钟个位
		smg3 = 17;
		smg4 = OptionTime[1] / 10;		//设置分钟十位
		smg5 = OptionTime[1] % 10;		//设置分钟个位
		smg6 = 17;
		smg7 = OptionTime[2] / 10;		//设置秒钟十位
		smg8 = OptionTime[2] % 10;		//设置秒钟个位
		
		if(flag_1s == 0)
		{
			switch(time_index)
			{
				case 1: smg1 = smg2 = 16; break;
				case 2: smg4 = smg5 = 16; break;
				case 3: smg7 = smg8 = 16; break;
			}
		}
	}
	else if(mode == 2)
	{
		smg1 = OptionTime[3] / 10;		//闹钟时钟十位
		smg2 = OptionTime[3] % 10;		//闹钟时钟个位
		smg3 = 17;                     
		smg4 = OptionTime[4] / 10;		//闹钟分钟十位
		smg5 = OptionTime[4] % 10;		//闹钟分钟个位
		smg6 = 17;                     
		smg7 = OptionTime[5] / 10;		//闹钟秒钟十位
		smg8 = OptionTime[5] % 10;		//闹钟秒钟个位
		
		if(flag_1s == 0)
		{
			switch(alarm_index)
			{
				case 4: smg1 = smg2 = 16; break;
				case 5: smg4 = smg5 = 16; break;
				case 6: smg7 = smg8 = 16; break;
			}
		}
	}
}

void Led_illume(u8 dat)
{
	P2 = (P2 & 0x1f) | 0x80;	//打开Y4C（LED）
	P0 = dat;					//点亮LED
	P2 = P2 & 0x1f;
}
