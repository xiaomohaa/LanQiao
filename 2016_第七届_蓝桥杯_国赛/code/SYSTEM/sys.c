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

void Timer1Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xCD;		//���ö�ʱ��ֵ
	TH1 = 0xD4;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}

void Time1(void) interrupt 3
{
	static u16 T1count1 = 0, T1count2 = 0;
	
	T1count1 ++;
	
	if(T1count1 % 2 == 0)		//2ms
		TubeScan();
	if(T1count1 % 5 == 0)		//5ms
		flag_5ms = 1;
	if(T1count1 % 100 == 0)	//100ms
		flag_100ms = 1;
	if(T1count1 >= 500)			//500ms
	{
		T1count1 = 0;
		if(flag_fretext)
			flag_500ms = 1;
	}
	
	if(mode_time || mode_val || (mode == 3))
	{
		T1count2 ++;
		if(T1count2 >= 1000)	//1s
		{
			T1count2 = 0;
			flag_1s = ~flag_1s;
		}
	}
	else
		T1count2 = 0;
	
	KeyScan();
}

void Timer0Init(void)
{
	AUXR &= 0x7F;					//��ʱ��ʱ��12Tģʽ
  TMOD = 0x04;          //���ö�ʱ��0Ϊ16λ�Զ���װ���ⲿ����ģʽ
  TH0 = TL0 = 0;     		//���ö�ʱ��0��ʼֵ
	TR0 = 1;              //��ʱ��0��ʼ����
}

