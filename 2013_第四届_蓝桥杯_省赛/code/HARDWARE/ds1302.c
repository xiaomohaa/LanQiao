#include "sys.h"

sbit DS1302_IO = P2^3;
sbit DS1302_CK = P1^7;
sbit DS1302_CE = P1^3;

void DS1302ByteWrite(unsigned char dat) 
{
	unsigned char mask;
	DS1302_IO = 1;					//拉低IO总线
	for(mask = 0x01; mask != 0; mask <<= 1)	//低位在前，逐位移出
	{
		if((dat&mask) != 0)		//首先输出该位数据
			DS1302_IO = 1;
		else
			DS1302_IO = 0;
		DS1302_CK = 1;				//拉高时钟线
		DS1302_CK = 0;				//拉低时钟线，完成一个位的操作
	}
	DS1302_IO = 1;  				//写完之后确保释放IO总线
}  

unsigned char DS1302ByteRead(void)
{
	unsigned char mask, dat = 0;
	
	for(mask = 0x01; mask != 0; mask <<= 1)	//低位在前，逐位读取
	{
		if(DS1302_IO)			//首先读取此时的IO引脚，并设置dat中的对应位
			dat |= mask;

		DS1302_CK = 1;		//拉高时钟
		DS1302_CK = 0;		//再拉低时钟，完成一个位的操作
	}
	return dat;					//返回读到的字节数据
} 

void DS1302SingleWrite(unsigned char reg, unsigned char dat)     
{
	DS1302_CE = 1;					//使能片选信号
	DS1302ByteWrite((reg << 1) | 0x80);	//发送写寄存器指令
	DS1302ByteWrite(dat);		//写入字节数据
	DS1302_CE = 0;					//除能片选信号
}

unsigned char DS1302SingleRead(unsigned char reg)
{
	unsigned char dat;
	
	DS1302_CE = 1;					//使能片选信号
	DS1302ByteWrite((reg << 1) | 0x81);	//发送读寄存器指令
	dat = DS1302ByteRead();	//读取字节数据
	DS1302_CE = 0;					//除能片选信号
	
	DS1302_IO = 0;					//单字节读必须加的！
	
	return dat;         
}

void DS1302BurstWrite(unsigned char *dat)
{
	unsigned char i;
	
	DS1302_CE = 1;
	DS1302ByteWrite(0xBE);				//发送突发写寄存器指令
	for(i = 0; i < 7; i ++)				//连续写入8字节数据
		DS1302ByteWrite(*dat++);        
	DS1302_CE = 0;      
}

void DS1302BurstRead (unsigned char *dat)
{
	unsigned char i;
	
	DS1302_CE = 1;
	DS1302ByteWrite(0xBF);				//发送突发读寄存器指令
	for(i = 0; i < 7; i++)				//连续读取8个字节
		dat[i] = DS1302ByteRead();      
	DS1302_CE = 0;  
	
	DS1302_IO = 0;								//突发读必须加
}	

void GetRealTime(unsigned char *time)
{
	unsigned char buf[8];
	
	DS1302BurstRead(buf);
	time[0] = buf[6];
	time[1] = buf[4];
	time[2] = buf[3];
	time[3] = buf[2];
	time[4] = buf[1];
	time[5] = buf[0];
	time[6] = buf[5];
}

void SetRealTime(unsigned char *time)
{
	unsigned char buf[8];
	
	buf[7] = 0;
	buf[6] = time[0];
	buf[4] = time[1];
	buf[3] = time[2];
	buf[2] = time[3];
	buf[1] = time[4];
	buf[0] = time[5];
	buf[5] = time[6];
	DS1302BurstWrite(buf);
}

void InitDS1302(void)
{
	unsigned char dat;
	signed char i = 7;
	unsigned char timeinit[8] = {0x19, 0x03, 0x11, 0x08, 0x30, 0x00, 0x02};
	
	DS1302_CE = 0;								//初始化DS1302通信引脚
	DS1302_CK = 0;
	dat = DS1302SingleRead(0);			//读取秒寄存器
	DS1302SingleWrite(7, 0x00);  	//撤销写保护以允许写入数据
	SetRealTime(&timeinit);					//设置DS1302时间
	DS1302SingleWrite(7, 0x80);  	//写保护以禁止写入数据
}

