MCU = atmega328p
F_CPU = 16000000UL
BAUD = 115200
PORT = COM3
PROGRAMMER = arduino
BAUDRATE = 115200

CC = avr-gcc
CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I../../drivers/uart -I../utils
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

TARGET = eeprom_dump
SOURCES = ../utils/eeprom_dump.c ../utils/blinky.c ../drivers/uart/uart.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET).hex

$(TARGET).elf: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUDRATE) -U flash:w:$<

clean:
	rm -f *.o *.elf *.hex ../utils/*.o ../drivers/uart/*.o ../test/*.o

# Test target
test: eeprom_test.hex
eeprom_test.hex: eeprom_test.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
eeprom_test.elf: ../test/eeprom_test.c ../utils/blinky.c ../drivers/uart/uart.c
	$(CC) $(CFLAGS) -o $@ $^

flash_test: eeprom_test.hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUDRATE) -U flash:w:$<

.PHONY: all flash clean