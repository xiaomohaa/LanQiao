#include <stc15.h>
#include "sys.h"

bit flag_temper = 0;
bit flag_led = 0;
bit flag_10ms = 0;
u8 Time_temper[] = {1, 5, 30, 60};	//储存时间间隔
u8 time_index = 0;			//时间间隔索引
u16 Realtemper[11];			//存储温度
u8 temper_index = 0;		//温度索引
u8 hour = 0x23, min = 0x59, sec = 0x50;		//时间
u8 mode = 0;

void main(void)
{
	int temp;
	
	AllInit();
	Start18B20();
	EA = 0;
	DS1302Init();
	Timer0Init();
	EA = 1;
	while(1)
	{
		if(flag_temper)			//时间到了采集温度
		{
			flag_temper = 0;
			Get18B20Temp(&temp);
			temp >>= 4;
			if((temp >= 0) && (temp <= 60))
			{
				Realtemper[temper_index] = temp;
				temper_index ++;
				if(temper_index >= 10)
				{
					flag_led = 1;				//打开LED
					temper_index = 0;
					mode = 2;
				}
			}
			Start18B20();
		}
		
		if(flag_10ms)
		{
			flag_10ms = 0;
			hour = Read_Ds1302(0x85);
			min = Read_Ds1302(0x83);
			sec = Read_Ds1302(0x81);
		}
			
		KeyPress();
	}
}