#include "sys.h"

sbit KeyOut1 = P4^4;
sbit KeyOut2 = P4^2;
sbit KeyOut3 = P3^5;
sbit KeyOut4 = P3^4;
sbit KeyIn1 = P3^0;
sbit KeyIn2 = P3^1;
sbit KeyIn3 = P3^2;
sbit KeyIn4 = P3^3;

unsigned char code KeyMap[4][4] = {{0x01, 0x02, 0x00, 0x00}, 
																	 {0x03, 0x04, 0x00, 0x00}, 
																	 {0x05, 0x06, 0x00, 0x00}, 
																	 {0x07, 0x08, 0x00, 0x00}};
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
		case 0: KeyOut4 = 1; KeyOut1 = 0; break;
		case 1: KeyOut1 = 1; KeyOut2 = 0; break;
		case 2: KeyOut2 = 1; KeyOut3 = 0; break;
		case 3: KeyOut3 = 1; KeyOut4 = 0; break;
	}
}

void KeyAction(unsigned char keycode)
{
	if(keycode == 0x01)		//S7		显示时钟
	{
		mode = 1;
		mode_time = 0;
		if(flag_readtime == 0)
		{
			flag_readtime = 1;
			if(flag_change == 1)		//如果没有修改时间就显示实时时间
			{
				Realtime[3] = Backuptime[3]; 
				Realtime[4] = Backuptime[4]; 
				Realtime[5] = Backuptime[5];
			}
			SetRealTime(Realtime);
			flag_change = 0;
		}
	}
	else if(keycode == 0x03)		//S6	电压测量
	{
		mode = 2;
		mode_val = 0;
	}
	else if(keycode == 0x02)		//S11 加
	{
		if(mode == 1)			//时间
		{
			if(mode_time > 0)
			{
				Backuptime[mode_time + 2] ++;
				
				if(mode_time == 1)
				{
					if((Backuptime[mode_time + 2] & 0x0f) > 9)	//个位大于9
						Backuptime[mode_time + 2] += 6;						//加6修正
					if(Backuptime[mode_time + 2] >= 0x24)				//时钟上限
						Backuptime[mode_time + 2] = 0x00;
				}
				else
				{				
					if((Backuptime[mode_time + 2] & 0x0f) > 9)	//个位大于9
						Backuptime[mode_time + 2] += 6;						//加6修正
					if(Backuptime[mode_time + 2] >= 0x60)				//分秒上限
						Backuptime[mode_time + 2] = 0x00;					
				}
				
				flag_change = 1;
			}
		}
		else if(mode == 2)		//电压
		{
			if(mode_val > 0)
			{
				Real_val[mode_val] += 5;
				if(Real_val[mode_val] >= 50)
					Real_val[mode_val] = 50;
				if(Real_val[2] >= Real_val[1])
					Real_val[2] = Real_val[1];
				
				Write_E2PROM(mode_val, Real_val[mode_val]);
			}
		}
	}
	else if(keycode == 0x04)		//S10 减
	{
		if(mode == 1)			//时间
		{
			if(mode_time > 0)
			{
				if(((Backuptime[mode_time + 2] & 0x0f) == 0) && (Backuptime[mode_time + 2] > 0))
					Backuptime[mode_time + 2] -= 6;
				else if(Backuptime[mode_time + 2] == 0x00)
				{
					if(mode_time == 1)
						Backuptime[mode_time + 2] = 0x23;
					else
						Backuptime[mode_time + 2] = 0x59;
				}
				else
					Backuptime[mode_time + 2] --;
				
				flag_change = 1;
			}
		}
		else if(mode == 2)		//电压
		{
			if(mode_val > 0)
			{
				if(Real_val[mode_val] >= 5)
					Real_val[mode_val] -= 5;
				else
					Real_val[mode_val] = 0;
				
				if(Real_val[1] <= Real_val[2])
					Real_val[1] = Real_val[2];
				
				Write_E2PROM(mode_val, Real_val[mode_val]);
			}
		}
	}
	else if(keycode == 0x05)		//S5		频率测量
	{
		mode = 4;
		flag_fretext = 1;
		mode_fre = 1;
	}
	else if(keycode == 0x06)		//S9		查询
	{
		mode = 3;
		flag_valtext = 0;
	}
	else if(keycode == 0x07)		//S4
	{
		if(mode == 1)		//时间
		{
			flag_readtime = 0;
			mode_time ++;
			if(mode_time > 3)
				mode_time = 1;
		}
		else if(mode == 2)		//电压
		{
			mode_time = 0;
			mode_val ++;
			if(mode_val >= 3)
				mode_val = 1;
		}
		else if(mode == 4)		//频率
		{
			mode_fre ++;
			if(mode_fre > 2)
				mode_fre = 1;
		}
	}
	
	if(mode != 3)
		flag_valtext = 1;
	if(mode != 4)
		flag_fretext = 1;
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
