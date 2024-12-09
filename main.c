
/*
 * Lab_RTC_Time_min_sek.c
 *
 * Created: 26.11.2023 19:37:15
 * Author : Emil Nilsen
 */ 

#include <avr/io.h>
#define F_CPU 4000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "rtc.h"




volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;

ISR(RTC_CNT_vect)
{
	seconds += 1;
	if(seconds >= 60)
	{
		minutes += 1 ; 
		seconds = 0;
	}
	if(minutes >= 60)
	{
		hours += 1 ; 
		minutes = 0;
	}
	RTC.INTFLAGS |= RTC_OVF_bm;
}

int main(void)
{
    usart_usb_init();
	rtc_clock_init();
	rtc_init();
	
	sei();
    while (1) 
    {
		printf("Hours: %d Minutes: %d Seconds: %d\n", hours, minutes, seconds);
		_delay_ms(1000);
    }
}

