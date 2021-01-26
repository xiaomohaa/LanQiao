#ifndef _SYS_H_
#define _SYS_H_

#include <stc15.h>
#include <intrins.h>
#include "ds1302.h"
#include "ds18b20.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

//外部变量
extern u8 Led_dat;
extern bit flag_100ms, flag_1s;
extern u8 mode;
extern bit flag_temp;
extern bit flag_led;
extern bit flag_alarm;
extern u8 RealTime[];				//时间
extern u8 AlarmTime[];			//闹钟时间
extern signed char OptionTime[];
extern u8 bufTime[];

extern u8 time_index, alarm_index;

//管脚定义
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

//函数声明
void All_Init(void);
void Timer0Init(void);
void IsAlarm(u8 *time);

void Nixie_Scan(void);
void Nixie_Show(void);
unsigned char BCDToNum(unsigned char bcd);
unsigned char NumToBCD(unsigned char num);
void Num_deal(int temp_int);
void RefreshTime(unsigned char *time);
void Led_illume(u8 dat);

void KeyScan(void);
void KeyPress(void);
void KeyAction(u8 key);
void TimeBackup(u8 *time);
void Updatatime(u8 *time);


#endif



