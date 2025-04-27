#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "blinky.h"

int blinky(volatile uint8_t *port, uint8_t pin, uint16_t delay_ms) 
{
    // Verify that the pin is configured to be OUT
    if (!(DDRB & (1 << pin))) {
        return -1;
    }
    // Set the output pin value to 1 to Turn ON the LED
    *port |= 1 << pin;
    _delay_ms(delay_ms);

    // Clear the output pin value to Turn OFF the LED
    *port &= ~(1 << pin);
    _delay_ms(delay_ms);

    return 0;
}