#include "dl_uart.h"

/* USART2 initialization */
void dlUART2_init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN ; //Enable clock on GPIO_A
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //Enable clock on USART2
	
	GPIOA->AFR[0] = 0x00007700;
	GPIOA->MODER = (2<<(2*2)) | (2<<(3*2)) ;
	GPIOA->OSPEEDR = (3<<(2*2)) | (3<<(3*2));
	
	USART2->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
	USART2->CR3 |= USART_CR3_ONEBIT;
	USART2->BRR = (84000000/(2*115200));
}


/* USART1 send byte */
void dlUART2_sendByte(char _data){
while (!(USART2-> SR & USART_SR_TXE)); // Wait until the TXE bit in the SR register becomes 1
USART2-> DR = _data; // Byte in UART2
}

/* USART1 send string */
void dlUART2_sendString(char* string){
	while (*string != 0) {
		dlUART2_sendByte(*string);
		string++;
	}
}
