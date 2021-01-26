#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <stc15.h>
#include <intrins.h>
#include "ds18b20.h"

extern bit flag_1s;
extern bit flag_error;
extern int Realtemper;
extern u8 Lowtemper, Hightemper;
extern u8 temp_mode;
extern u8 mode;
extern u8 temp_option[];

//function
void AllInit(void);
void Timer0Init(void);

void TubeScan(void);
void TubeShow(void);
void LEDLight(unsigned char dat);

void KeyScan(void);
void KeyAction(u8 keycode);
void KeyPress(void);


#endif


