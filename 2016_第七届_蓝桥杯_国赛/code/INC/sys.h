#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <stc15.h>
#include <intrins.h>
#include "ds1302.h"
#include "iic.h"

extern bit flag_100ms, flag_1s, flag_5ms, flag_500ms;
extern bit flag_change, flag_write;
extern bit flag_valtext, flag_fretext, flag_val;
extern bit flag_readtime;
extern u8 mode_time, mode_val, mode_fre;
extern u8 mode;
extern u8 Realtime[];
extern u8 Backuptime[];
extern u8 Val_time[];
extern u8 Real_val[];
extern u16 counter, fre, fre_time;

void AllInit(void);
void Timer0Init(void);
void Timer1Init(void);

void TubeScan(void);
void TubeShow(void);

void KeyScan(void);
void KeyPress(void);


#endif



