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
	if(Read_E2PROM(0x00) == 0x16)
	{
		H_Fre = ((u16)(Read_E2PROM(0x01) << 8)) | (Read_E2PROM(0x02));
		H_Temper_int = ((u16)(Read_E2PROM(0x03) << 8)) | (Read_E2PROM(0x04));
		H_Temper_dec = ((u16)(Read_E2PROM(0x05) << 8)) | (Read_E2PROM(0x06));
		H_Vol = Read_E2PROM(0x07);
	}
	else
	{
		 Write_E2PROM(0x00, 0x16); Delay5ms();
		 Write_E2PROM(0x01, 0); Delay5ms();
		 Write_E2PROM(0x02, 0); Delay5ms();
		 Write_E2PROM(0x03, 0); Delay5ms();
		 Write_E2PROM(0x04, 0); Delay5ms();
		 Write_E2PROM(0x05, 0); Delay5ms();
		 Write_E2PROM(0x06, 0); Delay5ms();
		 Write_E2PROM(0x07, 0);
	}
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

void Timer0Init(void)
{
	AUXR &= 0x7F;							//��ʱ��ʱ��12Tģʽ
  TMOD = 0x04;                    //���ö�ʱ��0Ϊ16λ�Զ���װ���ⲿ����ģʽ
  TH0 = TL0 = 0x00;               //���ö�ʱ��0��ʼֵ
  TR0 = 1;                        //��ʱ��0��ʼ����
  ET0 = 1;                        //����ʱ��0�ж�
}

void Timer1Init(void)		//1����@12.000MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x20;		//���ö�ʱ��ֵ
	TH1 = 0xD1;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}

void Timer1(void) interrupt 3
{
	static u16 t00 = 0, t01 = 0, t03 = 0;
	static u8 t02 = 0;
	static u8 leddat = 0;
	static bit temp = 1;
	u8 ledtemp;
	
	t00 ++; t01 ++; t02 ++;
	
	if(flag_keep)
	{
		if(t02 % 5 == 0)
			flag_5ms = 1;
	}
	
	if(t02 >= 200)
	{
		t02 = 0;
		flag_200ms = 1;
		
		if(Vol > VTH)
			temp = ~temp;		//ʵ��L8��˸
	}
	if(t01 >= 500)
	{
		t01 = 0;
		flag_500ms = 1;
	}
	if(t00 >= 1000)
	{
		t00 = 0;
		flag_1s = 1;
	}
	
	//S6������800ms������
	if(flag_S6_Press)
	{
		t03 ++;
		if(t03 >= 800)
		{
			t03 = 0;
			flag_S6_LongPress = 1;
		}
		if(flag_S6_LongPress)
		{
			if(t03 >= 100)
			{
				t03 = 0;
				VTH += 1;
				if(VTH > 50)
					VTH = 1;
			}
		}
	}
	else
	{
		t03 = 0;
		flag_S6_LongPress = 0;
	}
	
	if(mode == 3)				//�¶Ȳ��� L1��
		leddat = 0xfe;
	else if(mode == 2)	//Ƶ�ʲ��� L2��
		leddat = 0xfd;
	else if(mode == 1)	//��ѹ���� L3��
		leddat = 0xfb;
	else
		leddat = 0xff;
	
	//ʵ��L8��˸����
	if(mode != 4)
	{
		if(Vol > VTH)
		{
			ledtemp = (u8)temp << 7;
			ledtemp = 0xff - ledtemp;
			leddat &= ledtemp;
		}
	}
	
	KeyScan();
	TubeShow();
	TubeScan();
	LedWork(leddat);	
}







