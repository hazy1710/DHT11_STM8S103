#include <iostm8s103f3.h>
#include <stm8s_type.h>
#include "delay_us.h"

void delay_ms(u16 ms){
  for (u16 count=0;count<ms;count++){
    delay_us(1000);
  }
}

