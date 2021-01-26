#include <stc15.h>
#include "sys.h"

u8 temper_low = 20, temper_high = 30, mode = 0;
u8 temper_range[] = {3, 0, 2, 0}, temp_backup[] = {16, 16, 16, 16};
int Realtemper = 0;
bit flag_option = 0;
bit flag_right = 1;
bit flag_200ms = 0;

void main(void)
{
	int tem = 0;
	
	AllInit();
	Start18B20();
	Timer0Init();
	EA = 1;
	while(1)
	{
		if(flag_200ms == 1)
		{
			flag_200ms = 0;
			Get18B20Temp(&tem);
			tem >>= 4;
			if((tem >= 0) && (tem <= 75))
				Realtemper = tem;
			Start18B20();
		}
		
		if(flag_option == 0)
		{
			if(Realtemper < temper_low)
				mode = 0;
			else if((Realtemper >= temper_low) && (Realtemper <= temper_high))
				mode = 1;
			else
				mode = 2;
		}
		
		KeyPress();
		TubeShow();
	}
}