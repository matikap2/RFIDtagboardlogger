#include "uart.h"

/* Debug UART2 */
// PA2 - (OUT) TX UART2
// PA3 - (IN) RX UART2
/* NodeMCU UART1 */
// PA9 - (OUT) TX UART1
// PA10 - (IN) RX UART1

#define baudrate_debug 115200
#define baudrate_nodemcu 9600

uint8_t statusUART1;
//uint8_t CardID[4];
//uint8_t status;

/* Init function */
void UART2_Init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN ; //Enable clock on GPIO_A
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //Enable clock on USART2
	
	GPIOA->AFR[0] |= 0x00007700;								//AF for PA2 as TX and PA3 as RX
	GPIOA->MODER |= (2<<(2*2)) | (2<<(3*2)) ;		//Setting pins as AF
	GPIOA->OSPEEDR |= (3<<(2*2)) | (3<<(3*2));	//Setting pins speed as HIGH
	
	USART2->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);	//Enable UART, Transmitter and Reciver
	USART2->CR3 |= USART_CR3_ONEBIT;						//One sample bit method
	USART2->BRR = (SystemCoreClock/baudrate_debug);		//Setting baudrate
}

void UART1_Init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN ; //Enable clock on GPIO_A
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //Enable clock on USART1
	
	GPIOA->AFR[1] |= 0x00000770;								//AF for PA9 as TX and PA10 as RX
	GPIOA->MODER |= (2<<(9*2)) | (2<<(10*2)) ;		//Setting pins as AF
	GPIOA->OSPEEDR |= (3<<(9*2)) | (3<<(10*2));	//Setting pins speed as HIGH
	
	USART1->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);	//Enable UART, Transmitter and Reciver
	USART1->CR1 |= USART_CR1_RXNEIE;						//Receive interrupt enable
	USART1->CR3 |= USART_CR3_ONEBIT;						//One sample bit method
	USART1->BRR = (SystemCoreClock/baudrate_nodemcu);		//Setting baudrate
	
	NVIC_EnableIRQ(USART1_IRQn);			//Enable interrupt form USART1
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


void UART1_sendByte(char data){
	while (!(USART1-> SR & USART_SR_TXE));	//Wait until transmit data register is empty
	USART1-> DR = data;											//Write data to data register
}


void UART1_sendString(char* string){
	while (*string != 0) {				//Until string is empty
		UART1_sendByte(*string);		//Send each character in string
		string++;
	}
}

/* Receive function */
void USART1_IRQHandler(void){
	char buffer[50];
	if(USART1->SR & USART_SR_RXNE)
	statusUART1 = USART1->DR;
	UART2_sendString("Halo odbior");
	sprintf(buffer, "%d", statusUART1);
	UART2_sendString(buffer);
	
		/*switch (statusUART1){	//STATE MACHINE
			case 49:							//Read legitka
				for (int i = 0; i<4; i++){
					CardID[i]=0;
				}
				status = RC522_checkID(CardID);
				while(status != OK){
				status = RC522_checkID(CardID);
				}
				if (status == OK){
					RC522_dumpUIDtoNodeMCU(CardID);
					RC522_dumpUIDtoSerial(CardID);
				}
				statusUART1 = 0;
				break;
			case 50:							//Read plytka
				for (int i = 0; i<4; i++){
					CardID[i]=0;
				}
				status = RC522_checkID(CardID);
				while(status != OK){
				status = RC522_checkID(CardID);
				}
				if (status == OK){
					RC522_dumpUIDtoNodeMCU(CardID);
					RC522_dumpUIDtoSerial(CardID);
				}
				statusUART1 = 0;
				break;
			//case 51:							//Restart state machine
			//	statusUART1 = 0;
			//	break;
			default:
				break;
		}*/
}

