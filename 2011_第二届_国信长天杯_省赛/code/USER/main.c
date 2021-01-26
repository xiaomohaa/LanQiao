#include <stc15.h>
#include "sys.h"

bit flag_200ms = 0;
bit flag_pwm = 0, flag_relay = 0;
bit flag_write = 0;
u8 temper_low = 20, temper_high = 30;
int Real_temper = 0;

void main(void)
{
	int tem = 0;
	static u8 index = 0;
	
	AllInit();
	Timer0Init();
	Start18B20();
	if(Read_E2PROM(0x02) == 0x23)
	{
		temper_high = Read_E2PROM(0x00);
		temper_low = Read_E2PROM(0x01);
	}
	EA = 1;
	while(1)
	{
		//读取温度程序
		if(flag_200ms)
		{
			flag_200ms = 0;
			Get18B20Temp(&tem);
			tem >>= 4;					//分离温度整数部分
			if((tem > 0) && (tem < 99))
				Real_temper = tem;
			Start18B20();
			
			if(flag_write)
			{
				if(index == 0)
					Write_E2PROM(0x00, temper_high);
				else if(index == 1)
					Write_E2PROM(0x01, temper_low);
				else if(index == 2)
					Write_E2PROM(0x02, 0x23);
				else if(index == 3)
					flag_write = 0;
				index ++;
			}
			else
				index = 0;
		}
		
		if(Real_temper > temper_high)
			flag_pwm = 1;
		else if(Real_temper < temper_low)
			flag_relay = 1;
		else
		{
			PWM = 1;
			flag_pwm = 0;
			flag_relay = 0;
		}
		
		KeyPress();
		TubeShow();
	}
}