#include <stdio.h>
#include "typh_const_num.h"
#ifdef __CG
static __inline long rpcc()
{
   long a;
   asm volatile ("memb");
 //  asm("rtc %0":"=r"(a));
   asm volatile ("rcsr %0,4":"=r"(a));
   return a;
}
#else
long rpcc()
{
   long a;
   asm volatile("memb");
   asm volatile("rtc %0":"=r"(a));
//   asm("rcsr %0,4":"=r"(a));
      return a;
      }
#endif        
