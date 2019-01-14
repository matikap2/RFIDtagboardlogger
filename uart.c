#include "uart.h"

// PA2 - (OUT) TX UART1
// PA3 - (IN) RX UART1

#define baudrate 115200

/* Init function */
void UART2_Init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN ; //Enable clock on GPIO_A
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //Enable clock on USART2
	
	GPIOA->AFR[0] |= 0x00007700;								//AF for PA2 as TX and PA3 as RX
	GPIOA->MODER |= (2<<(2*2)) | (2<<(3*2)) ;		//Setting pins as AF
	GPIOA->OSPEEDR |= (3<<(2*2)) | (3<<(3*2));	//Setting pins speed as HIGH
	
	USART2->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);	//Enable UART, Transmitter and Reciver
	USART2->CR3 |= USART_CR3_ONEBIT;						//One sample bit method
	USART2->BRR = (SystemCoreClock/baudrate);		//Setting baudrate
}

/* Send functions */
void UART2_sendByte(char data){
	while (!(USART2-> SR & USART_SR_TXE));	//Wait until transmit data register is empty
	USART2-> DR = data;											//Write data to data register
}


void UART2_sendString(char* string){
	while (*string != 0) {				//Until string is empty
		UART2_sendByte(*string);		//Send each character in string
		string++;
	}
}
