from machine import Pin
import time


led1 = Pin(13, Pin.OUT)
led2 = Pin(14, Pin.OUT)
led3 = Pin(27, Pin.OUT)
led4 = Pin(26, Pin.OUT)
led5 = Pin(25, Pin.OUT)
led6 = Pin(33, Pin.OUT)
led7 = Pin(32, Pin.OUT)

button1 = Pin(34, Pin.IN, Pin.PULL_DOWN)
button2 = Pin(23, Pin.IN, Pin.PULL_DOWN)
button3 = Pin(18, Pin.IN, Pin.PULL_DOWN)

while True:
    #001
    if button1.value() == 1:
        led1.value(1)
        led2.value(0)
        led3.value(0)
        led4.value(0)
        led5.value(0)
        led6.value(0)
        led7.value(0)
    #010
    if button2.value() == 1:
        led1.value(1)
        led2.value(1)
        led3.value(0)
        led4.value(0)
        led5.value(0)
        led6.value(0)
        led7.value(0)
    #011
    if button1.value() == 1 & button2.value() == 1:
        led1.value(1)
        led2.value(1)
        led3.value(1)
        led4.value(0)
        led5.value(0)
        led6.value(0)
        led7.value(0)
    #100
    if button3.value() == 1:
        led1.value(1)
        led2.value(1)
        led3.value(1)
        led4.value(1)
        led5.value(0)
        led6.value(0)
        led7.value(0)
    #101
    if button3.value() == 1 & button1.value() == 1:
        led1.value(1)
        led2.value(1)
        led3.value(1)
        led4.value(1)
        led5.value(1)
        led6.value(0)
        led7.value(0)
    #110
    if button3.value() == 1 & button2.value() == 1:
        led1.value(1)
        led2.value(1)
        led3.value(1)
        led4.value(1)
        led5.value(1)
        led6.value(1)
        led7.value(0)
    #111
    if button1.value() == 1 & button2.value() == 1 & button3.value() == 1:
        led1.value(1)
        led2.value(1)
        led3.value(1)
        led4.value(1)
        led5.value(1)
        led6.value(1)
        led7.value(1)
    #000
    if button1.value() == 0 & button2.value() == 0 & button3.value() == 0:
        led1.value(0)
        led2.value(0)
        led3.value(0)
        led4.value(0)
        led5.value(0)
        led6.value(0)
        led7.value(0)
   

