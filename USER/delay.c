#include "delay.h"

void Delay( uint32_t Val) 
{
  for( ; Val != 0; Val--) 
 		{
		__nop();
 		}  		
}
