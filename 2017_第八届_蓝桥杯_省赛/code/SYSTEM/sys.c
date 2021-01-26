#include "sys.h"

//�ر���������
void All_Init(void)
{
	P2 = (P2 & 0x1f) | 0x80;	//��Y4C��LED��
	P0 = 0xff;					//�ر�LED
	P2 = (P2 & 0x1f) | 0xe0;	//��Y7C������ܣ�
	P0 = 0xff;					//�ر������
	P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
	P0 = 0x00;					//�رշ��������̵���
	P2 = P2 & 0x1f;
}

//��ʱ��0��ʼ��
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

//��ʱ��0�жϳ���
void Time0(void) interrupt 1
{
	static unsigned char t01 = 0, t02 = 0;
	static unsigned int t03 = 0, t04 = 0;
	
	t01 ++;
	t03 ++;
	if(t01 >= 50)
	{
		flag_100ms = 1;
		t01 = 0;
		
		if(mode == 0)
			TimeBackup(&bufTime);					//����ʱ��
	}
	
	if(flag_led == 1)
	{
		t02 ++;
		if(t02 <= 100)
			Led_dat = 0xfe;
		else
		{
			Led_dat = 0xff;
			if(t02 >= 200)
				t02 = 0;
		}
	}
	else if(flag_led == 0)
	{
		t02 = 0;
		Led_dat = 0xff;
	}
	
	if(t03 >= 500)
	{
		t03 = 0;
		flag_1s  = ~flag_1s;
	}
	
	if(flag_led == 1)
	{
		t04 ++;
		
		if(t04 >= 2500)
		{
			t04 = 0;
			flag_led = 0;
		}
	}
	else
		t04 = 0;
	
	KeyScan();

	Nixie_Scan();
}

void IsAlarm(u8 *time)
{
	if((time[5] == AlarmTime[2]))
	{
		if(time[4] == AlarmTime[1])
		{
			if(time[3] == AlarmTime[0])
			{
				if(flag_alarm)
					flag_led = 1;
			}
		}
	}
	else
		flag_alarm = 1;
}


