#include "lcd.h"

//VSS - GND
//VDD - +5V
//V0 - potentiometr (left to GND, middle to V0, right to VDD)
//RS - (OUT) PB8
//RW - GND
//E - (OUT) PB9	
//D4-D5-D6-D7 - (OUT) PB10-PB4-PB5-PB3
//A - +3.3V
//K - GND


/* Initialization function */
void LCD_Init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN ; 			//Enable clock on GPIO_B
	
	GPIOB->MODER = ((1<<(8*2)) | (1<<(9*2)) | (1<<(10*2)) | (1<<(4*2)) |
	(1<<(5*2)) | (1<<(3*2))) ;									//Every pin as OUTPUT
	GPIOB->OSPEEDR = ((3<<(8*2)) | (3<<(9*2)) | (3<<(10*2)) | (3<<(4*2)) |
	(3<<(5*2)) | (3<<(3*2))) ;									//High speed
	
	Delay_ms(45);
	
	//Set pin low
	GPIOB->BSRR |= GPIO_BSRR_BR8;
	GPIOB->BSRR |= GPIO_BSRR_BR9;
	GPIOB->BSRR |= GPIO_BSRR_BR10;
  GPIOB->BSRR |= GPIO_BSRR_BR4;
	GPIOB->BSRR |= GPIO_BSRR_BR5;
	GPIOB->BSRR |= GPIO_BSRR_BR3;
	 
									 
	for(int i = 0; i<3 ; i++){									//Lets try to initialize x3
	E_ON();
	LCD_wirteNibble(0x03);
	Delay_10us(1);
	E_OFF();
	Delay_ms(5);
	}
	
	E_ON();
	LCD_wirteNibble(0x02);											//Start in 4bit mode
	Delay_10us(1);
	E_OFF()
	Delay_ms(1);
	
	LCD_writeCmd(HD44780_FUNCTION_SET | 				//Two line mode, 4bit, fotn size
                   HD44780_FONT5x7 | 
                   HD44780_TWO_LINE | 
                   HD44780_4_BIT);
	
	LCD_writeCmd(HD44780_DISPLAY_ONOFF | 				//Turn off
                   HD44780_DISPLAY_OFF);
  
  LCD_writeCmd(HD44780_CLEAR);
	Delay_ms(2);
  
  LCD_writeCmd(HD44780_ENTRY_MODE | 					
                   HD44780_EM_SHIFT_CURSOR | 
                   HD44780_EM_INCREMENT);
	
	LCD_writeCmd(HD44780_DISPLAY_ONOFF | 
								HD44780_DISPLAY_ON | 
								HD44780_CURSOR_OFF | 
								HD44780_CURSOR_NOBLINK);
  

	Delay_ms(45);
}



/* Help functions */
void LCD_wirteNibble(uint8_t nibble){					//Splitting nibble to each data pin
	
	if (nibble & 0x01)
		GPIOB->BSRR |= GPIO_BSRR_BS10;
	else
		GPIOB->BSRR |= GPIO_BSRR_BR10;
	
	if (nibble & 0x02)
		GPIOB->BSRR |= GPIO_BSRR_BS4;
	else
		GPIOB->BSRR |= GPIO_BSRR_BR4;
	
	if (nibble & 0x04)
		GPIOB->BSRR |= GPIO_BSRR_BS5;
	else
		GPIOB->BSRR |= GPIO_BSRR_BR5;
	
	if (nibble & 0x08)
		GPIOB->BSRR |= GPIO_BSRR_BS3;
	else
		GPIOB->BSRR |= GPIO_BSRR_BR3;
}


void LCD_writeCmd(uint8_t cmd){								//Write command to lcd
	RS_OFF();
	Delay_10us(1);
	E_ON();
	LCD_wirteNibble(cmd >> 4);
	Delay_10us(1);
	E_OFF();
	Delay_10us(1);
	E_ON();
	LCD_wirteNibble(cmd & 0x0F);
	Delay_10us(1);
	E_OFF();
	Delay_10us(5);
}


void LCD_writeData(uint8_t data){							//Write data to lcd
	RS_ON();
	E_ON();
	LCD_wirteNibble(data >> 4);
	Delay_10us(1);
	E_OFF();
	Delay_10us(1);
	E_ON();
	LCD_wirteNibble(data & 0x0F);
	Delay_10us(1);
	E_OFF();
	Delay_10us(5);
}



/* Main functions */
void LCD_writeTxt(char* text){
	while(*text)
		LCD_writeData(*text++);
}


void LCD_goTo(uint8_t x, uint8_t y){
	LCD_writeCmd(HD44780_DDRAM_SET | (x + (0x40 * y)));
}


void LCD_puts(uint8_t x, uint8_t y, char* text){
	LCD_goTo(x,y);
	LCD_writeTxt(text);
}


void LCD_clear(void){
	LCD_writeCmd(HD44780_CLEAR);
	Delay_ms(3);
}

