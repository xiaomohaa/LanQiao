#include "sys.h"

unsigned char code table[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 
                            0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e,
														0xff, 0xbf};
u8 TubeBuff[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
u8 smg1, smg2, smg3, smg4, smg5, smg6, smg7, smg8;

void TubeScan(void)
{
	static u8 index = 0;
	
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = (0x01 << index);
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = TubeBuff[index];
	P2 = P2 & 0x1f;
	
	index ++;
	index &= 0x07;
}

void TubeShow(void)
{
	if(flag_option)
	{
		smg1 = smg6 = 17;	   //-
		smg4 = smg5 = 16;    //Ï¨Ãð
		smg2 = temper_range[0];
		smg3 = temper_range[1];
		smg7 = temper_range[2];
		smg8 = temper_range[3];
	}
	else if(flag_option == 0)
	{
		smg1 = smg3 = 17;					//-
		smg4 = smg5 = smg6 = 16;	//Ï¨Ãð
		smg2 = mode;
		smg7 = Realtemper / 10;
		smg8 = Realtemper % 10;
	}
	
	TubeBuff[0] = table[smg1];
	TubeBuff[1] = table[smg2];
	TubeBuff[2] = table[smg3];
	TubeBuff[3] = table[smg4];
	TubeBuff[4] = table[smg5];
	TubeBuff[5] = table[smg6];
	TubeBuff[6] = table[smg7];
	TubeBuff[7] = table[smg8];
}

void LEDLight(unsigned char dat)
{
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0x80;
	P0 = dat;
	P2 = P2 & 0x1f;
}

