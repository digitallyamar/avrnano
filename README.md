# avrnano

A bare-metal AVR development framework for the Arduino Nano (ATmega328P), featuring a modular EEPROM dump utility, UART driver, LED blinker, and test code. This project provides reusable utilities for embedded development, with a focus on clean code and extensibility for tasks like SRAM and Flash dumping.

## Table of Contents
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Setup](#setup)
- [Utilities](#utilities)
  - [EEPROM Dump (`utils/eeprom_dump.c`)](#eeprom-dump-utilseeprom_dumpc)
  - [EEPROM Test (`test/eeprom_test.c`)](#eeprom-test-testeeprom_testc)
  - [UART Driver (`drivers/uart/uart.c`)](#uart-driver-driversuartuartc)
  - [Blinky Utility (`utils/blinky.c`)](#blinky-utility-utilsblinkyc)
- [Building and Flashing](#building-and-flashing)
- [Capturing EEPROM Dump](#capturing-eeprom-dump)
- [Contributing](#contributing)
- [License](#license)

## Project Structure
avrnano/
├── .vscode/
│   └── settings.json       # VSCode settings (Git Bash terminal)
├── drivers/
│   └── uart/
│       ├── uart.c         # UART driver (9600 baud)
│       └── uart.h
├── utils/
│   ├── blinky.c           # LED blinker utility
│   ├── blinky.h
│   └── eeprom_dump.c      # EEPROM dump utility
├── test/
│   └── eeprom_test.c      # EEPROM write and dump test
├── scripts/
│   └── eeprom_dump.py     # Python script to capture EEPROM dump
├── misc/
│   ├── testnano.c         # Initial test code
│   └── Makefile           # Build and flash for eeprom_dump and eeprom_test
├── .gitignore             # Ignore build artifacts (*.o, *.elf, *.hex, *.bin)
└── README.md


## Prerequisites
- **Hardware**: Arduino Nano (ATmega328P) with CH340 USB-to-serial chip.
- **Software**:
  - AVR-GCC toolchain (e.g., [Zak Kemble’s AVR-GCC](https://github.com/ZakKemble/avr-gcc/releases)).
  - `avrdude` for flashing.
  - Git Bash (Windows) for building and flashing.
  - Python 3 with `pyserial` (`pip install pyserial`) for capturing dumps.
  - PuTTY or similar for serial monitoring.
- **OS**: Tested on Windows 10 (Git Bash in VSCode).

## Setup
1. **Clone the Repository**:
   ``
       git clone https://github.com/digitallyamar/avrnano.git
       cd avrnano
   ``
2. Install AVR-GCC:
Download avr-gcc-13.2.0-x64-windows.zip from Zak Kemble’s AVR-GCC.

Extract to C:\avr-gcc and add to PATH:
``
    export PATH=$PATH:/c/avr-gcc/bin
``
3. Verify Tools:
``
    avr-gcc --version
    avrdude --version
``
4. Install Python Dependencies:
``
    pip install pyserial
``
5. Configure VSCode:
    * Open avrnano in VSCode.
    * Ensure .vscode/settings.json sets Git Bash as the default terminal:
    ``
        {
            "terminal.integrated.defaultProfile.windows": "Git Bash"
        }
    ``

# Utilities
## EEPROM Dump (utils/eeprom_dump.c)

* ## Purpose: Dumps the ATmega328P’s 1024-byte EEPROM over UART (9600 baud) when triggered by ‘d’ in a serial terminal.

* ## Features:
Sends ‘S’ on startup to indicate readiness.

Blinks the onboard LED (PB5, Arduino D13) for 500ms before and after the dump.

Outputs 1024 bytes of EEPROM data (~0.9s at 9600 baud).

Usage:
Flash eeprom_dump.hex (see Building and Flashing (#building-and-flashing)).

Open PuTTY (COM3, 9600, 8N1).

Reset the Nano; expect ‘S’ after ~2s.

Type ‘d’ to trigger the dump (binary data appears as ▒).

Use scripts/eeprom_dump.py to capture the dump (see Capturing EEPROM Dump (#capturing-eeprom-dump)).

EEPROM Test (test/eeprom_test.c)
Purpose: Writes a test pattern (alternating 0x55, 0xAA) to the first 16 bytes of EEPROM and supports dumping to verify.

Features:
Writes pattern at startup, sends ‘W’ (write complete) and ‘S’ (ready).

Same dump functionality as eeprom_dump.c (triggered by ‘d’).

Usage:
Flash eeprom_test.hex (see Building and Flashing (#building-and-flashing)).

Open PuTTY; expect ‘W’ then ‘S’.

Type ‘d’ to dump; first 16 bytes are 55 AA ..., rest typically 0xFF (unprogrammed).

Modify eeprom_test.c to write different patterns (e.g., 0x00 or full 0xFF erase).

UART Driver (drivers/uart/uart.c)
Purpose: Initializes and handles UART communication at 9600 baud for the ATmega328P (PD0/RXD, PD1/TXD).

Functions:
uart_init(9600): Sets up UART with 9600 baud, 8 data bits, no parity, 1 stop bit.

uart_tx(byte): Sends a byte over UART.

uart_rx(): Receives a byte (blocks until data is available).

Usage:
Included by eeprom_dump.c and eeprom_test.c for serial communication.

Requires F_CPU = 16000000UL for accurate baud rate.

Blinky Utility (utils/blinky.c)
Purpose: Blinks an LED on a specified port and pin with a given delay.

Function:
blinky(&PORTB, DDB5, 500): Blinks PB5 (Arduino D13) for 500ms on, 500ms off.

Returns -1 if the pin isn’t an output, 0 on success.

Usage:
Used by eeprom_dump.c and eeprom_test.c to indicate dump start/end.

Configurable for other ports/pins (e.g., PORTD, DDB0).

Building and Flashing
Navigate to misc/:
bash

cd misc

Build eeprom_dump (Default):
bash

make clean && make

Generates eeprom_dump.hex in misc/.

Flash eeprom_dump:
bash

make flash

Uploads to Nano via COM3 (adjust PORT in Makefile if needed).

Build eeprom_test:
bash

make clean && make test

Generates eeprom_test.hex.

Flash eeprom_test:
bash

make flash_test

Clean Build Artifacts:
bash

make clean

Removes *.o, *.elf, *.hex from misc/, utils/, test/, drivers/uart/.

Capturing EEPROM Dump
Run Python Script:
bash

cd scripts
python eeprom_dump.py

Opens COM3 at 9600 baud, sends ‘d’, and saves 1024 bytes to eeprom_dump.bin.

Output: EEPROM dump saved to eeprom_dump.bin (1024 bytes).

View Dump:
Open scripts/eeprom_dump.bin in a hex editor (e.g., HxD).

Unprogrammed EEPROM shows 0xFF; after running eeprom_test, first 16 bytes are 55 AA ....

Manual Capture with PuTTY:
Open PuTTY (COM3, 9600, 8N1).

Enable logging (Session > Logging > All session output > eeprom_log.bin).

Type ‘d’ to capture the dump.

Contributing
Fork the repo and submit pull requests for enhancements (e.g., SRAM/Flash dumping, 115200 baud).

Report issues on the GitHub Issues page.






   
