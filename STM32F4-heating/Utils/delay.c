#include "delay.h"

static __IO uint32_t TimingDelay ;
static __IO uint32_t TimingDelay2 ;


void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0)
  {
	  TimingDelay --;
  }
}


void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

void Delay2(__IO uint32_t nTime)
{
  TimingDelay2 = nTime;

  while(TimingDelay2 != 0)
  {
	  TimingDelay2 --;
  }
}



