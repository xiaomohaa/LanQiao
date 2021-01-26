#include <stc15.h>
#include "sys.h"

bit flag_125ms = 0, flag_200ms = 0, flag_500ms = 0, flag_1s = 0;
u8 mode = 2;
u16 Temper_int = 0, Temper_dec = 0;
u16 distance = 0;

void main(void)
{	
	u16 t;
	
	AllInit();
	init_ds18b20();
	Trans_temper();
//	Timer0Init();
	Timer1Init();
	
	EA = 1;
	
	while(1)
	{
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
		
		//���������
		if(mode == 2)
		{
			if(flag_125ms)
			{
				flag_125ms = 0;
				SendSonic();			//����50KHz�ĳ�����	
				TR1 = 1;					//������ʱ��1��ʱ
				while((RX == 1) && (TF1 == 0));	//������յ��ز����߶�ʱ��1���
				TR1 = 0;					//�رն�ʱ��1
				if(TF1 == 1)
				{
					TF1 = 0;
					distance = 999;	//����999
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
		}
		
		KeyPress();
	}
}