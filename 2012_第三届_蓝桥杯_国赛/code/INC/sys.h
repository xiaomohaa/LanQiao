#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <stc15.h>
#include <intrins.h>
#include "ds1302.h"
#include "iic.h"

extern u8 mode;					//1-time
extern bit flag_100ms, flag_5s, flag_3s, flag_5ms, flag_125ms;
extern u8 hour, min, sec;
extern u8 Keyword[];
extern u8 Keyword_in[];
extern u8 Keyword_set[];
extern bit result;
extern bit flag_beep, flag_relay;
extern u8 error_times;
extern bit flag_write;
extern u16 distance;


void Check_data(void);
void AllInit(void);
void Timer0Init(void);
void Timer1Init(void);
void SendSonic(void);
sbit TX = P1^0; //发射引脚
sbit RX = P1^1; //接收引脚

void SendSonic(void);


void TubeScan(void);
void TubeShow(void);
void BeepWork(bit dat);
void RelayWork(bit dat);

void KeyScan(void);
void KeyPress(void);
void KeyAction(u8 keycode);
u8 Keyword_check(u8 *dat);


#endif



