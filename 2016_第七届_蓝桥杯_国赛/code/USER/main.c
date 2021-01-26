#include <stc15.h>
#include "sys.h"

bit flag_100ms = 0, flag_1s = 0, flag_5ms = 0, flag_500ms = 0;
bit flag_change = 0, flag_write = 0;
bit flag_valtext = 0, flag_fretext = 0, flag_val = 1;
bit flag_readtime = 1;
u8 mode_time = 0, mode_val = 0, mode_fre = 0;
u8 mode = 0;
u8 Realtime[] = {0x19, 0x03, 0x15, 0x23, 0x59, 0x55, 0x05};
u8 Backuptime[] = {0x19, 0x03, 0x15, 0x23, 0x59, 0x55, 0x05};
u8 Val_time[] = {0x19, 0x03, 0x15, 0x23, 0x59, 0x55, 0x05, 0};
u8 Real_val[] = {0xdd, 20, 15, 0, 0};
u16 counter = 0, fre, fre_time;

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}


void main(void)
{
	static u8 index = 0;
	
	AllInit();
	Timer0Init();
	Timer1Init();
	InitDS1302(Realtime);
	
	if(Read_E2PROM(0) == 0xdd)
	{
		Real_val[1] = Read_E2PROM(1);
		Real_val[2] = Read_E2PROM(2);
	}
	else
	{
		Write_E2PROM(0, Real_val[0]);
		Delay5ms();
		Write_E2PROM(1, Real_val[1]);
		Delay5ms();
		Write_E2PROM(2, Real_val[2]);
		Delay5ms();
	}
	if(Read_E2PROM(3) < 2)
	{
		Val_time[7] = Read_E2PROM(3);
		Val_time[3] = Read_E2PROM(4);
		Val_time[4] = Read_E2PROM(5);
		Val_time[5] = Read_E2PROM(6);
	}

	EA = 1;
	
	while(1)
	{
		if(flag_100ms)
		{
			flag_100ms = 0;
			GetRealTime(Realtime);
			Real_val[3] = (u8)(Read_AIN(0x03) / 100);
			Real_val[4] = (u8)(Read_AIN(0x03) % 100);
			
			//测电压
			if(flag_valtext)
			{
				if(flag_val == 0)
				{
					if((Real_val[3] < Real_val[1]) && (Real_val[3] > Real_val[2]))
						flag_val = 1;
				}
				
				if((Real_val[3] >= Real_val[1])	&& flag_val)	//上限
				{
					flag_write = 1;
					Val_time[7] = 1;
					flag_val = 0;
				}
				else if((Real_val[3] <= Real_val[2]) && flag_val)	//下限
				{
					flag_write = 1;
					Val_time[7] = 0;
					flag_val = 0;
				}
				if(flag_write)		//读取电压事件时的时间
				{
					Val_time[3] = Realtime[3];
					Val_time[4] = Realtime[4];
					Val_time[5] = Realtime[5];
				}
			}
		}
		
		//写事件类型和时间
		if(flag_write)
		{
			if(flag_5ms)
			{
				flag_5ms = 0;
				switch(index)
				{
					case 0: Write_E2PROM(3, Val_time[7]); break;
					case 1: Write_E2PROM(4, Val_time[3]); break;
					case 2: Write_E2PROM(5, Val_time[4]); break;
					case 3: Write_E2PROM(6, Val_time[5]); flag_write = 0; break;
				}
				index ++;
				index &= 0x03;
			}
		}
		
		//测频
		if(flag_fretext)
		{
			if(flag_500ms)
			{
				TR0 = 0;
				flag_500ms = 0;
				counter = TH0 * 256 + TL0;
				TH0 = TL0 = 0;
				fre = counter * 2;
				fre_time = (u16)(1000000 / fre);
				TR0 = 1;
			}
		}
		
		//备份时间
		if(flag_readtime == 1)	
		{
			Backuptime[3] = Realtime[3];
			Backuptime[4] = Realtime[4];
			Backuptime[5] = Realtime[5];
		}
		
		KeyPress();
		TubeShow();
	}
}
