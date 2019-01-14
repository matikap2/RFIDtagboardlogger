#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"
#include "stdint.h"

/* Init function */
void UART2_Init(void);

/* Send functions */
void UART2_sendByte(char);
void UART2_sendString(char*);




#endif
