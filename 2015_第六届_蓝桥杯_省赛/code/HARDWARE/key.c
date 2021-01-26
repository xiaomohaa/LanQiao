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
	static u8 index = 1;
	
	if((key == 3) && (mode == 1))		//S4
	{
		index ++;
		switch(index)
		{
			case 1: Temper_time = 1; break;
			case 2: Temper_time = 5; break;
			case 3: Temper_time = 30; break;
			case 4: Temper_time = 60; break;
			default: index = 1; Temper_time = 1; break;
		}
	}
	else if((key == 2) && (mode == 1))			//S5
	{
		flag_start = 1;
		mode = 2;
	}
	else if((key == 1) && (mode == 3))			//S6
	{
		if(flag_led)
			flag_led = 0;
		temperno ++;
		if(temperno >= 10)
			temperno = 0;
	}
	else if((key == 0) && (mode == 3))			//S7
	{
		mode = 1;
		temperno = 0;
	}
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
 