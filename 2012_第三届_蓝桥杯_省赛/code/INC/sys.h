#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <stc15.h>
#include <intrins.h>
#include "iic.h"

extern bit mode;
extern bit flag_100ms;
extern u8 price;
extern u16 money, volume;

void All_Init(void);
void Timer0Init(void);

void Key_Scan(void);
void Key_drive(u8 key);
void Key_press(void);

void Nixie_Scan(void);
void Nixie_Show(void);
void Led_illume(u8 dat);


#endif