#include "dl_SPI.h"

// PA4  - (OUT)	SPI1_NSS
// PA5  - (OUT)	SPI1_SCK
// PA6  - (IN)	SPI1_MISO (Master In)
// PA7  - (OUT)	SPI1_MOSI (Master Out)


/* SPI INIT AND CONFIG */
void dlSPI_init(void){
	/* Clock on peripheral enable */
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //Enable clock on SPI
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //Enable clock on GPIO_A
	
	/* GPIO config */
	GPIOA->MODER = (0<<(4*2)) | (2<<(5*2)) | (2<<(6*2)) | (2<<(7*2)); //OUT&AF mode
	GPIOA->OSPEEDR = (3<<(4*2)) | (3<<(5*2)) | (3<<(6*2)) | (3<<(7*2)); //High speed
	GPIOA->PUPDR = (1<<(6*2)); //Pull-up for MISO
	GPIOA->AFR[0] |= 0x55500000; //AF SPI for SCK/MISO/MOSI
	
	/* SPI config */
	SPI1->CR2=0x0000; //Interrupts enable, Motorola frame format, no DMA
	SPI1->CR1 |= (SPI_CR1_MSTR | SPI_CR1_BR | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE); //Master device
																																											//Baud rate prescaler set on 256
																																											//Softawre slave management 
																																											//Internal slave select
	SPI1->CR1 |= SPI_CR1_SPE;		//ENABLE SPI
}

/* SPI Send Byte */
uint8_t	dlSPISendByte (uint8_t data){
while(((SPI1)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 
			|| ((SPI1)->SR & SPI_SR_BSY)){}; //Wait until previous transfer complete
SPI1->DR = data; //Send data to SPI1
while(((SPI1)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 
			|| ((SPI1)->SR & SPI_SR_BSY)){}; //Wait until end of transfer
return SPI1->DR; //Read recived data
}

/* SPI Write to Register */
void dlSPI1_WriteReg(uint8_t adress, uint8_t value){
	GPIOA->BSRR = GPIO_BSRR_BR4; //Set NSS off
	dlSPISendByte(adress);
	dlSPISendByte(value);
	GPIOA->BSRR = GPIO_BSRR_BS4; //Set NSS off	
}


/* SPI Read from Register */
uint8_t dlSPI1_ReadReg(uint8_t adress){
	uint8_t temp;
	
	GPIOA->BSRR = GPIO_BSRR_BR4; //Set NSS off
	dlSPISendByte(adress);
	temp = dlSPISendByte(0x00);
	GPIOA->BSRR = GPIO_BSRR_BS4; //Set NSS off
	return temp;
}
