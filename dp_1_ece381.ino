const int ledPin = 5;
const int buttonPin = 4;

void setup() {
  // led pin is set to output mode
  pinMode(ledPin, OUTPUT);

  // button pin is set to input pull down since resister is between the button and ground
  pinMode(buttonPin, INPUT_PULLDOWN);
}

void loop() {
  // put your main code here, to run repeatedly:
  // variable to capture the current state of the button
  int buttonState = digitalRead(buttonPin);

  // if button is pressed, turn on
  if (buttonState == HIGH) {
    
    // write the led to be HIGH (on)
    digitalWrite(ledPin, HIGH);
  } 
  else {
    // write the led to be LOW (off)
    digitalWrite(ledPin, LOW);
  }
}
