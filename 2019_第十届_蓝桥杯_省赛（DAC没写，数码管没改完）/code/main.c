#include "sys.h"

bit flag200ms = 0, flag500ms = 0;
bit flagval = 0, flagled = 1, flagtube = 1;
unsigned char mode = 2;
unsigned int FRE, VAL;
unsigned char LEDDAT = 0xff, led = 0xff;

void main(void)
{
	All_Init();
	Timer1Init();
	Timer0Init();
	EA = 1;
	while(1)
	{
		//200ma��ȡһ��AD
		if(flag200ms)
		{
			flag200ms = 0;
			VAL = Read_AIN(0x03);		//��ȡAIN3������
		}
		//500ms��Ƶһ��
		if(flag500ms)
		{
			flag500ms = 0;
			TR0 = 0;								//�رն�ʱ��0����
			FRE = TH0 * 256 + TL0;	//ͳ��500ms��������
			FRE *= 2;								//����2����1s��������������Ƶ��
			TH0 = TL0 = 0;					//�����ʱ��0��������
			TR0 = 1; 								//��ʱ��0���¿�ʼ����
		}

		//���S5���£�val���ǹ̶���2.0V
		if(flagval == 0)
			VAL = 200;

		//���LED��ʾ��
		if(flagled)
		{
			switch(mode)
			{
				case 1: LEDDAT = (LEDDAT | 0x03) & 0xfd; break;			//ģʽ1Ϊ��Ƶģʽ
				case 2: LEDDAT = (LEDDAT | 0x03) & 0xfe; break;			//ģʽ2Ϊ���ѹģʽ
			}

			//����ǲ�Ƶģʽ�����ݷ�Χѡ�����LED
			if(mode == 1)		
			{
				LEDDAT = LEDDAT | 0x10;		//�ر�L5
				LEDDAT = LEDDAT | 0x04;		//�ر�L3					
				if(FRE < 1000)
					LEDDAT = LEDDAT | 0x08;	//�ر�L4
				else if((FRE >= 1000) && (FRE < 5000))
					LEDDAT = (LEDDAT | 0x08) & 0xf7;	//��L4
				else if((FRE >= 5000) && (FRE < 10000))
					LEDDAT = LEDDAT | 0x08;		//�ر�L4
				else if(FRE > 10000)
					LEDDAT = (LEDDAT | 0x08) & 0xf7;	//��L4
			}
			//����ǲ��ѹģʽ�����ݵ�ѹ��Χѡ�����LED
			else if(mode == 2)
			{
				//���ǹ̶����2.0V״̬
				if(flagval)	
				{
					LEDDAT = LEDDAT & 0xef;		//��L5
					LEDDAT = LEDDAT | 0x08;		//�ر�L4
					if(VAL < 150)
						LEDDAT = LEDDAT | 0x04;	//�ر�L3
					else if((VAL >= 150) && (VAL < 250))
						LEDDAT = (LEDDAT | 0x04) & 0xfb;	//��L3
					else if((VAL > 250) && (VAL < 350))
						LEDDAT = LEDDAT | 0x04;	//�ر�L3
					else if(VAL > 350)
						LEDDAT = (LEDDAT | 0x04) & 0xfb;		//��L3
				}
				//�̶����2.0V״̬
				else 
				{
					LEDDAT = LEDDAT | 0x10;			//�ر�L5
					LEDDAT = LEDDAT | 0x04;			//�ر�L3
				}
			}
		}
		//LED�ر�״̬��LEDȫ��
		else
			LEDDAT = 0xff;
		
		led = LEDDAT;

		KeyPress();
		TubeScan();
	}
}

