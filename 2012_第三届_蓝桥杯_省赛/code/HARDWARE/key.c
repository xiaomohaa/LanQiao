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
	if(key == 0)					//S7 开始接水
		mode = 1;
	else if((key == 1) && (mode == 1))			//S6 停止接水
	{
		mode = 0;
		money = volume / 2;
		volume = 0;
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
}
