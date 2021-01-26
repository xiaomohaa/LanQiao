#include "sys.h"

unsigned char code table[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 
                            0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e,
														0xff, 0xbf, 0xc1, 0x89, 0x8c};	//18-U 19-H 20-P
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
	//mode == 1 显示RB2电压
	if(mode == 1)
	{
		smg1 = 18;	//U
		smg2 = smg3 = smg4 = smg5 = smg6 = 16;
		smg7 = Vol / 10;
		smg8 = Vol % 10;
	}
	
	//mode == 2 显示频率
	else if(mode == 2)
	{
		smg1 = 15;	//F
		smg2 = smg3 = 16;
		smg4 = Fre / 10000;
		smg5 = (Fre % 10000) / 1000;
		smg6 = (Fre % 1000) / 100;
		smg7 = (Fre % 100) / 10;
		smg8 = Fre % 10;
		
		if(smg4 == 0)
		{
			smg4 = 16;
			if(smg5 == 0)
			{
				smg5 = 16;
				if(smg6 == 0)
				{
					smg6 = 16;
					if(smg7 == 0)
						smg7 = 16;
				}
			}
		}
	}
	
	//mode == 3 显示温度
	else if(mode == 3)
	{
		smg1 = 12;		//T
		smg2 = smg3 = smg4 = 16;
		smg5 = Temper_int / 10;
		smg6 = Temper_int % 10;
		smg7 = Temper_dec / 10;
		smg8 = Temper_dec % 10;
		
		if(smg5 == 0)
			smg5 = 16;
	}
	
	//mode == 4 显示阈值设置
	else if(mode == 4)
	{
		smg1 = 20;
		smg2 = smg3 = smg4 = smg5 = smg6 = 16;
		smg7 = VTH / 10;
		smg8 = VTH % 10;
	}

	//mode == 5 回显RB2电压
	else if(mode == 5)
	{
		smg1 = 19;	//H
		smg2 = 18;	//U
		smg3 = smg4 = smg5 = smg6 = 16;
		smg7 = H_Vol / 10;
		smg8 = H_Vol % 10;
	}
	
	//mode == 6 回显频率
	else if(mode == 6)
	{
		smg1 = 19;	//H
		smg2 = 15;	//F
		smg3 = 16;
		smg4 = H_Fre / 10000;
		smg5 = (H_Fre % 10000) / 1000;
		smg6 = (H_Fre % 1000) / 100;
		smg7 = (H_Fre % 100) / 10;
		smg8 = H_Fre % 10;
		
		if(smg4 == 0)
		{
			smg4 = 16;
			if(smg5 == 0)
			{
				smg5 = 16;
				if(smg6 == 0)
				{
					smg6 = 16;
					if(smg7 == 0)
						smg7 = 16;
				}
			}
		}
	}
	
	//mode == 7 回显温度
	else if(mode == 7)
	{
		smg1 = 19;	//H
		smg2 = 12;	//T
		smg3 = smg4 = 16;
		smg5 = H_Temper_int / 10;
		smg6 = H_Temper_int % 10;
		smg7 = H_Temper_dec / 10;
		smg8 = H_Temper_dec % 10;
		
		if(smg5 == 0)
			smg5 = 16;
	}
	
	TubeBuff[0] = table[smg1];
	TubeBuff[1] = table[smg2];
	TubeBuff[2] = table[smg3];
	TubeBuff[3] = table[smg4];
	TubeBuff[4] = table[smg5];
	TubeBuff[5] = table[smg6];
	TubeBuff[6] = table[smg7];
	TubeBuff[7] = table[smg8];
	
	//添加小数点
	if((mode == 1) || (mode == 5) || (mode == 4))
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