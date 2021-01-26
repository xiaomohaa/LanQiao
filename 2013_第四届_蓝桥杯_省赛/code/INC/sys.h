#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <stc15.h>
#include <intrins.h>
#include "ds1302.h"
#include "iic.h"

extern bit mode;
extern bit flag_buzz, flag_relay;
extern bit flag_option, flag_write;
extern bit flag_100ms;
extern u8 wet_value, real_wet;
extern u8 Realtime[];


//function
void All_Init(void);
void Timer0Init(void);
void Working_control(void);

void Key_Scan(void);
void Key_drive(u8 key);
void Key_press(void);

void Nixie_Scan(void);
void Nixie_Show(void);
unsigned char BCDToNum(unsigned char bcd);
void Led_illume(u8 dat);

#endif
