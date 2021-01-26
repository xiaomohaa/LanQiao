#include "sys.h"

sbit KeyOut1 = P4^4;
sbit KeyOut2 = P4^2;
sbit KeyOut3 = P3^5;
sbit KeyOut4 = P3^4;
sbit KeyIn1 = P3^0;
sbit KeyIn2 = P3^1;
sbit KeyIn3 = P3^2;
sbit KeyIn4 = P3^3;

unsigned char code KeyMap[4][4] = {{0, 1, 2, 3}, 
																	 {4, 5, 6, 7}, 
																	 {8, 9, 11, 22}, 
																	 {88, 88, 33, 44}};
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
	static u8 index_kw = 0;
	
	//数字键
	if(mode == 2 && !flag_beep)
	{
		if(keycode >= 0 && keycode <= 9)
		{
			if(index_kw < 6)
			{
				Keyword_in[index_kw] = keycode;
				index_kw ++;
			}
		}	
	}
	
	//设置界面
	else if(mode == 3)
	{
		if(keycode >= 0 && keycode <= 9)
		{
			if(index_kw < 6)
			{
				Keyword_set[index_kw] = keycode;
				index_kw ++;
			}
		}	
	}
	
	//输入新密码
	else if(mode == 4)
	{
		if(keycode >= 0 && keycode <= 9)
		{
			if(index_kw < 6)
			{
				Keyword_set[index_kw] = keycode;
				index_kw ++;
			}
		}	
	}
	
	//确认
	if(keycode == 33)
	{
		//开门
		if(mode == 2)
		{
			result = Keyword_check(&Keyword_in);
			if(result == 1)
			{
				flag_relay = 1;
				error_times = 0;
			}
			else if(result == 0)
			{
				error_times ++;
				if(error_times >= 3)
					flag_beep = 1;
			}
			index_kw = 0;
			Keyword_in[0] = Keyword_in[1] = 16;
			Keyword_in[2] = Keyword_in[3] = 16;
			Keyword_in[4] = Keyword_in[5] = 16;
		}
		
		//设置
		else if(mode == 3)
		{
			result = Keyword_check(&Keyword_set);
			if(result == 1)
				mode = 4;
			
			index_kw = 0;
			Keyword_set[0] = Keyword_set[1] = 16;
			Keyword_set[2] = Keyword_set[3] = 16;
			Keyword_set[4] = Keyword_set[5] = 16;
		}
		
		else if (mode == 4)
		{
			index_kw = 0;
			Keyword[0] = Keyword_set[0];
			Keyword[1] = Keyword_set[1];
			Keyword[2] = Keyword_set[2];
			Keyword[3] = Keyword_set[3];
			Keyword[4] = Keyword_set[4];
			Keyword[5] = Keyword_set[5];
			Keyword_set[0] = Keyword_set[1] = 16;
			Keyword_set[2] = Keyword_set[3] = 16;
			Keyword_set[4] = Keyword_set[5] = 16;
			mode = 1;
			flag_write = 1;
		}
	}
	
	//设置键
	if(keycode == 11)
	{
		mode = 3;
	}
	
	//复位
	if(keycode == 22 && mode < 3)
	{
		Keyword[0] = 6;
		Keyword[1] = 5;
		Keyword[2] = 4;
		Keyword[3] = 3;
		Keyword[4] = 2;
		Keyword[5] = 1;
	}
	
	//退出
	if(keycode == 44)
	{
		if(mode >= 3 && index_kw < 6)
		{
			mode = 1;
			index_kw = 0;
			Keyword_set[0] = Keyword_set[1] = 16;
			Keyword_set[2] = Keyword_set[3] = 16;
			Keyword_set[4] = Keyword_set[5] = 16;
		}
	}
}

u8 Keyword_check(u8 *dat)
{
	u8 i, temp = 0;
	
	for(i = 0; i < 6; i ++)
	{
		if(dat[i] != Keyword[i])
		{
			temp = 0;
			break;
		}
		else
			temp = 1;
	}
	
	return temp;
}











