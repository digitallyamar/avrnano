# Simple Projects With Arduino Nano CH430 and AVR GCC library

## Blinky
LED is connected to PB5 pin of the microcontroller

### Compile and build binary image

```
    avr-gcc -mmcu=atmega328p -Os -o blinky.elf blinky.c
    avr-objcopy -O ihex blinky.elf blinky.hex
```

### Flash the image to Arduino Nano Clone CH430

```
    avrdude -c arduino -p atmega328p -P COM3 -b 115200 -U flash:w:blinky.hex
```