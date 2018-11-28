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
		//NSS
	GPIOA->MODER |= GPIO_MODER_MODER4_0; //Output mode
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4 ; //Push-pull mode
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4;	//High speed
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR4; //No pull-up/down
	//GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL4_0 | GPIO_AFRL_AFSEL4_2); //Alternative pin function -> SPI1_NSS
	
		//SCK
	GPIOA->MODER |= GPIO_MODER_MODER5_0; //Output mode
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5 ; //Push-pull mode
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;	//High speed
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5; //No pull-up/down
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL5_2); //Alternative pin function -> SPI1_SCK
	
		//MISO
	GPIOA->MODER &= ~GPIO_MODER_MODER6; //Input mode
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6 ; //Push-pull mode
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6_1; //Pull up (01)
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_0; //
	GPIOA->BSRR = GPIO_BSRR_BS6; //Sets pin
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL6_0 | GPIO_AFRL_AFSEL6_2); //Alternative pin function -> SPI1_MISO
		//MOSI
	GPIOA->MODER |= GPIO_MODER_MODER7_0;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_7 ; //Push-pull mode
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7;	//High speed
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR7; //No pull-up/down
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL7_0 | GPIO_AFRL_AFSEL7_2); //Alternative pin function -> SPI1_SCK
	
	/* SPI config */
	SPI1->CR2=0x0000; //Interrupts enable, Motorola frame format, no DMA
	SPI1->CR1 |= SPI_CR1_MSTR; //Master device
	SPI1->CR1 |= SPI_CR1_BR; //Baud rate prescaler set on 256
	SPI1->CR1 |= SPI_CR1_SSM;	//Softawre slave management 
	SPI1->CR1 |= SPI_CR1_SSI;	//Internal slave select
	SPI1->CR1 |= SPI_CR1_SPE;	//ENABLE SPI
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