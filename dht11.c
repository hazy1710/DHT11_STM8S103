#include "dht11.h"
#include <iostm8s103f3.h>


#define NUM_OF_BYTES 5

#define START_DELAY  18

#define ONE_HIGH     150
#define ONE_LOW      130

//Таймаут ожидания события DHT11
static unsigned long int dht11_timeout;

//Задать таймаут в микросекундах
#define set_tmo_us(time)  dht11_timeout = (unsigned long int)(F_MASTER_MHZ * time)

//Задать таймаут в миллисекундах
#define set_tmo_ms(time)  dht11_timeout = (unsigned long int)(F_MASTER_MHZ * time * 1000)

#define tmo               dht11_timeout--

//Ожидание наступления события event
//в течении времени timeout в мс
#define wait_event(event, timeout) set_tmo_ms(timeout);\
                                   while(event && --dht11_timeout);\
                                   if(!dht11_timeout) return 1;
/**
 * @brief Initialize port and pin to which DHT is plugged as output open-drain
*/

/**
 * @brief Get temperature and humidity of DHT11
 * @param t - pointer to temperature var
 * @param h - pointer to humidity var
 * @returns RESULT
 * @note Period between functon calls should be more then 2 seconds
 * @note While reading interrupts are disable
*/
RESULT DHT11_GetData(u8 * t, u8 * h)
{
  u8 tmp[5];
  if (DHT_ReadData(tmp) == 1)
    return RESULT_ERRREAD;
  if (tmp[4] == DHT_CheckSum(tmp))
  {
    *h = tmp[0];
    *t = tmp[2];
  }
  else
    return RESULT_ERRCHKSUM;
  return RESULT_OK;
}

/**
 * @brief Reads 5 bytes and put them to data
 * @param data - pointer to output array
 * @returns 0 - if result OK, 1 - if error reading
 * @note Period between functon calls should be more then 2 seconds
 * @note While reading interrupts are disable
*/
u8 DHT_ReadData(u8* data)
{
  u8 byte_num = 0, bit_num = 0, byte = 0, count = 0;
  __disable_interrupt();
  /* sending start condition */
  DHT_PIN_OUT = 0;                          // write "0"
  delay_ms(START_DELAY);                    // wait 1 ms
  DHT_PIN_OUT = 1;                          // write "1": make pin as input
  /* begin reading */
  delay_us(1);
  wait_event(DHT_PIN_IN, 1);                // wait sensor response 20-40us
  wait_event(!(DHT_PIN_IN), 1);                    // pin low 80ms: response
  wait_event(DHT_PIN_IN, 1);                      // pin high 80ms: prepare
  for (byte_num = 0; byte_num < NUM_OF_BYTES; byte_num++)
  {
    for (bit_num = 0; bit_num < 8; bit_num++)
    {
      wait_event(DHT_PIN_IN, 1);         // pin low 50us
      wait_event(!DHT_PIN_IN, 1);        // pin high
      delay_us(40);
      byte |= (DHT_PIN_IN << (7 - bit_num));
    }
    *(data + byte_num) = byte;
    byte = 0;
  }
  __enable_interrupt();
  return 0;
}

/**
 * @brief Calculate checksum
 * @param data - pointer to input array
 * @returns checksum
*/
u8 DHT_CheckSum(u8* data)
{
  unsigned int sum = 0;
  for (int i = 0; i < 4; i++)
    sum += *(data + i);
  return (u8)sum;
}
