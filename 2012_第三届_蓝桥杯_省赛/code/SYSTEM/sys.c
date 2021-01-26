#include "sys.h"

void All_Init(void)
{
	P2 = (P2 & 0x1f) | 0x80;	//��Y4C��LED��
	P0 = 0xff;			//�ر�LED
	P2 = (P2 & 0x1f) | 0xe0;	//��Y7C������ܣ�
	P0 = 0xff;			//�ر������
	P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
	P0 = 0x00;			//�رշ��������̵���
	P2 = P2 & 0x1f;			//�ر�����ʹ��
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
	static u8 T0count1 = 0, T0count2 = 0;
	
	//���ƽ�ˮ����
	if(mode == 1)
	{
		T0count1 ++;
		if(T0count1 >= 50)
		{
			T0count1 = 0;
			volume ++;
		}
	}
	
	//100ms��ʱ���Ƴ���
	T0count2 ++;
	if(T0count2 >= 50)
	{
		T0count2 = 0;
		flag_100ms = 1;
	}
	
	//�̵������Ƴ���
	if(mode == 1)
	{
		if(volume < 9999)
		{
			P0 = 0x00;
			P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
			P04 = 1;
			P2 = P2 & 0x1f;			//�ر�����ʹ��
		}
		else if(volume >= 9999)
		{
			mode = 0;
			money = volume / 2;
			volume = 0;
		}
	}
	else
	{
		P0 = 0x00;
		P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
		P04 = 0;
		P2 = P2 & 0x1f;			//�ر�����ʹ��
	}
	
	Key_Scan();
	Nixie_Scan();
}