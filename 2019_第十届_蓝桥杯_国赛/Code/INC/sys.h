#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <stc15.h>
#include <intrins.h>
#include "ds18b20.h"
#include "iic.h"

extern bit flag_125ms, flag_1s;
extern u8 mode;						//0-OFF 1-U 2-F 3-T 4-P 5-HU 6-HF 7-HT
extern u16 Temper_int, Temper_dec;		//F T
extern u16 distance;


void Check_data(void);
void AllInit(void);
void Timer0Init(void);
void Timer1Init(void);

void TubeScan(void);
void TubeShow(void);
void Deal_distance(u16 distance);
void LedWork(u8 leddat);

void KeyScan(void);
void KeyPress(void);
void KeyAction(u8 keycode);

sbit TX = P1^0; //发射引脚
sbit RX = P1^1; //接收引脚

void SendSonic(void);



#endif



