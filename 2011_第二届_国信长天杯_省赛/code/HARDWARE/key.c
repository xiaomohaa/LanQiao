#include "sys.h"
 
u8 KeySta[] = {1, 1, 1, 1};
u8 KeyBackup[] = {1, 1, 1, 1};
u8 KeyBuff[] = {0xff, 0xff, 0xff, 0xff};
 
void KeyScan(void)
{
	u8 i;
	
	KeyBuff[0] = (KeyBuff[0] << 1) | S7;
	KeyBuff[1] = (KeyBuff[1] << 1) | S6;
	KeyBuff[2] = (KeyBuff[2] << 1) | S5;
	KeyBuff[3] = (KeyBuff[3] << 1) | S4;
	
	for(i = 0; i < 4; i ++)
	{
		if(KeyBuff[i] == 0xff)				//Key release
			KeySta[i] = 1;
		else if(KeyBuff[i] == 0x00)		//Key press
			KeySta[i] = 0;
		else
		{}
	}
}
 
void KeyAction(unsigned char key)
{
	if(key == 0)					//S7  减下限
	{
		if(temper_low > 0)
			temper_low --;
		else
			temper_low = 0;
	}
	else if(key == 1)			//S6  减上限
	{
		if(temper_high > temper_low)
			temper_high --;
		else
			temper_high = temper_low;
	}
	else if(key == 2)			//S5  加下限
	{
		if(temper_low < temper_high)
			temper_low ++;
		else
			temper_low = temper_high;
	}
	else if(key == 3)			//S4  加上限
	{
		if(temper_high < 99)
			temper_high ++;
		else
			temper_high = 99;
	}
	flag_write = 1;
}
 
void KeyPress(void)
{
	u8 i;
	
	for(i = 0; i < 4; i ++)
	{
		if(KeySta[i] != KeyBackup[i])
		{
			if(KeySta[i] == 0)					//action when key press
				KeyAction(i);
			KeyBackup[i] = KeySta[i];
		}
	}
}
 