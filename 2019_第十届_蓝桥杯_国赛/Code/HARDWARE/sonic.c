#include "sys.h"

#define sonic_nop {_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();};

void SendSonic(void)
{
	unsigned char i = 8;
	
	while(i --)
	{
		TX = 1;
		sonic_nop; sonic_nop; sonic_nop; sonic_nop; sonic_nop;
		sonic_nop; sonic_nop; sonic_nop; sonic_nop; sonic_nop;
		TX = 0;
		sonic_nop; sonic_nop; sonic_nop; sonic_nop; sonic_nop;
		sonic_nop; sonic_nop; sonic_nop; sonic_nop; sonic_nop;
	}
}



