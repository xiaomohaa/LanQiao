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
	if(key == 0)					//S7 temperture
	{		
		if(mode != 4)
		{
			mode = 4;
			flag_500ms = 1;
			flag_temper = 1;
		}
		else
		{
			flag_temper = 0;
			mode = modebackup;
		}
	}
	else if(key == 1)			//S6 stop
	{
		sec = 0;
		min = 0;
		flag_fan = 0;
	}
	else if(key == 2)			//S5 minter plus
	{
		min ++;
		if(min > 99)
			min = 99;
	}
	else if(key == 3)			//S4	mode change
	{
		if(mode == 4)
		{
			modebackup ++;
			if(modebackup == 4)
				modebackup = 1;
		}
		else
		{
			mode ++;
			if(mode == 4)
				mode = 1;
			modebackup = mode;	//backup mode
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

