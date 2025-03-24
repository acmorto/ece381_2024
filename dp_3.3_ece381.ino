int potPin = 4;
int ledPin[] = {5, 18, 19, 21, 22, 23};

void setup() {
  pinMode(potPin, INPUT);
  for (int i = 0; i < 6; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  int vol_num = analogRead(potPin);
  float vol_val = vol_num * (3.3 / 4095.0);
  if (vol_num <= 683) {
    digitalWrite(ledPin[5], HIGH);
    digitalWrite(ledPin[4], LOW);
    digitalWrite(ledPin[3], LOW);
    digitalWrite(ledPin[2], LOW);
    digitalWrite(ledPin[1], LOW);
    digitalWrite(ledPin[0], LOW);
  } else if (vol_num > 683 && vol_num <= 1365) {
    digitalWrite(ledPin[4], HIGH);
    digitalWrite(ledPin[3], LOW);
    digitalWrite(ledPin[2], LOW);
    digitalWrite(ledPin[1], LOW);
    digitalWrite(ledPin[0], LOW);
  } else if (vol_num > 1365 && vol_num <= 2048) {
    digitalWrite(ledPin[3], HIGH);
    digitalWrite(ledPin[2], LOW);
    digitalWrite(ledPin[1], LOW);
    digitalWrite(ledPin[0], LOW);
  } else if (vol_num > 2048 && vol_num <= 2730) {
    digitalWrite(ledPin[2], HIGH);
    digitalWrite(ledPin[1], LOW);
    digitalWrite(ledPin[0], LOW);
  } else if (vol_num > 2730 && vol_num <= 3413) {
    digitalWrite(ledPin[1], HIGH);
    digitalWrite(ledPin[0], LOW);
  }
  else digitalWrite(ledPin[0], HIGH);
  
  Serial.println(vol_val);
  delay(100);
}