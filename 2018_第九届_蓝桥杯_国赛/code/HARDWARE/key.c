#include "sys.h"

sbit KeyOut1 = P4^4;
sbit KeyOut2 = P4^2;
sbit KeyOut3 = P3^5;
sbit KeyOut4 = P3^4;
sbit KeyIn1 = P3^0;
sbit KeyIn2 = P3^1;
sbit KeyIn3 = P3^2;
sbit KeyIn4 = P3^3;

unsigned char code KeyMap[4][4] = {{1, 2, 3, 4}, 
																	 {5, 6, 7, 8}, 
																	 {9, 10, 11, 12}, 
																	 {13, 14, 15, 16}};
unsigned char KeyBuff[4][4] = {{0xff, 0xff, 0xff, 0xff},
															 {0xff, 0xff, 0xff, 0xff}, 
															 {0xff, 0xff, 0xff, 0xff}, 
															 {0xff, 0xff, 0xff, 0xff}};
unsigned char KeySta[4][4] = {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
unsigned char KeyBackup[4][4] = {{1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}};
	
void KeyScan(void)
{
	static u8 index = 0;
	u8 i;
	
	//��ȡ��ǰ��ֵ
	KeyBuff[0][index] = (KeyBuff[0][index] << 1) | KeyIn1;
	KeyBuff[1][index] = (KeyBuff[1][index] << 1) | KeyIn2;
	KeyBuff[2][index] = (KeyBuff[2][index] << 1) | KeyIn3;
	KeyBuff[3][index] = (KeyBuff[3][index] << 1) | KeyIn4;

	for(i = 0; i < 4; i ++)
	{
		if((KeyBuff[i][index] & 0x0f) == 0x00)
			KeySta[i][index] = 0;
		else if((KeyBuff[i][index] & 0x0f) == 0x0f)
			KeySta[i][index] = 1;
		else
		{
			//��S6���º�һ�����������ɶ������ǳ�����״̬
			if(flag_S6_Press)
				flag_S6_Press = 0;
		}
	}
	
	index ++;
	index &= 0x03;
	switch(index)
	{
		case 0: KeyOut1 = 0; KeyOut4 = 1; break;
		case 1: KeyOut2 = 0; KeyOut1 = 1; break;
		case 2: KeyOut3 = 0; KeyOut2 = 1; break;
		case 3: KeyOut4 = 0; KeyOut3 = 1; break;
	}
}

void KeyPress(void)
{
	u8 i, j;
	
	for(i = 0; i < 4; i ++)
	{
		for(j = 0; j < 4; j ++)
		{
			if(KeyBackup[i][j] != KeySta[i][j])
			{
				if(KeySta[i][j] == 0)        //��������ʱ����
					KeyAction(KeyMap[i][j]);
				KeyBackup[i][j] = KeySta[i][j];
			}
		}
	}
}

void KeyAction(u8 keycode)
{
	static u8 mode_backup;
	
	if(keycode == 13)		//S4 �л���ʾ
	{
		mode ++;
		if(!flag_HShow)		//�رջ���ģʽ
		{
			if(mode == 4)
				mode = 1;
		}
		else
		{
			if(mode == 8)
				mode = 5;
		}
	}
	else if(keycode == 9)		//S5 �������ݴ洢����
	{
		if((!flag_HShow) && (mode != 4))
		{
			if(!flag_keep)
			{
				//���ݵ�ǰ����
				H_Fre = Fre;
				H_Temper_int = Temper_int;
				H_Temper_dec = Temper_dec;
				H_Vol = Vol;
				flag_keep = 1;
			}
		}
	}
	
	else if(keycode == 5)		//S6 ����/��ֵ+
	{
		if(mode != 4)				//�����ý���
		{
			if(flag_HShow)		//�رջ���ģʽ
			{
				flag_HShow = 0;
				mode = mode_backup;
			}
			else
			{
				flag_HShow = 1;
				mode_backup = mode;
				mode = 5;
			}
		}
		else if(mode == 4)		//���ý���
		{
			VTH += 1;
			flag_S6_Press = 1;
			if(VTH > 50)
				VTH = 1;
		}
	}
	
	else if(keycode == 1)	//S7
	{
		if(mode != 4)				//�ڷ����ý��水��
		{
			mode_backup = mode;
			mode = 4;
		}
		else if(mode == 4)	//�����ý��水��
		{
			mode = mode_backup;
		}
	}
}











