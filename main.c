#include "stm32f4xx.h"
#include "uart.h"
#include "systick.h"
#include "spi.h"
#include "rc522.h"
#include "rtc.h"
#include "lcd.h"



int main(void){

	/* Clock update */
		SystemCoreClockUpdate();
	
	/* Initialization section */
		//Variables
		uint8_t CardID[4];
		uint8_t status;			
		rtcDate data;
		rtcTime czas;
		uint8_t temp;
		char buffer[100];
	
		//SysTick
		SysTick_Init();	
	
		//LCD
		LCD_Init();
	
		//UART
		UART2_Init();
	  UART1_Init();

		//RTC
		RTClock_Init();
		
		//SPI
		SPI1_Init();

		//RC522
		RC522_Init();
		
		/* PROGRAM TESTOWY DO WYSWIETLENIA WERSJI RC522 NA LCD */
		LCD_clear();
		LCD_goTo(0,0);
		LCD_writeTxt("Wersja RC522:");
		LCD_goTo(0,1);
		temp = RC522_readReg(VersionReg);						//Read version of chip -> 0x12 probably counterfit chip (info google)
		sprintf(buffer, "0x%x", temp);
		LCD_writeTxt(buffer);
		Delay_ms(3000);
		
		
	while(1){
		/* OBSLUGA PRZESYLU DANYCH DO FIREBASE */
		/*
			switch (statusUART1){													//STATE MACHINE
			case 49:																			//Read legitka UID
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
			case 50:																			//Read plytka UID
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
			case 51:																			//Get time and date form RTC
				RTClock_getTime(&czas);
				RTClock_getDate(&data);
				RTClock_fullToNodeMCU(&czas, &data);
				statusUART1 = 0;
				break;
			default:																			//Restart state machine
				break;
		}
	*/


	/* PROGRAM TESTOWY DO SPRAWDZENIA CZYTNIKA */
	/**/	
	status = RC522_checkID(CardID);
	//RC522_firmwareVer();
	Delay_ms(1000);
	if (status == 0){
		//RC522_dumpUIDtoSerial(CardID);
		sprintf(buffer, "%x%x%x%x", CardID[0],CardID[1],CardID[2],CardID[3]);
		LCD_clear();	
		LCD_goTo(0,0);
		LCD_writeTxt(buffer);
		Delay_ms(2000);
	}
	else{
		//UART2_sendString("Brak karty!\r\n");
		LCD_clear();	
			LCD_goTo(0,0);
		LCD_writeTxt("Brak karty!");
		Delay_ms(2000);	
	}
	

	/* PROGRAM TESTOWY DO SPRAWDZENIA RTC */
	/**/	
		RTClock_getTime(&czas);
		RTClock_getDate(&data);
		//RTClock_fullToSerial(&czas, &data);
		//UART2_sendString("\r\n");
		LCD_clear();	
		LCD_goTo(0,0);
		sprintf(buffer, "%u/%u/%u",  data.day, data.month, data.year);
		LCD_writeTxt(buffer);
		LCD_goTo(0,1);
		sprintf(buffer, "%u:%u:%u",  czas.hours, czas.minutes, czas.seconds);
		LCD_writeTxt(buffer);
		Delay_ms(2000);
	

	}
}

