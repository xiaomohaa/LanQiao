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


void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xD4;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
}

void Timer0(void) interrupt 1
{
	static u16 t00 = 0, t01 = 0;
	u16 ledtime;
	static u8 leddat = 0xff, ledtemp = 0;
	
	t00 ++;
	t01 ++;
	
	if(t00 >= 1000)
	{
		t00 = 0;
		flag_1s = 1;
	}
	
	//����ָʾ��L2
	if(flag_error == 1)
		leddat = leddat & 0xfd;
	else
		leddat = leddat | 0x02;
	
	//ģʽָʾ��L1
	switch(temp_mode)
	{
		case 0: ledtime = 800; break;
		case 1: ledtime = 400; break;
		case 2: ledtime = 200; break;
	}
	
	if(t01 >= ledtime)
	{
		t01 = 0;
		ledtemp = ~ledtemp;
		if(ledtemp)
			leddat = leddat & 0xfe;
		else
			leddat = leddat | 0x01;
	}
	
	if(temp_mode == 2)
	{
		P0 = 0x00;
		P2 = (P2 & 0x1f) | 0xa0;
		P04 = 1;			//�򿪼̵���
		P2 = P2 & 0x1f;
	}
	else
	{
		P0 = 0x00;
		P2 = (P2 & 0x1f) | 0xa0;
		P04 = 0;			//�رռ̵���
		P2 = P2 & 0x1f;
	}
	
	LEDLight(leddat);
	TubeScan();
	KeyScan();
}
