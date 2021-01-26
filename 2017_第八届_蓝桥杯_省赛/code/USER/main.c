#include <stc15.h>
#include "sys.h"

u8 Led_dat = 0xff;
u8 mode = 0;
bit flag_1s = 0, flag_100ms = 0;
bit flag_temp = 0;
bit flag_led = 0;
bit flag_alarm = 1;

u8 AlarmTime[] = {0x00, 0x00, 0x00};			//闹钟时间
signed char OptionTime[] = {0, 0, 0, 0, 0, 0};		//0-2时间 3-5闹钟

u8 bufTime[] = {  //2019年3月2日 星期六 23:59:50
			0x19, 0x03, 0x02, 0x23, 0x59, 0x50, 0x06
	};	//日期时间缓冲区
	
void main(void)
{
	bit res = 0;
	int Temp = 0;						//读取当前的温度值
	int Temp_int = 999;			//温度值的整数部分
	
	All_Init();
	Timer0Init();
	Start18B20();
	InitDS1302(&bufTime);
	EA = 1;
	while(1)
	{
		KeyPress();
		Nixie_Show();
		Led_illume(Led_dat);
		
		if(flag_100ms == 1)		//每100ms更新一次时间
		{
			flag_100ms = 0;
			GetRealTime(&bufTime);					//获取当前时间
			if(flag_temp == 0)
				RefreshTime(&bufTime);					//显示日期
			
			IsAlarm(&bufTime);							//是否到了闹钟时间
			
		}
		
		if((S4 == 0) && (mode == 0) && (flag_alarm == 1) && (flag_led == 0))
		{
			flag_temp = 1;							//显示温度标志
			res = Get18B20Temp(&Temp);	//读取当前温度
			if(res)											//如果读取到
				Temp_int = Temp >> 4;			//分离出温度值整数部分
			Start18B20();								//重新启动下一次转换
			Num_deal(Temp_int);					//显示温度
		}
		else
			flag_temp = 0;
	}
}




