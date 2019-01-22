#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"
#include "stdint.h"
#include "rc522.h"

extern uint8_t statusUART1;

/* Init function */
void UART2_Init(void);

void UART1_Init(void);

/* Send functions */
void UART2_sendByte(char);
void UART2_sendString(char*);

void UART1_sendByte(char);
void UART1_sendString(char*);

/* Receive function */
void USART1_IRQHandler(void);


#endif
