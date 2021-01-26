#ifndef __DS1302_H
#define __DS1302_H

void DS1302ByteWrite(unsigned char dat);
unsigned char DS1302ByteRead(void);
void DS1302SingleWrite(unsigned char reg, unsigned char dat);
unsigned char DS1302SingleRead(unsigned char reg);
void DS1302BurstWrite(unsigned char *dat);
void DS1302BurstRead (unsigned char *dat);
void GetRealTime(unsigned char *time);
void SetRealTime(unsigned char *time);
void InitDS1302(void);

#endif
