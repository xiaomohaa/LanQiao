#ifndef _SYS_H_
#define _SYS_H_

#include <stc15.h>
#include <intrins.h>
#include "iic.h"

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

extern bit flag200ms, flag500ms;
extern bit flagval, flagled, flagtube;
extern unsigned char mode;		//mode=为测频模式， mode =2为测电压模式
extern unsigned int FRE, VAL;
extern unsigned char LEDDAT, led;

void All_Init(void);
void Timer1Init(void);
void Timer0Init(void);

void KeyScan(void);
void KeyPress(void);
void KeyAction(unsigned char key);

void TubeScan(void);
void TubeShow(void);
void LedLight(void);



#endif