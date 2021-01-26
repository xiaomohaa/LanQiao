#include "sys.h"

sbit KeyOut1 = P4^4;
sbit KeyOut2 = P4^2;
sbit KeyOut3 = P3^5;
sbit KeyOut4 = P3^4;
sbit KeyIn1 = P3^0;
sbit KeyIn2 = P3^1;
sbit KeyIn3 = P3^2;
sbit KeyIn4 = P3^3;

unsigned char code KeyMap[4][4] = {{1, 2, 3, 4}, 
																	 {5, 6, 7, 8}, 
																	 {9, 10, 11, 12}, 
																	 {13, 14, 15, 16}};
unsigned char KeyBuff[4][4] = {{0xff, 0xff, 0xff, 0xff},
															 {0xff, 0xff, 0xff, 0xff}, 
															 {0xff, 0xff, 0xff, 0xff}, 
															 {0xff, 0xff, 0xff, 0xff}};
unsigned char KeySta[4][4] = {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
unsigned char KeyBackup[4][4] = {{1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}};
	
void KeyScan(void)
{
	static u8 index = 0;
	u8 i;
	
	//获取当前键值
	KeyBuff[0][index] = (KeyBuff[0][index] << 1) | KeyIn1;
	KeyBuff[1][index] = (KeyBuff[1][index] << 1) | KeyIn2;
	KeyBuff[2][index] = (KeyBuff[2][index] << 1) | KeyIn3;
	KeyBuff[3][index] = (KeyBuff[3][index] << 1) | KeyIn4;

	for(i = 0; i < 4; i ++)
	{
		if((KeyBuff[i][index] & 0x0f) == 0x00)
			KeySta[i][index] = 0;
		else if((KeyBuff[i][index] & 0x0f) == 0x0f)
			KeySta[i][index] = 1;
		else
		{
		}
	}
	
	index ++;
	index &= 0x03;
	switch(index)
	{
		case 0: KeyOut1 = 0; KeyOut4 = 1; break;
		case 1: KeyOut2 = 0; KeyOut1 = 1; break;
		case 2: KeyOut3 = 0; KeyOut2 = 1; break;
		case 3: KeyOut4 = 0; KeyOut3 = 1; break;
	}
}

void KeyPress(void)
{
	u8 i, j;
	
	for(i = 0; i < 4; i ++)
	{
		for(j = 0; j < 4; j ++)
		{
			if(KeyBackup[i][j] != KeySta[i][j])
			{
				if(KeySta[i][j] == 0)        //按键按下时操作
					KeyAction(KeyMap[i][j]);
				KeyBackup[i][j] = KeySta[i][j];
			}
		}
	}
}

void KeyAction(u8 keycode)
{
	keycode = 1;
}











