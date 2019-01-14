#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f4xx.h"
#include "stdint.h"

/* Init function */
void SysTick_Init(void);
void SysTick_Decrement(void);

/* Delay functions */
void Delay_us(uint32_t);
void Delay_1ms(void);
void Delay_ms(uint32_t);

#endif
