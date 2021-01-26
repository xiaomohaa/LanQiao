#ifndef _DS1302_H_
#define _DS1302_H_

void Write_Ds1302_Byte(unsigned  char temp);
void Write_Ds1302( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302 ( unsigned char address );
void DS1302Init(void);

#endif
