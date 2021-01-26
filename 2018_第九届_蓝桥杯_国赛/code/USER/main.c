#include <stc15.h>
#include "sys.h"

bit flag_5ms = 0, flag_200ms = 0, flag_500ms = 0, flag_1s = 0;
bit flag_keep = 0;
bit flag_HShow = 0;
bit flag_S6_Press = 0, flag_S6_LongPress = 0;
u8 mode = 3;
u16 Fre = 0, Temper_int = 0, Temper_dec = 0;
u16 H_Fre = 0, H_Temper_int = 0, H_Temper_dec = 0;
u8 Vol = 0, VTH = 20;
u8 H_Vol = 0;
u16 counter = 0;

void main(void)
{
	static u8 E2_index = 1;
	
	AllInit();
	Check_data();
	init_ds18b20();
	Trans_temper();
	Timer0Init();
	Timer1Init();
	
	EA = 1;
	while(1)
	{
		//RB2��ѹ��������
		if(flag_200ms)			//200ms����һ�ε�ѹ
		{
			flag_200ms = 0;
			Vol = Read_AIN(0x03);		//��ȡRB2��ѹ
		}
		
		//Ƶ�ʲ�������
		if(flag_500ms)
		{
			flag_500ms = 0;
			TR0 = 0;		//�رն�ʱ��0
			counter = TH0 * 256 + TL0;
			Fre = counter * 2;
			TH0 = TL0 = 0;
			TR0 = 1;
		}
		
		//�¶Ȳ�������
		if(flag_1s)
		{
			flag_1s = 0;
			Temper_int = Gettemper();
			Temper_dec = Temper_int & 0xf;		//����С������
			Temper_dec = Temper_dec * ((10000 / 16) / 100);
			Temper_int = Temper_int >> 4;			//������������
			
			Trans_temper();								//������һ���¶�ת��
		}
		
		//���ݱ������
		if(flag_keep)
		{
			if(flag_5ms)
			{
				flag_5ms = 0;
				switch(E2_index)
				{
					case 1: Write_E2PROM(0x00, 0x16); break;		//д���־
					case 2: Write_E2PROM(0x01, H_Fre >> 8); break;					//д��Ƶ�ʵĸ߰�λ
					case 3: Write_E2PROM(0x02, H_Fre & 0xff); break;				//д��Ƶ�ʵĵͰ�λ
					case 4: Write_E2PROM(0x03, H_Temper_int >> 8); break;		//д���¶��������ָ߰�λ
					case 5: Write_E2PROM(0x04, H_Temper_int & 0xff); break;	//д���¶��������ֵͰ�λ
					case 6: Write_E2PROM(0x05, H_Temper_dec >> 8); break;		//д���¶�С�����ָ߰�λ
					case 7: Write_E2PROM(0x06, H_Temper_dec & 0xff); break;	//д���¶�С�����ֵͰ�λ
					case 8: Write_E2PROM(0x07, H_Vol); E2_index = 0; flag_keep = 0; break;	//д���ѹֵ������дE2PROM
				}
				E2_index ++;
			}
		}
		
		KeyPress();
	}
}