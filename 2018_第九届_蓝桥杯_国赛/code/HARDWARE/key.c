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
			//当S6按下后，一旦发生按键松动，则不是长按键状态
			if(flag_S6_Press)
				flag_S6_Press = 0;
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
	static u8 mode_backup;
	
	if(keycode == 13)		//S4 切换显示
	{
		mode ++;
		if(!flag_HShow)		//关闭回显模式
		{
			if(mode == 4)
				mode = 1;
		}
		else
		{
			if(mode == 8)
				mode = 5;
		}
	}
	else if(keycode == 9)		//S5 备份数据存储数据
	{
		if((!flag_HShow) && (mode != 4))
		{
			if(!flag_keep)
			{
				//备份当前数据
				H_Fre = Fre;
				H_Temper_int = Temper_int;
				H_Temper_dec = Temper_dec;
				H_Vol = Vol;
				flag_keep = 1;
			}
		}
	}
	
	else if(keycode == 5)		//S6 回显/阈值+
	{
		if(mode != 4)				//非设置界面
		{
			if(flag_HShow)		//关闭回显模式
			{
				flag_HShow = 0;
				mode = mode_backup;
			}
			else
			{
				flag_HShow = 1;
				mode_backup = mode;
				mode = 5;
			}
		}
		else if(mode == 4)		//设置界面
		{
			VTH += 1;
			flag_S6_Press = 1;
			if(VTH > 50)
				VTH = 1;
		}
	}
	
	else if(keycode == 1)	//S7
	{
		if(mode != 4)				//在非设置界面按下
		{
			mode_backup = mode;
			mode = 4;
		}
		else if(mode == 4)	//在设置界面按下
		{
			mode = mode_backup;
		}
	}
}











