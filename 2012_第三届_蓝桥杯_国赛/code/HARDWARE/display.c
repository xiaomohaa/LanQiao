#include "sys.h"

unsigned char code table[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 
                            0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e,
														0xff, 0xbf, 0xc1, 0x89, 0x8c};	//18-U 19-H 20-P
unsigned char TubeBuff[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
unsigned char smg1, smg2, smg3, smg4, smg5, smg6, smg7, smg8;

void TubeScan(void)
{
	static u8 index = 0;
	
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
	P2 = P2 & 0x1f;
	
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
	//时钟显示程序
	if(mode == 1)
	{
		smg1 = hour >> 4;
		smg2 = hour & 0x0f;
		smg4 = min >> 4;
		smg5 = min & 0x0f;
		smg7 = sec >> 4;
		smg8 = sec & 0x0f;
		
		if(smg8 % 2 == 0)
			smg3 = smg6 = 16;
		else
			smg3 = smg6 = 17;
	}
	
	//输入密码界面
	else if(mode == 2)
	{
		smg1 = smg2 = 17;
		smg3 = Keyword_in[0];
		smg4 = Keyword_in[1];
		smg5 = Keyword_in[2];
		smg6 = Keyword_in[3];
		smg7 = Keyword_in[4];
		smg8 = Keyword_in[5];
		
	}
	
	//设置界面
	else if(mode == 3)
	{
		smg1 = 16;
		smg2 = 17;
		smg3 = Keyword_set[0];
		smg4 = Keyword_set[1];
		smg5 = Keyword_set[2];
		smg6 = Keyword_set[3];
		smg7 = Keyword_set[4];
		smg8 = Keyword_set[5];
	}
	
	else if(mode == 4)
	{
		smg1 = 17;
		smg2 = 16;
		smg3 = Keyword_set[0];
		smg4 = Keyword_set[1];
		smg5 = Keyword_set[2];
		smg6 = Keyword_set[3];
		smg7 = Keyword_set[4];
		smg8 = Keyword_set[5];
	}
	
	//测试用
//	else if(mode == 5)
//	{
//		smg1 = smg2 = smg3 = smg4 = smg5 = 16;
//		smg6 = distance / 100;
//		smg7 = (distance % 100) / 10;
//		smg8 = distance % 10;
//	
//		if(smg6 == 0)
//		{
//			smg6 = 16;
//			if(smg7 == 0)
//				smg7 = 16;
//		}
//	}
	
	TubeBuff[0] = table[smg1];
	TubeBuff[1] = table[smg2];
	TubeBuff[2] = table[smg3];
	TubeBuff[3] = table[smg4];
	TubeBuff[4] = table[smg5];
	TubeBuff[5] = table[smg6];
	TubeBuff[6] = table[smg7];
	TubeBuff[7] = table[smg8];
}

void BeepWork(bit dat)
{
	P06 = 0;
	P2 = (P2 & 0x1f) | 0xa0;
	P06 = dat;
	P2 = P2 & 0x1f;
}

void RelayWork(bit dat)
{
	P04 = 0;
	P2 = (P2 & 0x1f) | 0xa0;
	P04 = dat;
	P2 = P2 & 0x1f;
}


