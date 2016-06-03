#include <stm8s_type.h>

#pragma once

#ifndef F_CPU
#warning DEFINE F_CPU
#define F_CPU 16000000
#endif

#define CYCLES_PER_US (F_CPU/1000000)	/* тактов на мкс */
#define C4PUS (CYCLES_PER_US/4)		/* 4 такта на мкс */
#define DVUS(x) (C4PUS*x)

extern void _delay_us(unsigned short u);

#define delay_us(x) _delay_us(DVUS(x))

void delay_ms(u16 ms);