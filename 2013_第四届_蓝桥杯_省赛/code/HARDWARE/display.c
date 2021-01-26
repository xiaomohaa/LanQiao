#include "sys.h"

u8 code Nixie[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 
									0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e, 
									0xff, 0xbf};	//共阳数码管码字
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
	if(flag_option == 0)
	{
		smg1 = BCDToNum(Realtime[3]) / 10;
		smg2 = BCDToNum(Realtime[3]) % 10;
		if((BCDToNum(Realtime[5]) % 10) % 2 == 0)
			smg3 = 17;
		else
			smg3 = 16;
		smg4 = BCDToNum(Realtime[4]) / 10;
		smg5 = BCDToNum(Realtime[4]) % 10;
		smg6 = 16;
		smg7 = real_wet / 10;
		smg8 = real_wet % 10;
	}
	else if(flag_option == 1)
	{
		smg1 = smg2 = 17;
		smg3 = smg4 = smg5 = smg6 = 16;
		smg7 = wet_value / 10;
		smg8 = wet_value % 10;
	}
	
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

void Led_illume(u8 dat)
{
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0x80;	//打开Y4C（LED）
	P0 = dat;					//点亮LED
	P2 = P2 & 0x1f;
}
