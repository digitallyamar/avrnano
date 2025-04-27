#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(uint32_t baud);
void uart_tx(uint8_t data);
uint8_t uart_rx(void);

#endif  // UART_H