#ifndef __DS18B20_H
#define __DS18B20_H


//单总线延时函数
void Delayus(unsigned int us);
bit Get18B20Ack(void);
void DS18B20Write(unsigned char dat);
unsigned char DS18B20Read(void);
bit Start18B20();
bit Get18B20Temp(int *temp);


#endif
