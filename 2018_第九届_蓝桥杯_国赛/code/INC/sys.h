#ifndef _SYS_H_
#define _SYS_H_

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

#include <stc15.h>
#include <intrins.h>
#include "ds18b20.h"
#include "iic.h"

extern bit flag_5ms, flag_200ms, flag_500ms, flag_1s;	//5ms-keep 200ms-U 500ms-F 1s-T
extern bit flag_keep;
extern bit flag_HShow;		//回显标志
extern bit flag_S6_Press, flag_S6_LongPress;	//S6长按键功能标志
extern u8 mode;						//0-OFF 1-U 2-F 3-T 4-P 5-HU 6-HF 7-HT
extern u16 Fre, Temper_int, Temper_dec;		//F T
extern u16 H_Fre, H_Temper_int, H_Temper_dec;		//H_F H_T
extern u8 Vol, VTH;				//U P
extern u8 H_Vol;				//H_U

void Check_data(void);
void AllInit(void);
void Timer0Init(void);
void Timer1Init(void);

void TubeScan(void);
void TubeShow(void);
void LedWork(u8 leddat);

void KeyScan(void);
void KeyPress(void);
void KeyAction(u8 keycode);



#endif



