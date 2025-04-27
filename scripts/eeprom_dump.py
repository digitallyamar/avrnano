import serial
import time

# Configure serial port
ser = serial.Serial('COM3', 9600, timeout=5)  # 5s timeout
try:
    print("Opening serial port...")
    time.sleep(5)  # Wait for CH340 and bootloader
    print("Clearing buffer...")
    ser.reset_input_buffer()  # Clear noise
    ser.reset_output_buffer()

    print("Sending 'd'...")
    ser.write(b'd')  # Trigger dump
    ser.flush()  # Ensure 'd' is sent

    print("Reading 1024 bytes...")
    data = b""
    while len(data) < 1024:
        chunk = ser.read(1024 - len(data))  # Read in chunks
        data += chunk
        print(f"Read {len(chunk)} bytes, total {len(data)}/1024")
        time.sleep(0.1)  # Small delay to avoid CPU overload

    # Save to file
    with open('eeprom_dump.bin', 'wb') as f:
        f.write(data)
    print(f"EEPROM dump saved to eeprom_dump.bin ({len(data)} bytes)")

except Exception as e:
    print(f"Error: {e}")
finally:
    ser.close()
