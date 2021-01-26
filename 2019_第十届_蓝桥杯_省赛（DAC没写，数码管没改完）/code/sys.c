#include "sys.h"

void All_Init(void)
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;				
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;			
	P2 = (P2 & 0x1f) | 0xa0;	
	P0 = 0x00;				
	P2 = P2 & 0x1f;
}

//��ʱ��1��ʼ��
void Timer1Init(void)		//2����@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x30;		//���ö�ʱ��ֵ
	TH1 = 0xF8;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}

//��ʱ��1�жϷ������
void Timer1(void) interrupt 3
{
	static unsigned char t00 = 0, t01 = 0;

	t00 ++;		//200ms
	t01 ++;		//500ms

	if(t00 >= 100)
	{
		t00 = 0;
		flag200ms = 1;
	}

	if(t01 >= 250)
	{
		t01 = 0;
		flag500ms = 1;
	} 

	LedLight();
	KeyScan();
	TubeShow();
}

//��ʱ��0��ʼ��Ϊ����ģʽ
void Timer0Init(void)
{
	AUXR &= 0x7F;					//��ʱ��ʱ��12Tģʽ
  TMOD = 0x04;          //���ö�ʱ��0Ϊ16λ�Զ���װ���ⲿ����ģʽ
  TH0 = TL0 = 0;     		//���ö�ʱ��0��ʼֵ
	TR0 = 1;              //��ʱ��0��ʼ����
}