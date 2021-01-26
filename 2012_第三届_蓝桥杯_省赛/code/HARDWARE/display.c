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
	if(mode == 0)
	{
		smg1 = 16;
		smg2 = price / 100;
		smg3 = (price % 100) / 10;
		smg4 = price % 10;
		smg5 = money / 1000;
		smg6 = (money % 1000) / 100;
		smg7 = (money % 100) / 10;
		smg8 = money % 10;
	}
	else if(mode == 1)
	{
		smg1 = 16;
		smg2 = price / 100;
		smg3 = (price % 100) / 10;
		smg4 = price % 10;
		smg5 = volume / 1000;
		smg6 = (volume % 1000) / 100;
		smg7 = (volume % 100) / 10;
		smg8 = volume % 10;
	}

	NixieBuff[0] = Nixie[smg1];
	NixieBuff[1] = Nixie[smg2] & 0x7f;
	NixieBuff[2] = Nixie[smg3];
	NixieBuff[3] = Nixie[smg4];
	NixieBuff[4] = Nixie[smg5];
	NixieBuff[5] = Nixie[smg6] & 0x7f; 
	NixieBuff[6] = Nixie[smg7];
	NixieBuff[7] = Nixie[smg8];
}

void Led_illume(u8 dat)
{
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0x80;	//打开Y4C（LED）
	P0 = dat;					//点亮LED
	P2 = P2 & 0x1f;
}
