#ifndef _LCD_H_
#define _LCD_H_

#include "stm32f4xx.h"
#include "stdint.h"
#include "systick.h"


/* Macros */
#define RS_OFF()	GPIOB->BSRR |= GPIO_BSRR_BR8;
#define RS_ON()		GPIOB->BSRR |= GPIO_BSRR_BS8;
#define E_OFF()		GPIOB->BSRR |= GPIO_BSRR_BR9;
#define E_ON()		GPIOB->BSRR |= GPIO_BSRR_BS9;

/* HD44780 Commands */
#define HD44780_CLEAR					       0x01
#define HD44780_HOME					       0x02
#define HD44780_ENTRY_MODE				   0x04
	#define HD44780_EM_SHIFT_CURSOR		 0x00
	#define HD44780_EM_SHIFT_DISPLAY	 0x01
	#define HD44780_EM_DECREMENT		   0x00
	#define HD44780_EM_INCREMENT		   0x02
#define HD44780_DISPLAY_ONOFF			   0x08
	#define HD44780_DISPLAY_OFF		     0x00
	#define HD44780_DISPLAY_ON			   0x04
	#define HD44780_CURSOR_OFF			   0x00
	#define HD44780_CURSOR_ON			     0x02
	#define HD44780_CURSOR_NOBLINK	   0x00
	#define HD44780_CURSOR_BLINK		   0x01
#define HD44780_DISPLAY_CURSOR_SHIFT 0x10
	#define HD44780_SHIFT_CURSOR		   0x00
	#define HD44780_SHIFT_DISPLAY		   0x08
	#define HD44780_SHIFT_LEFT			   0x00
	#define HD44780_SHIFT_RIGHT		     0x04
#define HD44780_FUNCTION_SET			   0x20
	#define HD44780_FONT5x7				     0x00
	#define HD44780_FONT5x10			     0x04
	#define HD44780_ONE_LINE			     0x00
	#define HD44780_TWO_LINE			     0x08
	#define HD44780_4_BIT				       0x00
	#define HD44780_8_BIT				       0x10
#define HD44780_CGRAM_SET				     0x40
#define HD44780_DDRAM_SET				     0x80

/* Initialization function */
void LCD_Init(void);

/* Help functions */
void LCD_wirteNibble(uint8_t);
void LCD_writeCmd(uint8_t);
void LCD_writeData(uint8_t);

/* Main functions */
void LCD_writeTxt(char*);
void LCD_goTo(uint8_t, uint8_t);
void LCD_puts(uint8_t, uint8_t, char*);
void LCD_clear(void);



#endif
