#include "sys.h"

u8 KeySta[] = {1, 1, 1, 1};			//��ֵ�洢��
u8 Keybackup[] = {1, 1, 1, 1};	//��ֵ������

sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;
sbit S7 = P3^0;

//����ɨ�躯�����ڶ�ʱ���ж������
void Key_Scan(void)
{
	static u8 Keybuff[] = {0xff, 0xff, 0xff, 0xff};	//����������
	u8 i = 0;
	
	Keybuff[0] = (Keybuff[0] << 1) | S7;
	Keybuff[1] = (Keybuff[1] << 1) | S6;
	Keybuff[2] = (Keybuff[2] << 1) | S5;
	Keybuff[3] = (Keybuff[3] << 1) | S4;
	
	for(i = 0; i < 4; i++)
	{
		if(Keybuff[i] == 0xff)				//�����ɿ�
			KeySta[i] = 1;
		else if(Keybuff[i] == 0x00)		//��������
			KeySta[i] = 0;
		else				//��ֵ���ȶ�
		{}
	}
}

void Key_drive(u8 key)
{
	//�ֶ�ģʽ
	if(mode == 0)	
	{
		if(key == 0)			//S7 �л�����״̬
			mode = 1;
		else if(key == 1)	//S6 ���ط�����
			flag_buzz = ~flag_buzz;
		else if(key == 2)	//S5 �򿪹��ϵͳ
			flag_relay = 1;
		else if(key == 3)	//S4 �رչ��ϵͳ
			flag_relay = 0;
	}
	
	//�Զ�ģʽ
	else if(mode == 1)
	{
		if(key == 0)			//�л�����״̬
			mode = 0;
		else if(key == 1)	//S6 ʪ����ֵ��������
		{
			if(flag_option == 0)			//��������ģʽ
				flag_option = 1;
			else if(flag_option == 1)	//�˳�����ģʽ��дE2PROM
			{
				flag_option = 0;
				flag_write = 1;
			}
		}
		else if((key == 2) && (flag_option == 1))	//S5 +1
		{
			wet_value ++;
			if(wet_value >= 99)
				wet_value = 99;
		}
		else if((key == 3) && (flag_option == 1))	//S4 -1
		{
			wet_value --;
			if(wet_value <= 0)
				wet_value = 0;
		}
	}
}

//��ⰴ���Ƿ��£���main��������
void Key_press(void)
{
	u8 i;
	
	for(i = 0; i < 4; i ++)
	{
		if(KeySta[i] != Keybackup[i])
		{
			if(Keybackup[i] != 0)		//�����ɿ�ʱ����
				Key_drive(i);
			Keybackup[i] = KeySta[i];
		}
	}
}
