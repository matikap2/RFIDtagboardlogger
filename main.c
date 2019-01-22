#include "stm32f4xx.h"
#include "uart.h"
#include "systick.h"
#include "spi.h"
#include "rc522.h"
#include "rtc.h"


int main(void){

	/* Clock update */
		SystemCoreClockUpdate();
	
	/* Initialization section */
		//Variables
		uint8_t CardID[4];
		uint8_t status;			
		rtcDate data;
		rtcTime czas;
	
		//SysTick
		SysTick_Init();	
	
		//UART
		UART2_Init();
	  UART1_Init();

		//RTC
		UART2_sendString("Przed init!\r\n");
		Delay_ms(1000);	
		RTClock_Init();
		UART2_sendString("Po init!\r\n");
		Delay_ms(1000);	
		
		//SPI
		SPI1_Init();

		//RC522
		RC522_Init();
		
		
		
	while(1){
		/* OBSLUGA PRZESYLU DANYCH DO FIREBASE */
		/**/
			switch (statusUART1){	//STATE MACHINE
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
			case 51:							//Restart state machine
				RTClock_getTime(&czas);
				RTClock_getDate(&data);
				RTClock_fullToNodeMCU(&czas, &data);
				statusUART1 = 0;
				break;
			default:
				break;
		}



	/* PROGRAM TESTOWY DO SPRAWDZENIA CZYTNIKA */
	/*
	status = RC522_checkID(CardID);
	//RC522_firmwareVer();
	Delay_ms(1000);
	if (status == 0)
		RC522_dumpUIDtoSerial(CardID);
	else
		UART2_sendString("Brak karty!\r\n");
	Delay_ms(1000);	
	*/

	/* PROGRAM TESTOWY DO SPRAWDZENIA RTC */
	/*
		RTClock_getTime(&czas);
		RTClock_getDate(&data);
		RTClock_fullToSerial(&czas, &data);
		UART2_sendString("\r\n");
		Delay_ms(5000);
	*/	

	}
}

