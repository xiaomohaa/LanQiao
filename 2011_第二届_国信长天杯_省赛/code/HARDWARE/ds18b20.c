 #include "sys.h"

sbit DS18B20_IO = P1^4;

void Delayus(unsigned int us)
{
	do{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}while(--us);
}

bit Get18B20Ack(void)
{
	bit ack;
	
	DS18B20_IO = 0;			//产生500us的复位脉冲
	Delayus(500);
//	EA = 0;							//禁止总中断
	ET0 = 0;
	DS18B20_IO = 1;			//延时60us
	Delayus(60);
//	EA = 1;							//重新使能总中断
	ET0 = 1;
	ack = DS18B20_IO;		//读取存在脉冲
	while(!DS18B20_IO);	//等待存在脉冲结束
	
	return ack; 	
}

void DS18B20Write(unsigned char dat)
{
 	unsigned char mask;
						
	for(mask = 0x01; mask != 0; mask <<= 1)	//低位在先，依次移出8个bit
	{
//		EA = 0;
		ET0 = 0;
		DS18B20_IO = 0;				//产生2us低电平脉冲
		Delayus(2);
	 	if(dat & mask)				//输出该bit值
		 	DS18B20_IO = 1;
		else
		 	DS18B20_IO = 0;
//		EA = 1;
		ET0 = 1;
		Delayus(60);					//延时60us
		DS18B20_IO = 1;				//拉高通信引脚
	}
}

unsigned char DS18B20Read(void)
{
 	unsigned char mask, dat = 0;
	
	for(mask = 0x01; mask != 0; mask <<= 1)	//低位在先，依次采集8个bit
	{
//		EA = 0;
		ET0 = 0;
		DS18B20_IO = 0;				//产生2us低电平脉冲
		Delayus(2);
		DS18B20_IO = 1;				//结束低电平脉冲，等待18B20输出数据
		Delayus(2);						//延时2us
	 	if(DS18B20_IO)				//读取通信引脚上的值
		 	dat |= mask;
//		EA = 1;
		ET0 = 1;
		Delayus(60);					//再延时60us
	}
	
	return dat;	
}

bit Start18B20()
{
 	bit ack;
	static bit flag = 1;

	ack = Get18B20Ack();		//执行总线复位，并获取18B20应答
	if(ack == 0)						//如18B20正确应答，则启动一次转换
	{
		DS18B20Write(0xCC);		//跳过ROM操作
		
		if(flag)
		{
			flag = 0;
			DS18B20Write(0x4e);			//写暂存器指令4E
			DS18B20Write(0x63);			//写高速缓存器TH高温限值99度
			DS18B20Write(0x00);			//写高速缓存器TL低温限值0度
			DS18B20Write(0x1f);			//写配置寄存器4
															//0x1f ： 0.5000°C  转换时间93.75ms
															//0x3f ： 0.2000°C  转换时间187.5ms
															//0x5f ： 0.1250°C  转换时间375ms
															//0x7f ： 0.0625°C  转换时间750ms
		}
		
		ack = Get18B20Ack();		//执行总线复位，并获取18B20应答
		if(ack == 0)						//如18B20正确应答，则启动一次转换
		{
			DS18B20Write(0xCC);		//跳过ROM操作
			DS18B20Write(0x44);		//启动一次温度转换
		}
	}

	return ~ack;						//ack == 0 表示操作成功，所以返回值对其取反
}

bit Get18B20Temp(int *temp)
{
 	bit ack;
	unsigned char LSB, MSB;			//16bit温度值的低字节和高字节

	ack = Get18B20Ack();				//执行总线复位，并获取18B20应答
	if(ack == 0)								//如18B20正确应答，则读取温度值
	{
	 	DS18B20Write(0xCC);				//跳过ROM操作
		DS18B20Write(0xBE);				//发送读命令
		LSB = DS18B20Read();			//读温度值的低字节
		MSB = DS18B20Read();			//读温度值的高字节
		*temp = ( MSB << 8) + LSB;	//合成16bit的整数
	}

	return ~ack;								//ack == 0 表示操作应答，所以返回值为1其取反值
} 

