#define F_CPU 16000000UL  // 16 MHz clock
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

void uart_init(uint32_t baud)
{
    uint16_t ubrr = F_CPU / 16 / baud - 1; // Table 20.1 Datasheet, ATMega328P
    UBRR0H = (uint8_t)ubrr >> 8;
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable Rx and Tx
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data, no parity, 1 stop bit
}

void uart_tx(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0))); // Wait for Tx Buffer to be empty
    UDR0 = data; // Send data
}

uint8_t uart_rx(void)
{
    while(!(UCSR0A & (1 << RXC0))); // Wait for data to fill Rx buffer
    return UDR0;
}