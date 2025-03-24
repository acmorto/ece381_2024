int const potPin = 4;
int const ledPin = 21;

void setup() {
  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int potRead = analogRead(potPin);

  int brightness = map(potRead, 0, 4095, 0, 255);

  analogWrite(ledPin, brightness);

  delay(25);
}
