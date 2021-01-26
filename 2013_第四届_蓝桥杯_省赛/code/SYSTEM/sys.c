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
	static u8 T0count1 = 0;
	
	//100ms��ʱ����
	T0count1 ++;
	if(T0count1 >= 50)			//100ms
	{
		T0count1 = 0;
		flag_100ms = 1;
	}
	
	if(flag_option == 0)
		Working_control();
	
	Nixie_Scan();
	Key_Scan();
}

void Working_control(void)
{	
	//�ֶ�ģʽ�̵������Ƴ���
	if((mode == 0) && (flag_relay == 1))
	{
		Led_illume(0xfe);
		P0 = 0x00;
		P04 = 1;									//�򿪼̵���
		P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
		P2 = P2 & 0x1f;						//�ر�����ʹ��
	}
	else if((mode == 0) && (flag_relay == 0))
	{
		Led_illume(0xff);
		P0 = 0x00;
		P04 = 0;									//�رռ̵���
		P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
		P2 = P2 & 0x1f;						//�ر�����ʹ��
	}
	
	//�ֶ�ģʽ���������Ƴ���
	if((real_wet < wet_value) && (flag_buzz == 1) && (mode == 0))
	{
		P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
		P06 = 1;									//�򿪷�����
		P2 = P2 & 0x1f;						//�ر�����ʹ��
	}
	else if(((real_wet >= wet_value) || (flag_buzz == 0)) && (mode == 0))
	{
		P06 = 0;									//�رշ�����
		P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
		P2 = P2 & 0x1f;						//�ر�����ʹ��
	}
	
	//�Զ�ģʽ�̵������Ƴ���
	if((real_wet < wet_value) && (mode == 1))
	{
		Led_illume(0xfd);
		P0 = 0x00;
		P04 = 1;									//�򿪼̵���
		P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
		P2 = P2 & 0x1f;						//�ر�����ʹ��
	}
	else if((real_wet >= wet_value) && (mode == 1))
	{
		Led_illume(0xff);
		P0 = 0x00;
		P04 = 0;									//�رռ̵���
		P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
		P2 = P2 & 0x1f;						//�ر�����ʹ��
	}
}
