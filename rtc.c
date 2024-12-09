/*
 * rtc.c
 *
 * Created: 18.09.2023 11:37:43
 *  Author: Emil_
 */ 
#include <avr/io.h>
#include "rtc.h"
#include <avr/cpufunc.h>


void rtc_clock_init()
{
	while(CLKCTRL.MCLKSTATUS & CLKCTRL_XOSC32KS_bm)
	{
		;
	}
	ccp_write_io((void *) &CLKCTRL.XOSC32KCTRLA, 1<<CLKCTRL_ENABLE_bp | 0<<CLKCTRL_SEL_bp); //starts x32k clock
	RTC.CLKSEL = RTC_CLKSEL_XOSC32K_gc;
}

void rtc_init()
{
	RTC.CTRLA = RTC_RTCEN_bm;
	while(RTC.STATUS > 0)
	{
		;
	}
	RTC.CTRLA |= RTC_PRESCALER_DIV1_gc;
	RTC.CNT = 0;
	RTC.PER = 32678;
	RTC.INTCTRL = RTC_OVF_bm;	
}


