#ifndef BLINKY_H
#define BLINKY_H

#include <stdint.h>

int blinky(volatile uint8_t *port, uint8_t pin, uint16_t delay_ms);

#endif // BLINKY_H