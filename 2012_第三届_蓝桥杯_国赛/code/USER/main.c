#include <stc15.h>
#include "sys.h"

u8 mode = 1;
bit flag_100ms = 0, flag_5s = 0, flag_3s = 0, flag_5ms = 0, flag_125ms = 0;
u8 hour = 0x06, min = 0x59, sec = 0x00;
u8 Keyword[] = {6, 5, 4, 3, 2, 1};
u8 Keyword_in[] = {16, 16, 16, 16, 16, 16};
u8 Keyword_set[] = {16, 16, 16, 16, 16, 16};
bit result = 0;
bit flag_beep = 0, flag_relay = 0;
u8 error_times = 0;
bit flag_write = 0;
u16 distance = 100;

void main(void)
{
	u8 index = 1;
	u16 t;
	
	AllInit();
	Check_data();
	DS1302Init();
	Timer0Init();
	Timer1Init();
	EA = 1;
	while(1)
	{
		if(flag_100ms)
		{
			flag_100ms = 0;
			
			//读取DS1302时间
			hour = Read_Ds1302(0x85);
			min  = Read_Ds1302(0x83);
			sec  = Read_Ds1302(0x81);
		}
					
		if(mode == 1 || mode == 2)
		{
			if(hour < 0x07 || hour > 0x21)
				mode = 2;
			else
				mode = 1;
		}
		
		//写E2PROM
		if(flag_write)
		{
			if(flag_5ms)
			{
				flag_5ms = 0;
				switch(index)
				{
					case 1: Write_E2PROM(0x00, 0x18); break;
					case 2: Write_E2PROM(0x01, Keyword[0]); break;
					case 3: Write_E2PROM(0x02, Keyword[1]); break;
					case 4: Write_E2PROM(0x03, Keyword[2]); break;
					case 5: Write_E2PROM(0x04, Keyword[3]); break;
					case 6: Write_E2PROM(0x05, Keyword[4]); break;
					case 7: Write_E2PROM(0x06, Keyword[5]); flag_write = 0; index = 1; break;
				}
				index ++;
			}
		}
		
		//超声波
		if(flag_125ms)
		{
			flag_125ms = 0;
			SendSonic();			//发送50KHz的超声波	
			TR1 = 1;					//开启定时器1计时
			while((RX == 1) && (TF1 == 0));	//如果接收到回波或者定时器1溢出
			TR1 = 0;					//关闭定时器1
			if(TF1 == 1)
			{
				TF1 = 0;
				distance = 999;	//距离999
			}
			else
			{
				t = TH1;
				t <<= 8;
				t |= TL1;
				distance = (u16)(t * 0.017);
			}
			TH1 = 0;
			TL1 = 0;
		}
		
		if(mode == 1)
		{
			if(distance < 31)
				flag_relay = 1;
		}
		
		KeyPress();
	}
}