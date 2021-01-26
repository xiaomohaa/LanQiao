#ifndef _SYS_H_
#define _SYS_H_

//头文件包含
#include <stc15.h>
#include <intrins.h>
#include "iic.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

//外部变量
extern u8 time_led[];				//LED流转间隔 *100
extern u8 Mode_led;
extern u8 pwm;
extern u8 Mode_Option;			//设置模式位  0无/1流转方式/2流转间隔
extern u8 led_lighting;			//LED亮度
extern bit flag_led;				//LED工作标志
extern bit flag_800ms;
extern bit display_pwm;

extern u8 KeySta[];			//键值存储区
extern u8 Keybackup[];

//管脚定义

//函数声明
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


