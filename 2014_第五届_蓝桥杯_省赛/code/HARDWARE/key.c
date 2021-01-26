#include "sys.h"

sbit Output0 = P4^4;
sbit Output1 = P4^2;
sbit Output2 = P3^5;
sbit Output3 = P3^4;
sbit Input0 = P3^0;
sbit Input1 = P3^1;
sbit Input2 = P3^2;
sbit Input3 = P3^3;

u8 code KeyMap[4][4] = {{0, 1, 2, 19},
												{3, 4, 5, 18}, 
												{6, 7, 8, 17}, 
												{9, 0x88, 0x99, 16}};
u8 KeyBuff[4][4] = {{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff},
									{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}};
u8 KeySta[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};
u8 KeyBackup[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};
	

void KeyScan(void)
{
	static u8 index = 0;
	u8 i, j;
	
	KeyBuff[0][index] = (KeyBuff[0][index] << 1) | Input0;	//第一行
	KeyBuff[1][index] = (KeyBuff[1][index] << 1) | Input1;	//第二行
	KeyBuff[2][index] = (KeyBuff[2][index] << 1) | Input2;	//第三行
	KeyBuff[3][index] = (KeyBuff[3][index] << 1) | Input3;	//第四行
	
	index ++;
	index &= 0x03;
	switch(index)
	{
		case 0: Output3 = 1; Output0 = 0; break;
		case 1: Output0 = 1; Output1 = 0; break;
		case 2: Output1 = 1; Output2 = 0; break;
		case 3: Output2 = 1; Output3 = 0; break;
	}
	
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(KeyBuff[i][j] == 0x00)				//按键按下
				KeySta[i][j] = 0;
			else if(KeyBuff[i][j] == 0xff)	//按键松开
				KeySta[i][j] = 1;
			else		//按键抖动
			{}
		}
	}
}

void KeyAction(u8 keycode)
{
	static u8 length = 0;
	
	if((keycode >= 0) && (keycode <= 9) && (length < 4) && (flag_option == 1))
	{
		temper_range[length] = keycode;
		
		length ++;
		if(length >= 4)
		{
			length = 4;
			temper_low  = temper_range[3] + temper_range[2] * 10;
			temper_high = temper_range[1] + temper_range[0] * 10;
			
			if(temper_low > temper_high)		//检测到错误
				flag_right = 0;
		}
	}
	else if((keycode == 0x88) && ((length == 0) || (length == 4)) && (temper_low <= temper_high))
	{		
		flag_right = 1;
		if(flag_option == 0)
		{
			flag_option = 1;
			temp_backup[0] = temper_range[0];
			temp_backup[1] = temper_range[1];
			temp_backup[2] = temper_range[2];
			temp_backup[3] = temper_range[3];
			temper_range[0] = 16;
			temper_range[1] = 16;
			temper_range[2] = 16;
			temper_range[3] = 16;
			
			length = 0;
		}
		else if(flag_option == 1)
		{
			flag_option = 0;
			temp_backup[0] = temper_range[0];
			temp_backup[1] = temper_range[1];
			temp_backup[2] = temper_range[2];
			temp_backup[3] = temper_range[3];
		}
	}
	else if((keycode == 0x99) && (flag_option == 1))		//清除键
	{
		length = 0;
		temper_range[0] = 16;
		temper_range[1] = 16;
		temper_range[2] = 16;
		temper_range[3] = 16;
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
				if(KeySta[i][j] == 0)
					KeyAction(KeyMap[i][j]);
				KeyBackup[i][j] = KeySta[i][j];
			}
		}
	}
}

