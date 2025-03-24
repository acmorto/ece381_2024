from machine import TouchPad, Pin, PWM
import time

# Define touch pins and their associated notes (in Hz)
TOUCH_PINS = [4, 15, 13, 12, 14, 27, 33, 32]  # List of touch-capable GPIO pins
NOTES = [262, 294, 330, 349, 392, 440, 494, 523]  # C, D, E, F, G, A, B, C

# Setup touch sensors and buzzer
touch_sensors = [TouchPad(Pin(pin)) for pin in TOUCH_PINS]
buzzer = PWM(Pin(25))  # Connect the buzzer to GPIO25

# Threshold value to detect touch
TOUCH_THRESHOLD = 400  # Adjust based on sensitivity and setup

while True:
    note_playing = False  # Flag to indicate if a note is playing
    for i, touch in enumerate(touch_sensors):
        if touch.read() < TOUCH_THRESHOLD:  # Touch detected
            buzzer.freq(NOTES[i])
            buzzer.duty(512)  # Start playing the note
            note_playing = True
            break  # Only play one note at a time

    # If no note is playing, stop the buzzer
    if not note_playing:
        buzzer.duty(0)  # Stop the buzzer

    time.sleep(0.05)  # Small delay to avoid rapid polling

   
