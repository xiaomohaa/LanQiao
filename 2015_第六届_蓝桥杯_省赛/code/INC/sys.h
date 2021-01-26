#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <stc15.h>
#include <intrins.h>
#include "ds18b20.h"
#include "ds1302.h"

extern bit flag_start;
extern bit flag_50ms;
extern bit flag_temper;
extern bit flag_led;
extern u8 mode, Temper_time;
extern u8 temperno;
extern int Realtemper[11];
extern u8 Realtime[];

//function
void AllInit(void);
void Timer0Init(void);

void TubeScan(void);
void TubeShow(void);
void LEDLight(unsigned char dat);
unsigned char BCDToNum(unsigned char bcd);

void KeyScan(void);
void KeyAction(unsigned char key);
void KeyPress(void);

//pin
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

#endif


