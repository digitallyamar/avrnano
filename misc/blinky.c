#include <avr/io.h>
#include <stdio.h>
// AVR Libc requires you to define F_CPU (e.g., 16 MHz for the Nano) 
// before including <util/delay.h>
#define F_CPU 16000000UL // 16 MHz
#include <util/delay.h>

int blinky(volatile uint8_t *port, uint8_t pin, uint16_t delay_ms) 
{
    // Verify that the pin is configured to be OUT
    if (!(DDRB & (1 << pin))) {
        return -1;
    }
    // Set the output pin value to 1 to Turn ON the LED
    *port |= 1 << pin;
    _delay_ms(delay_ms);

    // Clear the output pin value to 1 to Turn ON the LED
    *port &= ~(1 << pin);
    _delay_ms(delay_ms);

    return 0;
}

int main()
{
    //LED is connected to PB5 og Arduino Nano
    // So configure Port B Bit 5 register


    // First set the direction of this pin to be OUT
    // 1: Out, 0: IN
    DDRB = (1 << DDB5);

    // Reset all output values
    PORTB = 0;

    while (1) {
        if (blinky(&PORTB, PORTB5, 1000)) {
            break;
        }
    }
}