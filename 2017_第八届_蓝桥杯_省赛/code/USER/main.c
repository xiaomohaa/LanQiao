#include <stc15.h>
#include "sys.h"

u8 Led_dat = 0xff;
u8 mode = 0;
bit flag_1s = 0, flag_100ms = 0;
bit flag_temp = 0;
bit flag_led = 0;
bit flag_alarm = 1;

u8 AlarmTime[] = {0x00, 0x00, 0x00};			//����ʱ��
signed char OptionTime[] = {0, 0, 0, 0, 0, 0};		//0-2ʱ�� 3-5����

u8 bufTime[] = {  //2019��3��2�� ������ 23:59:50
			0x19, 0x03, 0x02, 0x23, 0x59, 0x50, 0x06
	};	//����ʱ�仺����
	
void main(void)
{
	bit res = 0;
	int Temp = 0;						//��ȡ��ǰ���¶�ֵ
	int Temp_int = 999;			//�¶�ֵ����������
	
	All_Init();
	Timer0Init();
	Start18B20();
	InitDS1302(&bufTime);
	EA = 1;
	while(1)
	{
		KeyPress();
		Nixie_Show();
		Led_illume(Led_dat);
		
		if(flag_100ms == 1)		//ÿ100ms����һ��ʱ��
		{
			flag_100ms = 0;
			GetRealTime(&bufTime);					//��ȡ��ǰʱ��
			if(flag_temp == 0)
				RefreshTime(&bufTime);					//��ʾ����
			
			IsAlarm(&bufTime);							//�Ƿ�������ʱ��
			
		}
		
		if((S4 == 0) && (mode == 0) && (flag_alarm == 1) && (flag_led == 0))
		{
			flag_temp = 1;							//��ʾ�¶ȱ�־
			res = Get18B20Temp(&Temp);	//��ȡ��ǰ�¶�
			if(res)											//�����ȡ��
				Temp_int = Temp >> 4;			//������¶�ֵ��������
			Start18B20();								//����������һ��ת��
			Num_deal(Temp_int);					//��ʾ�¶�
		}
		else
			flag_temp = 0;
	}
}




