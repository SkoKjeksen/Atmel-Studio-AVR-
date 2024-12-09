
/*
* usart.h
*
* Created: 24.08.2022 14:14:25
* Author: pol022
*/
#ifndef USART_H_
#define USART_H_
#include <avr/io.h>
#define TX_PIN_bp 0
#define RX_PIN_bp 1
#include <stdio.h>
char usb_message[256];
volatile uint8_t marker_pos;
volatile uint8_t usb_message_ready;
//char usb_message_transmitt[10];
//volatile uint8_t marker_transmitt_pos;
//volatile uint8_t usb_message_length;
volatile uint8_t u2_marker_pos;
volatile uint8_t u2_message_ready;
char u2_message[256];
void usart_usb_init();
void usart_usb_transmit(char c);
void usart_usb_transmit_interrupt(char c);
char usart_usb_receive();
/* printing an array of chars*/
void usart_usb_transmit_char_array(uint8_t addEndLine,char string[], uint8_t
length);
/* preparing for printf */
static uint8_t usart_usb_transmit_printf(char c, FILE *stream);
static FILE new_std_out = FDEV_SETUP_STREAM(usart_usb_transmit_printf, NULL,
_FDEV_SETUP_WRITE);
/* generic usart files */
void usart_init(USART_t *usart_nr, PORT_t *usart_port, uint8_t tx_bp, uint8_t
rx_bp);
void usart_transmit_char(USART_t *usart_nr,char c);
typedef void (*usart_char_transmitter)(USART_t *usart_nr,char c);
void usart_transmit_string(USART_t *usart_nr, char *string, usart_char_transmitter
transmit_char);
#endif /* USART_H_ */
