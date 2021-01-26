#include <stc15.h>
#include "sys.h"

u8 time_led[] = {0x11, 0x04, 0x04, 0x04, 0x04};				//LED��ת��� *100
u8 Mode_led = 1;
u8 pwm = 4;

u8 Mode_Option = 0;			//����ģʽλ  0��/1��ת��ʽ/2��ת���
u8 led_lighting = 0;		//LED����
bit flag_led = 0;				//LED������־
bit flag_800ms = 1;
bit display_pwm = 0;

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}

void main(void)
{
	u8 flag_dat = 0;
	
	All_init();
	Timer0Init();
	Timer1Init();
	flag_dat = Read_E2PROM(0x00);			//��ȡE2PROMд���־λ
	if(flag_dat == 0x11)
	{
		time_led[0] = flag_dat;
		Delay5ms();
		time_led[1] = Read_E2PROM(0x01);
		Delay5ms();
		time_led[2] = Read_E2PROM(0x02);
		Delay5ms();
		time_led[3] = Read_E2PROM(0x03);
		Delay5ms();
		time_led[4] = Read_E2PROM(0x04);
		Delay5ms();
	}
	LED_work(0xff);
	EA = 1;
	while(1)
	{
		Key_press();
	}
}

