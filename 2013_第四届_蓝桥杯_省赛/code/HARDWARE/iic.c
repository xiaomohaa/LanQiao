#include "sys.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}    

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

//总线启动条件
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}

//应答位控制
void IIC_Ack(bit ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	SDA = 1; 
	somenop;
}

//等待应答
bit IIC_WaitAck(void)
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
	}
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop;
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}

unsigned char Read_AIN(unsigned char chn)
{
	unsigned char dat, val;
	EA = 0;
	IIC_Start();						//IIC总线起始信号							
	IIC_SendByte(0x90);			//PCF8591的写设备地址		
	IIC_WaitAck();  		    //等待从机应答		
	IIC_SendByte(chn); 			//写入PCF8591的控制字节		
	IIC_WaitAck();  				//等待从机应答						
	IIC_Stop(); 						//IIC总线停止信号					
	
	IIC_Start();						//IIC总线起始信号									
	IIC_SendByte(0x91); 	  //PCF8591的读设备地址		
	IIC_WaitAck(); 			    //等待从机应答		
	dat = IIC_RecByte();	  //读取PCF8591通道3的数据 			
	IIC_Ack(0); 						//产生非应答信号				
	IIC_Stop(); 						//IIC总线停止信号		
	val = (dat * 50) / 255;
	val *= 2;
	EA = 1;
	
	if(val >= 100)
		val = 99;
	
	return val;	
}

void Write_E2PROM(unsigned char add, unsigned char dat)
{
	EA = 0;
  IIC_Start();
  IIC_SendByte(0xa0);	//发送器件地址
  IIC_WaitAck();
  IIC_SendByte(add);	//发送操作地址
  IIC_WaitAck();
  IIC_SendByte(dat);	//写一字节
  IIC_WaitAck();
  IIC_Stop();
  somenop;
	EA = 1;
}

unsigned char Read_E2PROM(unsigned char add)
{
  unsigned char d;
	
	IIC_Start();
	IIC_SendByte(0xa0); 	//发送器件地址
	IIC_WaitAck();
	IIC_SendByte(add);		//发送要操作的地址
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0xa1);		//发送读操作
	IIC_WaitAck();
	d = IIC_RecByte();	//读一字节
	IIC_Ack(0);
	IIC_Stop();
	return d;
}
