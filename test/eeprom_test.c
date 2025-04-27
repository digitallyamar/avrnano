#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../drivers/uart/uart.h"
#include "../utils/blinky.h"

void eeprom_write_byte(uint16_t addr, uint8_t data) {
    while (EECR & (1 << EEPE)); // Wait for previous write
    EEAR = addr; // Set address
    EEDR = data; // Set data
    EECR |= (1 << EEMPE); // Enable write
    EECR |= (1 << EEPE);  // Start write
}

uint8_t eeprom_read_byte(uint16_t addr) {
    while (EECR & (1 << EEPE)); // Wait for previous write
    EEAR = addr; // Set address
    EECR |= (1 << EERE); // Start read
    return EEDR; // Return data
}

int main(void) {
    uint16_t addr;
    // Configure PB5 (LED) as output
    DDRB |= (1 << DDB5); // Set PB5 as output
    PORTB &= ~(1 << PORTB5); // LED off initially

    uart_init(9600); // Initialize UART at 9600 baud
    _delay_ms(2000); // Wait 2s for CH340/bootloader to settle

    // Write test pattern to first 16 bytes of EEPROM
    for (addr = 0; addr < 16; addr++) {
        eeprom_write_byte(addr, addr % 2 ? 0xAD : 0xDE); // Alternating 0x55, 0xAA
    }
    uart_tx('W'); // Indicate write complete

    // Debug: Send initial status
    uart_tx('S'); // Start indicator

    while (1) {
        if (uart_rx() == 'd') { // Wait for 'd' to dump
            // Debug: Check DDRB before blinky
            if (!(DDRB & (1 << DDB5))) {
                uart_tx('P'); // Pin not output
                DDRB |= (1 << DDB5); // Re-set PB5
            }
            // Blink LED to indicate dump start
            int result = blinky(&PORTB, DDB5, 500); // Use DDB5 for consistency
            if (result != 0) {
                uart_tx('E'); // Blink error
                continue;
            }
            for (addr = 0; addr < 1024; addr++) {
                uint8_t byte = eeprom_read_byte(addr);
                uart_tx(byte);
            }
            // Blink LED to indicate dump complete
            blinky(&PORTB, DDB5, 500);
        }
    }
}