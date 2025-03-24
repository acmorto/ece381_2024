const int buzzerPin = 25;
const int resolution = 8;

int frequency[] = {239, 262, 294, 330, 392, 440, 494};
int touchPin[] = {4, 15, 13, 12, 14, 27, 33, 32};

void playFreq(int frequency) {
  ledcWriteTone(buzzerPin, frequency);
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 8; i++) {
      pinMode(touchPin[i], INPUT);
  }
  ledcAttach(buzzerPin, 2000, resolution);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 8; i++) {
    int touchState = touchRead(touchPin[i]);
    if (touchState < 20) {
      playFreq(frequency[i]);
      delay(50);
    }
    else ledcWriteTone(buzzerPin, 0);
  }
}
