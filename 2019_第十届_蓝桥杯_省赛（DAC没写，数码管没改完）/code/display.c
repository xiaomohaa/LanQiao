#include "sys.h"

unsigned char code table[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 
                            0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e,
														0xff, 0xbf, 0xc1};
unsigned char TubeBuff[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
unsigned char smg1, smg2, smg3, smg4, smg5, smg6, smg7, smg8;

void TubeShow(void)
{
	static unsigned char index = 0;
	
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

void TubeScan(void)
{
	//如果为测频显示，这样显示
	if(mode == 1)
	{
		smg1 = 15;		//显示F
		smg2 =smg3 = 16;	 	//熄灭
		smg4 = FRE / 10000;
		smg5 = (FRE % 10000) / 1000;
		smg6 = (FRE % 1000) / 100;
		smg7 = (FRE % 100) / 10;
		smg8 = FRE % 10;
	
		//消0
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
					{
						smg7 = 16;
					}
				}
			}
		}
	}
	//如果是测电压模式
	else if(mode == 2)
	{
		smg1 = 18;			 		//显示U
		smg2 = smg3 = smg4 = smg5 = 16;	 //熄灭
		smg6 = VAL / 100;
		smg7 = (VAL % 100) / 10;
		smg8 = VAL % 10;
	}

	//如果关闭数码管时全部熄灭
	if(flagtube == 0)
		smg1 = smg2 = smg3 = smg4 = smg5 = smg6 = smg7 = smg8 = 16;

	if((mode == 2) && (flagtube == 1))
		TubeBuff[5] = table[smg6] & 0x7f;		//测电压时加上小数点
	else
			TubeBuff[5] = table[smg6];		
	TubeBuff[0] = table[smg1];
	TubeBuff[1] = table[smg2];
	TubeBuff[2] = table[smg3];
	TubeBuff[3] = table[smg4];
	TubeBuff[4] = table[smg5];
	TubeBuff[6] = table[smg7];
	TubeBuff[7] = table[smg8];
}

void LedLight(void)
{
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0x80;
	P0 = led;			
	P2 = P2 & 0x1f;
}