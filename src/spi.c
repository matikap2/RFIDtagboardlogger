#include "spi.h"

// PA4  - (OUT)	SPI1_NSS/SDA
// PA5  - (AF-OUT)	SPI1_SCK
// PA6  - (AF-IN)	SPI1_MISO (Master In)
// PA7  - (AF-OUT)	SPI1_MOSI (Master Out)

#define NSS_ON()		GPIOA->BSRR = GPIO_BSRR_BR4;	//NSS=0 -> transmit
#define NSS_OFF()		GPIOA->BSRR = GPIO_BSRR_BS4;	//NSS=1

/* Init function */
void SPI1_Init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 					//Enable clock on GPIO_A
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 					//Enable clock on SPI
	
	GPIOA->AFR[0] |= 0x55500000;																					//Alternate function for SCK/MISO/MOSI
	GPIOA->MODER |= (1<<(4*2));																						//OUTPUT for NSS
	GPIOA->MODER |= (2<<(5*2)) | (2<<(6*2)) | (2<<(7*2));									//AF for SCK/MISO/MOSI
	GPIOA->OSPEEDR |= (3<<(4*2)) | (3<<(5*2)) | (3<<(6*2)) | (3<<(7*2));  //High speed
	
	SPI1->CR1 |= SPI_CR1_MSTR;		//SPI in Master config
	SPI1->CR1 |= SPI_CR1_BR;			//Baudrade divisor /256
	SPI1->CR1 |= SPI_CR1_SSM;			//Software Slave Management	
	SPI1->CR1 |= SPI_CR1_SSI;			//Internal Slave Select
	
	SPI1->CR1 |= SPI_CR1_SPE;			//ENABLE SPI
}

/* Send/Read functions */
uint8_t SPI1_writeByte(uint8_t data){
	while(!(SPI1->SR & SPI_SR_TXE));  //Wait until previous transfer complete
	SPI1->DR = data; 									//Send data to SPI1
	while(!(SPI1->SR & SPI_SR_RXNE)); //Wait until end of transfer
	return SPI1->DR; 									//Read recived data
}


void SPI1_writeReg(uint8_t adr, uint8_t val){
	NSS_ON();
	SPI1_writeByte(adr);
	SPI1_writeByte(val);
	NSS_OFF();	
}


uint8_t SPI1_readReg(uint8_t adr){
	uint8_t val;
	
	NSS_ON();
	SPI1_writeByte(adr);
	val = SPI1_writeByte(0x00);
	NSS_OFF();
	
	return val;
}

