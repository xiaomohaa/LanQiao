#include <stc15.h>
#include "sys.h"

bit mode = 1;			//0-手动模式，1-自动模式
bit flag_buzz = 1, flag_relay = 0;
bit flag_option = 0, flag_write = 0;
bit flag_100ms = 0;
u8 wet_value = 50, real_wet;
u8 Realtime[] = {0x19, 0x03, 0x11, 0x08, 0x30, 0x00, 0x02};

void main(void)
{
	static u8 index = 0;
	
	All_Init();
	InitDS1302();
	Timer0Init();
	
	if(Read_E2PROM(0) == 0x12)
		wet_value = Read_E2PROM(1);
	
	EA = 1;
	while(1)
	{
		if(flag_100ms)
		{
			flag_100ms = 0;
			
			//更新时间程序
			real_wet = Read_AIN(0x03);
			GetRealTime(Realtime);
			
			//写E2PROM程序
			if(flag_write == 1)
			{
				if(index == 0)
					Write_E2PROM(index, 0x12);
				else if(index == 1)
				{
					Write_E2PROM(index, wet_value);
					flag_write = 0;
					index = 0;
				}
				index ++;
			}
		}
		
		Key_press();
		Nixie_Show();
	}
}
