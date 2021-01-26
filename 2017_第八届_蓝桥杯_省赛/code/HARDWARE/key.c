#include "sys.h"

u8 time_index = 0, alarm_index = 3;

u8 KeySta[] = {1, 1, 1, 1};		//键值
u8 Keybackup[] = {1, 1, 1, 1};		//键值备份
u8 Keybuff[] = {0xff, 0xff, 0xff, 0xff};

//按键扫描
void KeyScan(void)
{
	u8 i;
	
	Keybuff[0] = (Keybuff[0] << 1) | S7;
	Keybuff[1] = (Keybuff[1] << 1) | S6;
	Keybuff[2] = (Keybuff[2] << 1) | S5;
	Keybuff[3] = (Keybuff[3] << 1) | S4;
	
	for(i = 0; i < 4; i ++)
	{
		if(Keybuff[i] == 0xff)
			KeySta[i] = 1;
		else if(Keybuff[i] == 0x00)
			KeySta[i] = 0;
		else
		{}
	}
}

//检测按键是否按下
void KeyPress(void)
{
	u8 i;
	
	for(i = 0; i < 4; i++)
	{
		if(Keybackup[i] != KeySta[i])
		{
			if(KeySta[i] == 0)			//按键按下时执行操作
				KeyAction(i);
			Keybackup[i] = KeySta[i];
		}
	}
}

//按键驱动
void KeyAction(u8 key)
{
	if(flag_led == 0)
	{
		if((key == 0) && (mode == 0))			//S7
		{
			if(mode == 0)
			{
				mode = 1;				//设置模式1
				time_index = 0;
			}
			time_index ++;
			if(time_index >= 4)
			{
				Updatatime(&bufTime);						//设置完更新时间
				InitDS1302(&bufTime);
				mode = 0;
			}
		}
		
		else if((key == 1) && (mode == 0))	//S6
		{
			if(mode == 0)
			{
				mode = 2;			//设置模式2
				alarm_index = 3;
			}
			alarm_index ++;
			if(alarm_index >= 7)
			{
				Updatatime(&bufTime);						//设置完更新时间
				InitDS1302(&bufTime);
				mode = 0;
			}
		}
		
		else if((key == 2) && (mode > 0))		//加
		{
			time_index --;
			alarm_index --;
			if(mode == 1)
			{
				OptionTime[time_index] ++;
				
				if(time_index == 0)
				{
					if(OptionTime[time_index] >= 24)		//小时上限
						OptionTime[time_index] = 0;
				}
				else
				{
					if(OptionTime[time_index] >= 60)		//分秒上限
						OptionTime[time_index] = 0;
				}
			}
			else if(mode == 2)
			{
				OptionTime[alarm_index] ++;
				
				if(alarm_index == 3)
				{
					if(OptionTime[alarm_index] >= 24)		//小时上限
						OptionTime[alarm_index] = 0;
				}
				else
				{
					if(OptionTime[alarm_index] >= 60)		//分秒上限
						OptionTime[alarm_index] = 0;
				}
			}
			time_index ++;
			alarm_index ++;
		}
		
		else if((key == 3) && (mode > 0))				//减
		{
			time_index --;
			alarm_index --;
			if(mode == 1)
			{
				OptionTime[time_index] --;
				
				if(time_index == 0)
				{
					if(OptionTime[time_index] < 0)		//小时下限
						OptionTime[time_index] = 23;
				}
				else
				{
					if(OptionTime[time_index] < 0)		//分秒下限
						OptionTime[time_index] = 59;
				}
			}
			else if(mode == 2)
			{
				OptionTime[alarm_index] --;
				
				if(alarm_index == 3)
				{
					if(OptionTime[alarm_index] < 0)		//小时下限
						OptionTime[alarm_index] = 23;
				}
				else
				{
					if(OptionTime[alarm_index] < 0)		//分秒下限
						OptionTime[alarm_index] = 59;
				}
			}
			time_index ++;
			alarm_index ++;
		}
	}
	else if(flag_led == 1)
	{
		if(++key)
		{
			flag_led = 0;
			flag_alarm = 0;
		}
	}
}

//时间备份
void TimeBackup(u8 *time)
{
	OptionTime[0] = BCDToNum(time[3]);
	OptionTime[1] = BCDToNum(time[4]);
	OptionTime[2] = BCDToNum(time[5]);
	
	OptionTime[3] = AlarmTime[0];
	OptionTime[4] = AlarmTime[1];
	OptionTime[5] = AlarmTime[2];
	
}

//更新时间
void Updatatime(u8 *time)
{
	time[3] = NumToBCD(OptionTime[0]);
	time[4] = NumToBCD(OptionTime[1]);
  time[5] = NumToBCD(OptionTime[2]);

  AlarmTime[0] = OptionTime[3];
  AlarmTime[1] = OptionTime[4];
  AlarmTime[2] = OptionTime[5];
}
