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
	if(key == 3)			//S4
	{
		if(mode == 0)
			time_index ++;
		time_index &= 0x03;
	}
	else if(key == 2)	//S5
	{
		if(mode == 0)
			mode = 1;
	}
	else if(key == 1)	//S6
	{
		if(mode == 2)
		{
			flag_led = 0;
			temper_index ++;
			if(temper_index >= 10)
				temper_index = 0;
		}
	}
	else if(key == 0)	//S7
	{
		if(mode == 2)
		{
			mode = 0;
			flag_led = 0;
		}
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
 