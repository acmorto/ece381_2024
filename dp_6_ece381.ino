const int motionPin = 27;
const int buzzerPin = 26;
const int resolution = 8;
long unsigned int chair = 0;

void setup() {
  Serial.begin(112500);
  pinMode(motionPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  int motionState = digitalRead(motionPin);

  if (motionState == HIGH) {
    chair = millis();
    Serial.println("Motion detected! Buzzer alarm!");
    digitalWrite(buzzerPin, HIGH);
  }

  if (millis() -  chair == 4000) {
    digitalWrite(buzzerPin, LOW);
  }
}
