#include "sys.h"

void AllInit(void)
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x00;
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 = P2 & 0x1f;
}


void Timer0Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x9A;		//���ö�ʱ��ֵ
	TH0 = 0xA9;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
}

void Timer0(void) interrupt 1
{
	static u16 t00 = 0;
	static u8 t01 = 0, t02 = 0;
	u16 temp;		//�洢�ɼ�ʱ������
	
	t00 ++;
	t01 ++;
	t02 ++;
	
	//��ȡDS1302
	if(t02 >= 5)		//10ms
	{
		t02 = 0;
		flag_10ms = 1;
	}
	
	
	//�ɼ��¶�ʱ����
	temp = Time_temper[time_index] * 500;		//ʱ������
	if(mode == 1)
	{
		if(t00 >= temp)
		{
			t00 = 0;
			flag_temper = 1;
		}
	}
	else
		t00 = 0;
	
	//LED��˸
	if(flag_led)
	{
		if(t01 < 100)		//200ms
			LEDLight(0xfe);
		else
		{
			LEDLight(0xff);
			if(t01 >= 200)	//400ms
				t01 = 0;
		}
	}
	else
	{
		t01 = 0;
		LEDLight(0xff);
	}
	
	KeyScan();
	TubeShow();
	TubeScan();
}

