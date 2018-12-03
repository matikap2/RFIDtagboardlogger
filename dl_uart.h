#ifndef __DL_UART_H
#define __DL_UART_H

#include "stm32f4xx.h"
#include "stdint.h"

//UART2 debug
void dlUART2_init(void);
void dlUART2_sendByte(char);
void dlUART2_sendString(char*);

#endif
