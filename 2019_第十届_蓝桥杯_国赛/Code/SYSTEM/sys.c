#include "sys.h"

void Delay5ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 59;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}

void Check_data(void)
{

}

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

void Timer0Init(void)		//1����@12.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x20;		//���ö�ʱ��ֵ
	TH0 = 0xD1;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;		//����ʱ��0�ж�
}


void Timer1Init(void)		//1����@12.000MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0;		//���ö�ʱ��ֵ
	TH1 = 0;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1��ʼ��ʱ
	ET1 = 0;
}

void Timer0(void) interrupt 1
{
	static u16 t00 = 0;
	
	t00 ++;
	
	if(t00 % 125 == 0)
		flag_125ms = 1;
	
	if(t00 >= 1000)
	{
		t00 = 0;
		flag_1s = 1;
	}
	
	
	KeyScan();
	TubeShow();
	TubeScan();
	LedWork(0xff);	
}







