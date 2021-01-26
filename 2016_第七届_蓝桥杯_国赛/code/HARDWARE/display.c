#include "sys.h"

unsigned char code table[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 
                            0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e,
														0xff, 0xbf};
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
	if(mode == 1)
	{
		if(mode_time == 0)
		{
			smg1 = Realtime[3] >> 4;
			smg2 = Realtime[3] & 0x0f;
			smg4 = Realtime[4] >> 4;
			smg5 = Realtime[4] & 0x0f;
			smg7 = Realtime[5] >> 4;
			smg8 = Realtime[5] & 0x0f;
			
			if(smg8 % 2 == 0)
				smg3 = smg6 = 16;
			else
				smg3 = smg6 = 17;
		}
		else if(mode_time > 0)
		{
			smg1 = Backuptime[3] >> 4;
			smg2 = Backuptime[3] & 0x0f;
			smg3 = smg6 = 17;
			smg4 = Backuptime[4] >> 4;
			smg5 = Backuptime[4] & 0x0f;
			smg7 = Backuptime[5] >> 4;
			smg8 = Backuptime[5] & 0x0f;
			if(flag_1s)
			{
				switch(mode_time)
				{
					case 1: smg1 = smg2 = 16; break;
					case 2: smg4 = smg5 = 16; break;
					case 3: smg7 = smg8 = 16; break;
				}
			}
		}
	}
	else if(mode == 2)
	{
		if(mode_val == 0)
		{
			smg1 = smg3 = 17;
			smg2 = 1;
			smg4 = 16;
			smg5 = Real_val[3] / 10;
			smg6 = Real_val[3] % 10;
			smg7 = Real_val[4] / 10;
			smg8 = Real_val[4] % 10;
		}
		else
		{
			smg1 = Real_val[1] / 10;
			smg2 = Real_val[1] % 10;
			smg3 = smg4 = 0;
			smg5 = Real_val[2] / 10;
			smg6 = Real_val[2] % 10;
			smg7 = smg8 = 0;
			
			if(flag_1s)
			{
				if(mode_val == 1)
					smg1 = smg2 = smg3 = smg4 = 16;
				else if(mode_val == 2)
					smg5 = smg6 = smg7 = smg8 = 16;
			}
		}
	}
	else if(mode == 3)
	{
		if(flag_1s)
		{
			smg1 = smg2 = smg3 = smg4 = smg5 = smg6 = 16;
			smg7 = 0;
			smg8 = Val_time[7];
		}
		else
		{
			smg1 = Val_time[3] >> 4;
			smg2 = Val_time[3] &0x0f;
			smg3 = smg6 = 17;
			smg4 = Val_time[4] >> 4;
			smg5 = Val_time[4] &0x0f;
			smg7 = Val_time[5] >> 4;
			smg8 = Val_time[5] &0x0f;
		}
	}
	else if(mode == 4)
	{
		smg1 = smg3 = 17;
		smg2 = 2;
		
		if(mode_fre == 1)
		{
			smg4 = fre / 10000;
			smg5 = (fre % 10000) / 1000;
			smg6 = (fre % 1000) / 100;
			smg7 = (fre % 100) /10;
			smg8 = fre % 10;
		}
		else if(mode_fre == 2)
		{
			smg4 = fre_time / 10000;
			smg5 = (fre_time % 10000) / 1000;
			smg6 = (fre_time % 1000) / 100;
			smg7 = (fre_time % 100) /10;
			smg8 = fre_time % 10;
		}
	}
	else
		smg1 = smg2 = smg3 = smg4 = smg5 = smg6 = smg7 = smg8 = 16;
		
	TubeBuff[0] = table[smg1];
	TubeBuff[1] = table[smg2];
	TubeBuff[2] = table[smg3];
	TubeBuff[3] = table[smg4];
	TubeBuff[4] = table[smg5];
	TubeBuff[5] = table[smg6];
	TubeBuff[6] = table[smg7];
	TubeBuff[7] = table[smg8];
}

