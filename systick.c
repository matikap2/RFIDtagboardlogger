#include "systick.h"
#include "uart.h"

static volatile uint32_t sysTickCounter;

/* Init function */
void SysTick_Init(void){
	while (SysTick_Config(SystemCoreClock / 100000) != 0) {} //every 10us
}


void SysTick_Decrement(void){
	if (sysTickCounter != 0x00) {
		sysTickCounter--;
	}
}


void SysTick_Handler(void){
	SysTick_Decrement();
}



/* Delay functions */
void Delay_10us(uint32_t us){
	sysTickCounter = us;
	while (sysTickCounter != 0){}
}


void Delay_1ms(void){
	sysTickCounter = 100;
	while (sysTickCounter != 0){}
}


void Delay_ms(uint32_t ms){
	while(ms){
		Delay_1ms();
		ms--;
	}
}
