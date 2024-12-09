
/*
* usart.c
*
* Created: 24.08.2022 14:14:12
* Author: pol022
*/
#include <avr/io.h>
#include "usart.h"
#include <avr/interrupt.h>
volatile uint8_t usb_message_length = 10;
volatile uint8_t marker_transmit_pos;
char usb_message_transmit[] = "hei og h ";
volatile char transmit_buffer[100];
volatile uint8_t next = 0;
volatile uint8_t last = 0;
void usart_usb_init(){
	/* Necessary configuration*/
	USART3.BAUD = 1667; // Baudrate 9600 // F_CPU = 4M
	//USART3.BAUD = 6667; //Baudrate 9600 // FCPU = 16MHz
	USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm | USART_RXMODE_NORMAL_gc;
	//enable RX TX
	PORTB.DIR |= (1<<TX_PIN_bp);
	
	//PORTB.PINCTRLSET |= (1<<RX_PIN_bp);

	/* interrupt on RX*/
	USART3.CTRLA |= USART_RXCIE_bm;
	/* interrupt on TX */
	//USART3.CTRLA |= USART_TXCIE_bm;
	/* printf */
	stdout = &new_std_out; // address to struct
}
void usart_usb_transmit(char c){
	while(!(USART3.STATUS & USART_DREIF_bm)){
		;
	}
	USART3.TXDATAL = c;
}
void usart_usb_transmit_interrupt(char c){
	if (next == last){
		next = 0;
		last = 0;
	}
	if(USART3.STATUS & USART_DREIF_bm)
	USART3.TXDATAL = c;
	else{
		transmit_buffer[last] = c;
		last++;
	}
}
/*ISR(USART3_TXC_vect){
	if(next < last){
		USART3.TXDATAL = transmit_buffer[next];
		next++;
		USART3.STATUS |= USART_TXCIF_bm;
	}
	else// no problems
	USART3.STATUS |= USART_TXCIF_bm;
}*/
/* transmitting*/
void usart_usb_transmit_char_array(uint8_t addEndLine,char string[], uint8_t length){
	for(uint8_t i = 0; i<length; i++){
		usart_usb_transmit(string[i]);
	}
	if(addEndLine)
	usart_usb_transmit('\n');
}
/* rx-polling*/
char usart_usb_receive(){
	while(!(USART3.STATUS & USART_RXCIF_bm))// not unread data in receiver
	;
	return USART3.RXDATAL;
}
/* rx-interrupt with some handling. see main*/
 ISR(USART3_RXC_vect){
	char tmp = USART3.RXDATAL;
	
	switch (tmp)
	{
		case '<':
		marker_pos = 0;
		break;
		case '>':
		usb_message_ready = 1;
		usb_message[marker_pos] = 0;
		break;
		default:
		usb_message[marker_pos] = tmp;
		marker_pos++;
	}
} 
static uint8_t usart_usb_transmit_printf(char c, FILE *stream){
	usart_usb_transmit(c);
	return 0;
}
/* Generic usart-files */
void usart_init(USART_t *usart_nr, PORT_t *usart_port, uint8_t tx_bp, uint8_t
rx_bp){
	/* Necessary configuration*/
	PORTMUX.USARTROUTEA |= PORTMUX_USART2_ALT1_gc;
	usart_nr->BAUD = 1667; // Baudrate 9600
	usart_nr->CTRLB |= USART_RXEN_bm | USART_TXEN_bm | USART_RXMODE_NORMAL_gc; //enable RX TX
	usart_port->DIR |= (1<<tx_bp);
	usart_port->PINCONFIG |= PORT_PULLUPEN_bm;
	usart_port->PINCTRLSET |= (1<<rx_bp);
	// RX-handling in separate function.
	//usart_port->CTRLA |= USART_RXCIE_bm; // interrupt on RX
}
void usart_transmit_char(USART_t *usart_nr,char c){
	while(!(usart_nr->STATUS & USART_DREIF_bm)){
		;
	}
	usart_nr->TXDATAL = c;
}
void usart_transmit_string(USART_t *usart_nr, char *string, usart_char_transmitter
transmit_char){
	for(uint8_t i = 0; string[i] != '\0'; i++)
	transmit_char(usart_nr, string[i]);
}
// example with USART2
/*ISR(USART2_RXC_vect){
	char tmp = USART2.RXDATAL;
	switch (tmp)
	{
		case '{':
			u2_marker_pos = 0;
			break;
		case '}':
		u2_message_ready = 1;
		u2_message[marker_pos] = 0;
		break;
		default:
		u2_message[marker_pos] = tmp;
		u2_marker_pos++;
	}
}*/
