#include "sys.h"

u8 KeySta[] = {1, 1, 1, 1};			//键值存储区
u8 Keybackup[] = {1, 1, 1, 1};	//键值备份区

sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;
sbit S7 = P3^0;

//按键扫描函数，在定时器中断里调用
void Key_Scan(void)
{
	static u8 Keybuff[] = {0xff, 0xff, 0xff, 0xff};	//按键缓冲区
	u8 i = 0;
	
	Keybuff[0] = (Keybuff[0] << 1) | S7;
	Keybuff[1] = (Keybuff[1] << 1) | S6;
	Keybuff[2] = (Keybuff[2] << 1) | S5;
	Keybuff[3] = (Keybuff[3] << 1) | S4;
	
	for(i = 0; i < 4; i++)
	{
		if(Keybuff[i] == 0xff)				//按键松开
			KeySta[i] = 1;
		else if(Keybuff[i] == 0x00)		//按键按下
			KeySta[i] = 0;
		else				//键值不稳定
		{}
	}
}

void Key_drive(u8 key)
{	
	if(key == 0)
	{
		if(TR1 == 0)
			TR1 = 1;
		else
			TR1 = 0;
	}
	
	else if(key == 1)
	{
		Mode_Option ++;
		if(Mode_Option >= 3)
			Mode_Option = 0;
	}
	
	else if(key == 2)
	{
		if(Mode_Option == 1)
		{
			Mode_led ++;
			if(Mode_led >= 4)				//模式最大4
				Mode_led = 4;
		}
		else if(Mode_Option == 2)
		{
			time_led[Mode_led] ++;
			if(time_led[Mode_led] >= 12)		//最大1200ms
				time_led[Mode_led] = 12;
		}
	}
	
	else if(key == 3)
	{
		if(Mode_Option == 1)
		{
			Mode_led --;
			if(Mode_led <= 1)				//模式最小1
				Mode_led = 1;
		}
		
		else if(Mode_Option == 2)
		{
			time_led[Mode_led] --;
			if(time_led[Mode_led] <= 4)		//最小400ms
				time_led[Mode_led] = 4;
		}
	}
}

//检测按键是否按下，在main函数调用
void Key_press(void)
{
	u8 i;
	
	for(i = 0; i < 4; i ++)
	{
		if(KeySta[i] != Keybackup[i])
		{
			if(Keybackup[i] != 0)		//按键松开时操作
				Key_drive(i);
			Keybackup[i] = KeySta[i];
		}
	}
	
	if(Mode_Option == 0)
	{
		if(KeySta[3] == 0)
			display_pwm = 1;
		else
			display_pwm = 0;
	}
}


