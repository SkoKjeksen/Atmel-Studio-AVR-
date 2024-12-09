/*
 * xosc32k_init.c
 *
 * Created: 18.09.2023 12:26:01
 *  Author: Emil_
 */ 
#include <avr/io.h>
#include "xosc32k_init.h"
#include <avr/cpufunc.h>


void xosc32k_init()
{
	ccp_write_io((void *) & (CLKCTRL.MCLKCTRLA), CLKSEL_OSC32K_gc);
	CLKCTRL.XOSC32KCTRLA |= CLKCTRL_RUNSTBY_bm | CLKCTRL_ENABLE_bm;
	
	
}