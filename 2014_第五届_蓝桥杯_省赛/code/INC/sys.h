#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <stc15.h>
#include <intrins.h>
#include "ds18b20.h"

extern u8 temper_low, temper_high, mode;
extern u8 temper_range[], temp_backup[];
extern int Realtemper;
extern bit flag_option;
extern bit flag_right;
extern bit flag_200ms;

//function
void AllInit(void);
void Timer0Init(void);

void TubeScan(void);
void TubeShow(void);
void LEDLight(unsigned char dat);

void KeyScan(void);
void KeyAction(u8 keycode);
void KeyPress(void);

//pin
sbit relay = P0^5;

#endif


