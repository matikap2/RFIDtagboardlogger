#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f4xx.h"
#include "stdint.h"

/* Init function */
void RTClock_Init(void);

typedef struct {
uint8_t year;
uint8_t month;
uint8_t day;
uint8_t dayWeek;
}rtcDate;

typedef struct {
uint8_t hours;
uint8_t minutes;
uint8_t seconds;
}rtcTime;

/* Get/Set functions */
void RTClock_getDate(rtcDate*);
void RTClock_getTime(rtcTime*);
void RTClock_setDate(rtcDate);
void RTClock_setTime(rtcTime);

/* Help functions */
uint8_t bin2bcd(uint8_t);
uint8_t bcd2bin(uint8_t);
void RTClock_dateToSerial(rtcDate*);
void RTClock_timeToSerial(rtcTime*);
void RTClock_fullToSerial(rtcTime*, rtcDate*);
void RTClock_fullToNodeMCU(rtcTime*, rtcDate*);





#endif
