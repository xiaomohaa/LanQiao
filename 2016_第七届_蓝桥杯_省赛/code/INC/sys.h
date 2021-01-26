#ifndef _SYS_H_
#define _SYS_H_

#include <stc15.h>
#include <intrins.h>
#include "ds18B20.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

//External variables
extern bit flag_2ms, flag_500ms;
extern bit flag_fan, flag_temper;
extern u8 mode, modebackup;
extern signed char min, sec;
extern int temper_int;

//Pin
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

sbit PWM = P3^4;

//Function
void AllInit(void);
void Timer0Init(void);

void TubeScan(void);
void TubeShow(void);
void LEDWork(void);

void KeyScan(void);
void KeyAction(unsigned char key);
void KeyPress(void);

#endif

