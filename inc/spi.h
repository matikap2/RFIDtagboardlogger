#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f4xx.h"
#include "stdint.h"

/* Init function */
void SPI1_Init(void);

/* Send/Read functions */
uint8_t SPI1_writeByte(uint8_t);
void SPI1_writeReg(uint8_t,uint8_t);
uint8_t SPI1_readReg(uint8_t);



#endif
