#include <iostm8s103f3.h>
#include <stm8s_type.h>
#include "delay_us.h"
#include <intrinsics.h>

#define DHT11
#define F_MASTER_MHZ    16UL

#define DHT_PIN_IN       PC_IDR_bit.IDR4
#define DHT_PIN_OUT      PC_ODR_bit.ODR4
#define DHT_PIN_CR1      PC_CR1_bit.C14
#define DHT_PIN_DDR      PC_DDR_bit.DDR4

typedef enum
{
  RESULT_OK =           0,      // OK
  RESULT_ERRREAD =      1,      // Error reading
  RESULT_ERRCHKSUM =    2,      // Error check sum
} RESULT;

RESULT DHT11_GetData(u8 * t, u8 * h);
unsigned char DHT_ReadData(u8 * data);
unsigned char DHT_CheckSum(u8 * data);
