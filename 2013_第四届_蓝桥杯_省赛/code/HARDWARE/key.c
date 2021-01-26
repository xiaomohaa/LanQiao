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
	//手动模式
	if(mode == 0)	
	{
		if(key == 0)			//S7 切换工作状态
			mode = 1;
		else if(key == 1)	//S6 开关蜂鸣器
			flag_buzz = ~flag_buzz;
		else if(key == 2)	//S5 打开灌溉系统
			flag_relay = 1;
		else if(key == 3)	//S4 关闭灌溉系统
			flag_relay = 0;
	}
	
	//自动模式
	else if(mode == 1)
	{
		if(key == 0)			//切换工作状态
			mode = 0;
		else if(key == 1)	//S6 湿度阈值调整按键
		{
			if(flag_option == 0)			//进入设置模式
				flag_option = 1;
			else if(flag_option == 1)	//退出设置模式并写E2PROM
			{
				flag_option = 0;
				flag_write = 1;
			}
		}
		else if((key == 2) && (flag_option == 1))	//S5 +1
		{
			wet_value ++;
			if(wet_value >= 99)
				wet_value = 99;
		}
		else if((key == 3) && (flag_option == 1))	//S4 -1
		{
			wet_value --;
			if(wet_value <= 0)
				wet_value = 0;
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
}
