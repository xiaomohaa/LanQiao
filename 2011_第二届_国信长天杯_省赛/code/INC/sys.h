#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <stc15.h>
#include <intrins.h>
#include "ds18b20.h"
#include "iic.h"

extern bit flag_200ms;
extern bit flag_pwm, flag_relay;
extern bit flag_write;
extern u8 temper_low, temper_high;
extern int Real_temper;

//function
void AllInit(void);
void Timer0Init(void);

void TubeScan(void);
void TubeShow(void);

void KeyScan(void);
void KeyAction(unsigned char key);
void KeyPress(void);

//pin
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
sbit PWM = P3^4;

#endif


