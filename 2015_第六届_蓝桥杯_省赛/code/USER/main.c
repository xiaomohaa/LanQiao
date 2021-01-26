#include <stc15.h>
#include "sys.h"

bit flag_start = 0;
bit flag_50ms = 0;
bit flag_temper = 0;
bit flag_led = 0;
u8 mode = 1, Temper_time = 1;
u8 temperno = 0;
int Realtemper[11];
u8 Realtime[] = {0x19, 0x03, 0x07, 0x23, 0x59, 0x50, 0x04};


void main(void)
{
	u8 index = 0, t = 1, leddat = 0xff;
	int temp = 0;
	
	AllInit();
	Start18B20();
	EA = 0;
	Timer0Init();
	InitDS1302(&Realtime);
	EA = 1;
	while(1)
	{
		KeyPress();
		
		if(flag_50ms)
		{
			flag_50ms = 0;
			GetRealTime(&Realtime);		//读取1302时间
			if(flag_led)
			{
				leddat = 0xfe | t;
				t = ~t;
			}
			else
				leddat = 0xff;
		}
		
		if(flag_temper == 1)
		{
			flag_temper = 0;
			Get18B20Temp(&temp);
			temp >>= 4;
			Realtemper[index] = temp;
			index ++;
			Start18B20();
			if(index >= 10)
			{
				mode = 3;						//自动切换到温度显示界面
				flag_start = 0;			//停止采集温度
				flag_led = 1;		//显示温度
			}
		}
		
		if(flag_start == 0)
			index = 0;
		
		LEDLight(leddat);
	}
}