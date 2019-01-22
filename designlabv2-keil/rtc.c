#include "rtc.h"
#include "systick.h"
#include "uart.h"

/* Init function */
void RTClock_Init(void){
	uint32_t predivA = 127;
	//uint32_t predivS = 255;
	uint32_t predivS = 249;
	rtcDate rtcDate;
	rtcTime	rtcTime;
	
	rtcDate.day=1;
	rtcDate.month=1;
	rtcDate.year=1;
	rtcDate.dayWeek=1;
	
	rtcTime.hours=0;
	rtcTime.minutes=0;
	rtcTime.seconds=0;
	
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;	//PWR clock enable
	PWR->CR |= PWR_CR_DBP;							//Disable backup domain write protection
	//RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;	//PWR clock disable
	
	RCC->BDCR |= RCC_BDCR_BDRST;				//Enable RTC (reset backup domain)
	Delay_ms(20);												//Wait for reset
	RCC->BDCR = 0;											//Deactivate RTC domain reset
	
	
	if(!(RCC->BDCR & RCC_BDCR_LSERDY)){						//Check if LSE is stopped
		RCC->BDCR |= RCC_BDCR_LSEON;										//Enable LSE
		while(!(RCC->BDCR & RCC_BDCR_LSERDY));			//Wait until LSE is working
	}

	//RCC->CSR |= RCC_CSR_LSION;											//Enable LSI
	//while(!(RCC->CSR & RCC_CSR_LSIRDY));						//Wait until LSI is working
	
	RCC->BDCR &= ~RCC_BDCR_RTCSEL;										//Clear clock source register
	RCC->BDCR |= RCC_BDCR_RTCSEL_0;										//Set LSE as clock source
	//RCC->BDCR |= RCC_BDCR_RTCSEL_1;										//Set LSI as clock source
	RCC->BDCR |= RCC_BDCR_RTCEN;											//Enable RTC
	
	//RCC->APB1ENR |= RCC_APB1ENR_PWREN;	//PWR clock enable
	//PWR->CR |= PWR_CR_DBP;							//Disable backup domain write protection
	//RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;	//PWR clock disable
	
	RTC->WPR = 0xCA;										//Disable RTC registers write protection
	RTC->WPR = 0x53;
	
	RTC->ISR |= RTC_ISR_INIT;										//Enable RTC Init mode
	while(!(RTC->ISR & RTC_ISR_INITF));		//Wait until register can be updated
	
	RTC->PRER = predivA<<16 | predivS;					//Set prescalers
	RTC->TR = ((rtcTime.hours<<16) | 					//Set time
						(rtcTime.minutes<<8) | 
						(rtcTime.seconds));
	RTC->DR = ((rtcDate.year<<16) | 					//Set date
						(rtcDate.dayWeek<<13) | 
						(rtcDate.month<<8) |
						(rtcDate.day));
	//RTC->TR=0;
	//RTC->DR=0;
	RTC->CR &= ~RTC_CR_FMT;											//24h format
	RTC->ISR &= ~RTC_ISR_INIT;									//Disable RTC Init mode
	RTC->ISR &= ~RTC_ISR_RSF;										//Synchronization
	while(!(RTC->ISR & RTC_ISR_RSF));						//Wait until done synchronization
	RTC->WPR = 0xFF;														//Enable write protection
}

/* Get/Set functions */
void RTClock_getDate(rtcDate* RTCdt){
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t dayWeek;
	uint32_t temp;
	
	RTC->CR |= RTC_CR_BYPSHAD;
	temp = RTC->DR;																		//get date from register
	
	year = (temp & (RTC_DR_YT | RTC_DR_YU)) >> 16;
	dayWeek = (temp & RTC_DR_WDU) >> 13;
	month = (temp & (RTC_DR_MT | RTC_DR_MU)) >> 8;
	day = (temp & (RTC_DR_DT | RTC_DR_DU));
	
	RTCdt->year = bcd2bin(year);
	RTCdt->dayWeek = bcd2bin(dayWeek);
	RTCdt->month = bcd2bin(month);
	RTCdt->day = bcd2bin(day);
	
}

