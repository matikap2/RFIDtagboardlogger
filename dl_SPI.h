#ifndef __DL_SPI_H
#define __DL_SPI_H

#include "stm32f4xx.h"
#include "stdint.h"

//INITIALIZATION AND FUNCTIONS
void dlSPI_init(void);
uint8_t	dlSPISendByte (uint8_t);
void dlSPI1_WriteReg(uint8_t, uint8_t);
uint8_t dlSPI1_ReadReg(uint8_t);

#endif
