int potPin = 4;

void setup() {
  pinMode(potPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int vol_num = analogRead(potPin);
  Serial.print(vol_num);
  Serial.print("\n");
  delay(500);
}
