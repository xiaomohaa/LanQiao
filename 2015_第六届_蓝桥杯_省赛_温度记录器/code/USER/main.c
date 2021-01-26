#include <stc15.h>
#include "sys.h"

bit flag_temper = 0;
bit flag_led = 0;
bit flag_10ms = 0;
u8 Time_temper[] = {1, 5, 30, 60};	//����ʱ����
u8 time_index = 0;			//ʱ��������
u16 Realtemper[11];			//�洢�¶�
u8 temper_index = 0;		//�¶�����
u8 hour = 0x23, min = 0x59, sec = 0x50;		//ʱ��
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
		if(flag_temper)			//ʱ�䵽�˲ɼ��¶�
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
					flag_led = 1;				//��LED
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