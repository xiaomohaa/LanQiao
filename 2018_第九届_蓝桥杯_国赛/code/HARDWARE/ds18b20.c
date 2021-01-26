#include "sys.h"

sbit DQ = P1^4;  //单总线接口

//单总线延时函数
void Delay_OneWire(unsigned int t)  
{
	unsigned char i;
	while(t--){
		for(i=0; i<8; i++);
	}
}


//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20设备初始化
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    	initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}

//开始温度转换
void Trans_temper()
{	
	init_ds18b20();					//初始化
	Write_DS18B20(0xcc);		//跳过ROM,因为只有一个DS18B20
	Write_DS18B20(0x44);		//启动温度转换
}

//读取温度
int Gettemper()
{
	unsigned char higt, low;	//定义高八位低八位
	unsigned int temper;			//温度值
	
	init_ds18b20();					//初始化
	Write_DS18B20(0xcc);		//跳过ROM,因为只有一个DS18B20
	Write_DS18B20(0xbe);
	
	low = Read_DS18B20();
	higt = Read_DS18B20();
	temper = ((unsigned int)higt << 8) | low;
	
	return temper;
}




