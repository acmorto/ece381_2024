from micropython import const
import asyncio
import aioble
import bluetooth
import struct
from machine import Pin, PWM
from random import randint


# Init LED
buzzer = PWM(Pin(25))
buzzer.duty(0)

# Init random value
value = 0


_BLE_SERVICE_UUID = bluetooth.UUID('e88dde54-755a-4af0-9ab4-37f9ff74d3f6')
_BLE_SENSOR_CHAR_UUID = bluetooth.UUID('e88dde54-755a-4af0-9ab4-37f9ff74d3f6')
_BLE_LED_UUID = bluetooth.UUID('e88dde54-755a-4af0-9ab4-37f9ff74d3f6')
# How frequently to send advertising beacons.
_ADV_INTERVAL_MS = 250_000

# Register GATT server, the service and characteristics
ble_service = aioble.Service(_BLE_SERVICE_UUID)
sensor_characteristic = aioble.Characteristic(ble_service, _BLE_SENSOR_CHAR_UUID,
read=True, notify=True)
led_characteristic = aioble.Characteristic(ble_service, _BLE_LED_UUID, read=True,
write=True, notify=True, capture=True)

# Register service(s)
aioble.register_services(ble_service)

def tone(pin, frequency, duration):
    pin.freq(frequency) # Set the frequency
    pin.duty(512) # Set the duty cycle
    time.sleep_ms(duration) # Pause for the duration in milliseconds
    pin.duty(0) # Set the duty cycle to 0 to stop the tone

# Helper to encode the data characteristic UTF-8
def _encode_data(data):
    return str(data).encode('utf-8')

# Helper to decode the LED characteristic encoding (bytes).
def _decode_data(data):
    try:
        if data is not None:
            # Decode the UTF-8 data
            number = int.from_bytes(data, 'big')
            return number
    except Exception as e:
        print("Error decoding temperature:", e)
        return None

# Get sensor readings
def get_random_value():
    return randint(0,100)

# Get new value and update characteristic
async def sensor_task():
    while True:
        value = get_random_value()
        sensor_characteristic.write(_encode_data(value), send_update=True)
        print('New random value written: ', value)
        await asyncio.sleep_ms(1000)
        
# Serially wait for connections. Don't advertise while a central is connected.
async def peripheral_task():
    while True:
        try:
            async with await aioble.advertise(
                _ADV_INTERVAL_MS,
                name="ESP32",
                services=[_BLE_SERVICE_UUID],
                ) as connection:
                    print("Connection from", connection.device)
                    await connection.disconnected()
        except asyncio.CancelledError:
            # Catch the CancelledError
            print("Peripheral task cancelled")
        except Exception as e:
            print("Error in peripheral_task:", e)
        finally:
            # Ensure the loop continues to the next iteration
            await asyncio.sleep_ms(100)
            
async def wait_for_write():
    while True:
        try:
            connection, data = await led_characteristic.written()
            print(data)
            print(type)
            data = _decode_data(data)
            print('Connection: ', connection)
            print('Data: ', data)
            if data == 1:
                print('Playing tone 1')
                tone(buzzer, 262, 5000)
            elif data == 2:
                print('Playing tone 2')
                tone(buzzer, 330, 2500)
            elif data == 3:
                print('PLaying tone 3')
                tone(buzzer, 392, 2500)
            else:
                print('Unknown command')
                buzzer.duty(0)
        except asyncio.CancelledError:
            # Catch the CancelledError
            print("Peripheral task cancelled")
        except Exception as e:
            print("Error in peripheral_task:", e)
        finally:
            # Ensure the loop continues to the next iteration
            await asyncio.sleep_ms(100)
            
# Run tasks
async def main():
    t1 = asyncio.create_task(sensor_task())
    t2 = asyncio.create_task(peripheral_task())
    t3 = asyncio.create_task(wait_for_write())
    await asyncio.gather(t1, t2)
    
asyncio.run(main())
