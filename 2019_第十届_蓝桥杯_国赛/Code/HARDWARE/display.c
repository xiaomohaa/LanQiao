#include "sys.h"

unsigned char code table[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 
                            0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e,
														0xff, 0xbf, 0xc7};	//18-L
unsigned char TubeBuff[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
unsigned char smg1, smg2, smg3, smg4, smg5, smg6, smg7, smg8;

void TubeScan(void)
{
	static u8 index = 0;
	
	P0 = 0x00;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x01 << index;
	P2 = P2 & 0x1f;
	
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = TubeBuff[index];
	P2 = P2 & 0x1f;
	
	index ++;
	index &= 0x07;
}

void TubeShow(void)
{
	//显示温度
	if(mode == 1)
	{
		smg1 = 12;		//12
		smg2 = smg3 = smg4 = 16;
		smg5 = Temper_int / 10;
		smg6 = Temper_int % 10;
		smg7 = Temper_dec / 10;
		smg8 = Temper_dec % 10;
	}
	//显示距离
	else if(mode == 2)
	{
		smg1 = 18;
		smg2 = smg3 = smg4 = smg5 = smg6 = 16;
		smg7 = distance / 10;
		smg8 = distance % 10;
	}
	
	TubeBuff[3] = table[smg1];
	TubeBuff[2] = table[smg2];
	TubeBuff[1] = table[smg3];
	TubeBuff[0] = table[smg4];
	TubeBuff[7] = table[smg5];
	TubeBuff[6] = table[smg6];
	TubeBuff[5] = table[smg7];
	TubeBuff[4] = table[smg8];
	
	//添加小数点
	if(mode == 1)
		TubeBuff[6] &= 0x7f;
	else if((mode == 3) || (mode == 7))
		TubeBuff[5] &= 0x7f;
}

void LedWork(u8 leddat)
{
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0x80;
	P0 = leddat;
	P2 = P2 & 0x1f;
}