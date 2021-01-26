#include <stc15.h>
#include "sys.h"

bit flag_1s = 0;
bit flag_error = 0;
int Realtemper = 0;
u8 Lowtemper = 20, Hightemper = 30;
u8 temp_mode = 0;
u8 mode = 0;
u8 temp_option[] = {16, 16, 16, 16};

void main(void)
{
	int temp;			//存储温度的临时变量
	
	AllInit();
	Start18B20();
	Timer0Init();
	EA = 1;
	while(1)
	{
		if(flag_1s == 1)		//1s读取一次温度
		{
			flag_1s = 0;
			Get18B20Temp(&temp);
			temp >>= 4;			//温度整数部分
			if((temp >= 0) && (temp <= 60))		//显示温度在0-60℃
				Realtemper = temp;
			Start18B20();
		}
		
		//判断温度区间
		if(Realtemper > Hightemper)
			temp_mode = 2;
		else if(Realtemper < Lowtemper)
			temp_mode = 0;
		else
			temp_mode = 1;			
			
		KeyPress();
		TubeShow();
	}
}