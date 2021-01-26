#include <stc15.h>
#include "sys.h"

bit flag_125ms = 0, flag_200ms = 0, flag_500ms = 0, flag_1s = 0;
u8 mode = 2;
u16 Temper_int = 0, Temper_dec = 0;
u16 distance = 0;

void main(void)
{	
	u16 t;
	
	AllInit();
	init_ds18b20();
	Trans_temper();
//	Timer0Init();
	Timer1Init();
	
	EA = 1;
	
	while(1)
	{
		//温度测量程序
		if(flag_1s)
		{
			flag_1s = 0;
			Temper_int = Gettemper();
			Temper_dec = Temper_int & 0xf;		//分离小数部分
			Temper_dec = Temper_dec * ((10000 / 16) / 100);
			Temper_int = Temper_int >> 4;			//分离整数部分
			
			Trans_temper();								//启动下一次温度转换
		}
		
		//超声波测距
		if(mode == 2)
		{
			if(flag_125ms)
			{
				flag_125ms = 0;
				SendSonic();			//发送50KHz的超声波	
				TR1 = 1;					//开启定时器1计时
				while((RX == 1) && (TF1 == 0));	//如果接收到回波或者定时器1溢出
				TR1 = 0;					//关闭定时器1
				if(TF1 == 1)
				{
					TF1 = 0;
					distance = 999;	//距离999
				}
				else
				{
					t = TH1;
					t <<= 8;
					t |= TL1;
					distance = (u16)(t * 0.017);
				}
				TH1 = 0;
				TL1 = 0;
			}
		}
		
		KeyPress();
	}
}