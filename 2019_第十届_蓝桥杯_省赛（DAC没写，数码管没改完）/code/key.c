#include "sys.h"

unsigned char keysta[] = {1, 1, 1, 1};
unsigned char keybackup[] = {1, 1, 1, 1};

unsigned char keybuff[] = {0xff, 0xff, 0xff, 0xff};

void KeyScan(void)
{
	unsigned char i;

	keybuff[0] = (keybuff[0] << 1) | S4;
	keybuff[1] = (keybuff[1] << 1) | S5;
	keybuff[2] = (keybuff[2] << 1) | S6;
	keybuff[3] = (keybuff[3] << 1) | S7;

	for(i = 0; i < 4; i ++)
	{
		if(keybuff[i] == 0xff)
			keysta[i] = 1;
		else if(keybuff[i] == 0x00)
			keysta[i] = 0;
	}
}

void KeyPress(void)
{
	unsigned char i;

	for(i = 0; i < 4; i ++)
	{
		if(keysta[i] != keybackup[i])
		{
			if(keysta[i] == 0)
				KeyAction(i);
			keybackup[i] = keysta[i];
		}
	}
}

void KeyAction(unsigned char key)
{
	//S4�����л���Ƶ�Ͳ��ѹģʽ
	if(key == 0)	
	{
		if(mode == 1)	//�����ģʽ���л���ģʽ2
			mode = 2;
		else if(mode == 2)	//�����ģʽ2���л���ģʽ
			mode = 1;
	}

	//S5Ϊ�Ƿ�Ϊ�̶����2.0V
	else if(key == 1)
		flagval = !flagval;

	//S6�����Ƿ��LED
	else if(key == 2)	
		flagled = ~flagled;

	//S7�����Ƿ���������ʾ
	else if(key == 3)	//S7
		flagtube = !flagtube;
}



