#include <stc15.h>
#include "sys.h"

bit flag_2ms = 0, flag_500ms = 0;
bit flag_fan = 0, flag_temper = 0;
u8 mode = 1, modebackup = 1;
signed char min = 2, sec = 0;		//time
int temper_int = 0;				//temperture

void main(void)
{
	int temper = 0;
	bit res;
	
	AllInit();
	Timer0Init();
	Start18B20();
	EA = 1;
	while(1)
	{
		TubeShow();
		KeyPress();
		
		if((sec == 0) && (min == 0))
			flag_fan = 0;
		else
			flag_fan = 1;
		
		if(flag_2ms)
		{
			flag_2ms = 0;
			KeyScan();
		}
		
		if(flag_500ms)		//show temperture
		{
			flag_500ms = 0;
			res = Get18B20Temp(&temper);
			if(res)
			{
				temper >>= 4;
				if((temper > 0) && (temper < 75))
					temper_int = temper;
			}
			Start18B20();
		}
	}
}




