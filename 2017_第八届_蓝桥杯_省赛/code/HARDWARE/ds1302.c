#include "sys.h"

sbit DS1302_IO = P2^3;
sbit DS1302_CK = P1^7;
sbit DS1302_CE = P1^3;

/*******************************************************************************
* 函数名	：DS1302ByteWrite
* 输入值	：unsigned char dat
* 返回值	：none
* 作者		：小默haa
* 时间		：2019年3月1日
* 功能描述：发送一个字节到DS1302通信总线上
* 备注		：
*******************************************************************************/
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

/*******************************************************************************
* 函数名	：DS1302ByteRead
* 输入值	：none
* 返回值	：unsigned char dat
* 作者		：小默haa
* 时间		：2019年3月1日
* 功能描述：由DS1302通信总线上读取一个字节
* 备注		：返回读到的字节数据
*******************************************************************************/
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

/*******************************************************************************
* 函数名	：DS1302SingleWrite
* 输入值	：unsigned char reg, unsigned char dat
* 返回值	：none
* 作者		：小默haa
* 时间		：2019年3月1日
* 功能描述：用单次写操作向某一寄存器写入一个字节
* 备注		：reg为寄存器地址，dat为待写入字节
*******************************************************************************/
void DS1302SingleWrite(unsigned char reg, unsigned char dat)     
{
	DS1302_CE = 1;					//使能片选信号
	DS1302ByteWrite((reg << 1) | 0x80);	//发送写寄存器指令
	DS1302ByteWrite(dat);		//写入字节数据
	DS1302_CE = 0;					//除能片选信号
}

/*******************************************************************************
* 函数名	：DS1302SingleRead
* 输入值	：unsigned char reg
* 返回值	：unsigned char dat
* 作者		：小默haa
* 时间		：2019年3月1日
* 功能描述：用单次读操作从某一寄存器读取一个字节
* 备注		：reg为寄存器地址，返回值dat为读到的字节
*******************************************************************************/
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

/*******************************************************************************
* 函数名	：DS1302BurstWrite
* 输入值	：unsigned char *dat
* 返回值	：none
* 作者		：小默haa
* 时间		：2019年3月1日
* 功能描述：用突发模式连续写入8个寄存器数据
* 备注		：reg为寄存器地址，返回值dat为读到的字节
*******************************************************************************/
void DS1302BurstWrite(unsigned char *dat)
{
	unsigned char i;
	
	DS1302_CE = 1;
	DS1302ByteWrite(0xBE);				//发送突发写寄存器指令
	for(i = 0; i < 7; i ++)				//连续写入8字节数据
		DS1302ByteWrite(*dat++);        
	DS1302_CE = 0;      
}

/*******************************************************************************
* 函数名	：DS1302BurstRead
* 输入值	：unsigned char *dat
* 返回值	：none
* 作者		：小默haa
* 时间		：2019年3月1日
* 功能描述：用突发模式连续读取8个寄存器的数据
* 备注		：dat为读到的字节
*******************************************************************************/
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

/*******************************************************************************
* 函数名	：GetRealTime
* 输入值	：struct sTime *time
* 返回值	：none
* 作者		：小默haa
* 时间		：2019年3月1日
* 功能描述：读取DS1302时间
* 备注		：
*******************************************************************************/
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

/*******************************************************************************
* 函数名	：SetRealTime
* 输入值	：u8 *time
* 返回值	：none
* 作者		：小默haa
* 时间		：2019年3月1日
* 功能描述：设置DS1302时间
* 备注		：
*******************************************************************************/
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

/*******************************************************************************
* 函数名	：InitDS1302
* 输入值	：struct sTime *time
* 返回值	：none
* 作者		：小默haa
* 时间		：2019年3月1日
* 功能描述：DS1302初始化
* 备注		：
*******************************************************************************/
void InitDS1302(unsigned char *time)
{
	unsigned char dat;
	signed char i = 7;
	unsigned char timeinit[8];
	
	while(i >= 0)
	{
		timeinit[i] = time[i];
		i --;
	}
	
	DS1302_CE = 0;								//初始化DS1302通信引脚
	DS1302_CK = 0;
	dat = DS1302SingleRead(0);			//读取秒寄存器
	DS1302SingleWrite(7, 0x00);  	//撤销写保护以允许写入数据
	SetRealTime(&timeinit);					//设置DS1302时间
//	DS1302SingleWrite(7, 0x80);  	//写保护以禁止写入数据
}

