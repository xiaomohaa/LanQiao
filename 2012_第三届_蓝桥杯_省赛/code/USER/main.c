#include <stc15.h>
#include "sys.h"

bit mode = 0;
bit flag_100ms = 0;
u8 price = 50;		//price 0.5Ԫ/L  �۸��ȫ����Ҫ*100
u16 volume = 0, money = 0;

void main(void)
{
	u16 val;
	
	All_Init();
	Timer0Init();
	EA = 1;
	while(1)
	{
		//��ȡADֵ����LED����
		if(flag_100ms)
		{
			flag_100ms = 0;
			val = Read_AIN(0x01);
			if(val < 125)
				Led_illume(0xfe);
			else
				Led_illume(0xff);
		}
		
		Key_press();
		Nixie_Show();
	}
}


