#ifndef _SYS_H_
#define _SYS_H_

//ͷ�ļ�����
#include <stc15.h>
#include <intrins.h>
#include "iic.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

//�ⲿ����
extern u8 time_led[];				//LED��ת��� *100
extern u8 Mode_led;
extern u8 pwm;
extern u8 Mode_Option;			//����ģʽλ  0��/1��ת��ʽ/2��ת���
extern u8 led_lighting;			//LED����
extern bit flag_led;				//LED������־
extern bit flag_800ms;
extern bit display_pwm;

extern u8 KeySta[];			//��ֵ�洢��
extern u8 Keybackup[];

//�ܽŶ���

//��������
void All_init(void);
void Timer0Init(void);
void Timer1Init(void);

void Smg_Scan(void);
void Smg_show();
void Smg_show();
void LED_work(u8 dat);

void Key_Scan(void);
void Key_drive(u8 key);
void Key_press(void);

#endif


