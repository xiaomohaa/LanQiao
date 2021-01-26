#include "sys.h"

void All_init(void)
{
	P2 = (P2 & 0x1f) | 0x80;	//��Y4C
	P0 = 0xff;		//�ر�LED
	P2 = (P2 & 0x1f) | 0xc0;	//��Y6C
	P0 = 0x00;		//�ر����������
	P2 = (P2 & 0x1f) | 0xa0;	//��Y5C
	P0 = 0x00;		//�رշ������ͼ̵���
	P2 = P2 & 0x1f;						//�ر�����573	
}

void Timer0Init(void)		//2����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x9a;		//���ö�ʱ��ֵ
	TH0 = 0xa9;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
}

void Timer0(void) interrupt 1
{
	static u16 T0count1 = 0, T0count2 = 0;
	static u8 index = 0, e2_addr = 0x00;
	
	T0count2 ++;
	
	if(Mode_Option > 0)			//800ms�������˸
	{
		T0count1 ++;
		if(T0count1 >= 400)
		{
			T0count1 = 0;
			flag_800ms = ~flag_800ms;
		}
	}
	
	if((T0count2 % 5 == 0) && (T0count2 < 30))	//ÿ10msʱдһ��E2PROM
	{
		switch(index)
		{
			case 0: Write_E2PROM(0x00, time_led[e2_addr]); break;
			case 1: Write_E2PROM(0x01, time_led[e2_addr]); break;
			case 2: Write_E2PROM(0x02, time_led[e2_addr]); break;
			case 3: Write_E2PROM(0x03, time_led[e2_addr]); break;
			case 4: Write_E2PROM(0x04, time_led[e2_addr]); break;
			default : break;
		}
		index ++;
		e2_addr ++;
		
		if(index >= 5)
		{
			index = 0;
			e2_addr  = 0;
		}
	}
	else if(T0count2 >= 30)		//60msʱ��ȡһ��ADֵ
	{
		T0count2 = 0;
		pwm = Read_AIN(0x03);
	}

	Key_Scan();
	Smg_show();
	Smg_Scan();
}

void Timer1Init(void)		//100us@11.0592MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xAE;		//���ö�ʱ��ֵ
	TH1 = 0xFB;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1��ͣ��ʱ
	ET1 = 1;
}

void Timer1(void) interrupt 3
{
	static u8 dat;
	static u16 T1count1 = 0;
	static u8 mode_backup = 0;
	static u8 index = 0, T1count2 = 0;
	static u8 mode_index = 1;
	u16 temp = 0;
	u8 hightime = 0;
	
	T1count1 ++;
	T1count2 ++;
	
	T1count2 &= 0x0f;			//��������15
	
	temp = 1000 * time_led[mode_index];		//��תʱ����
	hightime = pwm * pwm;								//�ߵ�ƽʱ��
	
	if(T1count1 >= temp)
	{
		T1count1 = 0;
		
		if(mode_index == 1)
		{
			if(mode_backup != mode_index)
			{
				dat = 0x7f;
				mode_backup = mode_index;
			}
			dat = _crol_(dat, 1);			//ģʽ1
			if(dat == 0x7f)
				mode_index ++;
		}
		
		else if(mode_index == 2)
		{
			if(mode_backup != mode_index)
			{
				dat = 0xfe;
				mode_backup = mode_index;
			}
			dat = _cror_(dat, 1);			//ģʽ2
			if(dat == 0xfe)
				mode_index ++;
		}
		
		else if(mode_index == 3)
		{
			if(mode_backup != mode_index)
			{
				index = 0;
				mode_backup = mode_index;
			}
			switch(index)
			{
				case 0: dat = 0x7e; break;		//0111 1110
				case 1: dat = 0xbd; break;		//1011 1101
				case 2: dat = 0xdb; break;
				case 3: dat = 0xe7; break;
				default : break;
			}
			index ++;
			index &= 0x03;
			if(dat == 0xe7)
				mode_index ++;
		}
		
		else if(mode_index == 4)
		{
			if(mode_backup != mode_index)
			{
				index = 0;
				mode_backup = mode_index;
			}
			switch(index)
			{
				case 0: dat = 0xe7; break;		//1110 0111
				case 1: dat = 0xdb; break;		//1011 1101
				case 2: dat = 0xbd; break;
				case 3: dat = 0x7e; break;
				default : break;
			}
			index ++;
			index &= 0x03;
			if(dat == 0x7e)
				mode_index = 1;
		}
	}
	
	if(T1count2 < hightime)			//PWM
		LED_work(dat);
	else
		LED_work(0xff);
}