void RTClock_getTime(rtcTime* RTCdt){
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint32_t temp;
	
	RTC->CR |= RTC_CR_BYPSHAD;
	temp = RTC->TR;																	//get time from register
	
	hours = (temp & (RTC_TR_HT | RTC_TR_HU)) >> 16;
	minutes = (temp & (RTC_TR_MNT | RTC_TR_MNU)) >> 8;
	seconds = (temp & (RTC_TR_ST | RTC_TR_SU));
	
	RTCdt->hours = bcd2bin(hours);
	RTCdt->minutes = bcd2bin(minutes);
	RTCdt->seconds = bcd2bin(seconds);
}


void RTClock_setDate(rtcDate RTCdt){
	uint8_t year = bin2bcd(RTCdt.year);
	uint8_t month = bin2bcd(RTCdt.month);
	uint8_t day = bin2bcd(RTCdt.day);
	uint8_t dayWeek = bin2bcd(RTCdt.dayWeek);
	
	RTC->WPR = 0xCA;										//Disable RTC registers write protection
	RTC->WPR = 0x53;
	
	RTC->ISR |= RTC_ISR_INIT;										//Enable RTC Init mode
	while(!(RTC->ISR & RTC_ISR_INITF));		//Wait until register can be updated
	
	RTC->DR = ((year<<16) | 					//Set date
						(dayWeek<<13) | 
						(month<<8) |
						day);

	RTC->ISR &= ~RTC_ISR_INIT;									//Disable RTC Init mode
	RTC->ISR &= ~RTC_ISR_RSF;										//Synchronization
	while(!(RTC->ISR & RTC_ISR_RSF));						//Wait until done synchronization
	RTC->WPR = 0xFF;														//Enable write protection
}

void RTClock_setTime(rtcTime RTCdt){
	uint8_t hours = bin2bcd(RTCdt.hours);
	uint8_t minutes = bin2bcd(RTCdt.minutes);
	uint8_t seconds = bin2bcd(RTCdt.seconds);
	
	RTC->WPR = 0xCA;										//Disable RTC registers write protection
	RTC->WPR = 0x53;
	
	RTC->ISR |= RTC_ISR_INIT;										//Enable RTC Init mode
	while(!(RTC->ISR & RTC_ISR_INITF));		//Wait until register can be updated
	
	RTC->TR = ((hours<<16) | 					//Set time
						(minutes<<8) | 
						seconds);

	RTC->ISR &= ~RTC_ISR_INIT;									//Disable RTC Init mode
	RTC->ISR &= ~RTC_ISR_RSF;										//Synchronization
	while(!(RTC->ISR & RTC_ISR_RSF));						//Wait until done synchronization
	RTC->WPR = 0xFF;														//Enable write protection
}

/* Help functions */
uint8_t bin2bcd(uint8_t bin){
	return ((bin / 10)<<4) | (bin % 10);
}

uint8_t bcd2bin(uint8_t bcd){
	return ((bcd & 0x0f)+10*((bcd>>4) & 0x0f));
}

void RTClock_dateToSerial(rtcDate* RTCdump){
	char buffer[256];
	sprintf(buffer, "%u,%u,%u,%u", RTCdump->year, RTCdump->dayWeek, RTCdump->month, RTCdump->day);
	UART2_sendString(buffer);
}

void RTClock_timeToSerial(rtcTime* RTCdump){
	char buffer[256];
	sprintf(buffer, "%u:%u:%u", RTCdump->hours, RTCdump->minutes, RTCdump->seconds);
	UART2_sendString(buffer);
}

void RTClock_fullToSerial(rtcTime* time, rtcDate* date){
	char buffer[256];
	sprintf(buffer, "%u/%u/%u @ %u:%u:%u",  date->day, date->month, date->year, time->hours, time->minutes, time->seconds);
	UART2_sendString(buffer);
}

void RTClock_fullToNodeMCU(rtcTime* time, rtcDate* date){
	char buffer[256];
	sprintf(buffer, "%u/%u/%u @ %u:%u:%u",  date->day, date->month, date->year, time->hours, time->minutes, time->seconds);
	UART2_sendString(buffer);
	UART1_sendString(buffer);
}