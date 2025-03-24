#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"

const int buzzerPin = 14;
const int resolution = 8;

// Define the Bluetooth device name
const char *bleName = "ESP32_Bluetooth";

// Define the received text and the time of the last message
String receivedText = "";
unsigned long lastMessageTime = 0;

// Define the UUIDs of the service and characteristics
#define SERVICE_UUID "74e3dd4c-7c3d-11ef-b864-0242ac120002"
#define CHARACTERISTIC_UUID_RX "74e3dd4c-7c3d-11ef-b864-0242ac120002"
#define CHARACTERISTIC_UUID_TX "74e3dd4c-7c3d-11ef-b864-0242ac120002"

BLECharacteristic *pCharacteristic;

void setup() {
  Serial.begin(115200);
  setupBLE();
  
  ledcAttach(buzzerPin, 2000, resolution);
}

void loop() {
  // When the received text is not empty and the time since the last message is over 1 second
  // Send a notification and print the received text
  if (receivedText.length() > 0 && millis() - lastMessageTime > 1000) {
    Serial.print("Received message: ");
    Serial.println(receivedText);
    pCharacteristic->setValue(receivedText.c_str());
    pCharacteristic->notify();
    receivedText = "";
  }

  // Read data from the serial port and send it to BLE characteristic
  if (Serial.available() > 0) {
    String str = Serial.readStringUntil('\n');
    const char *newValue = str.c_str();
    pCharacteristic->setValue(newValue);
    pCharacteristic->notify();
  }
}

// Define the BLE server callbacks
class MyServerCallbacks : public BLEServerCallbacks {
  // Print the connection message when a client is connected
  void onConnect(BLEServer *pServer) {
    Serial.println("Connected");
  }

  // Print the disconnection message when a client is disconnected
  void onDisconnect(BLEServer *pServer) {
    Serial.println("Disconnected");
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = std::string(pCharacteristic->getValue().c_str());
    if (value == "off") {
      ledcWriteTone(buzzerPin, 0);
      Serial.println("Buzzer has turned off");
    } else if (value == "theme") {
      Serial.println("The hero has arrived to save the princess...");
      themeSong(buzzerPin);
    } else if (value == "level_up") {
      Serial.println("The hero gets stronger...");
      levelUp(buzzerPin);
    } else if (value == "level_done") {
      Serial.println("Level complete!");
      levelDone(buzzerPin);
    } else if (value == "game_over") {
      Serial.println("GAME OVER");
      gameOver(buzzerPin);
    }
  } 
};

// Initialize the Bluetooth BLE
void setupBLE() {
  BLEDevice::init(bleName); // Initialize the BLE device

  BLEServer *pServer = BLEDevice::createServer(); // Create the BLE server
  // Print the error message if the BLE server creation fails
  if (pServer == nullptr) {
    Serial.println("Error creating BLE server");
    return;
  }
  pServer->setCallbacks(new MyServerCallbacks()); // Set the BLE server callbacks

  // Create the BLE service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // Print the error message if the BLE service creation fails
  if (pService == nullptr) {
    Serial.println("Error creating BLE service");
    return;
  }

  // Create the BLE characteristic for sending notifications
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic->addDescriptor(new BLE2902()); // Add the descriptor
  // Create the BLE characteristic for receiving data
  BLECharacteristic *pCharacteristicRX = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  pCharacteristicRX->setCallbacks(new MyCharacteristicCallbacks()); // Set the BLE characteristic callbacks
  pService->start(); // Start the BLE service
  pServer->getAdvertising()->start(); // Start advertising
  Serial.println("Waiting for a client connection..."); // Wait for a client connection
}

// Theme Song melody
void themeSong(int buzzerPin) {
  // Define the frequencies (in Hz) for the melody notes
  int melody[] = {
    660, 660, 0, 660, 0, 520, 660, 0, 784, 0, 392, 
    0, 520, 0, 440, 0, 392, 0, 349, 523, 440, 
    660, 784, 0, 660, 0, 523, 587, 494
  };

  // Define the durations for each note (in ms)
  int noteDurations[] = {
    100, 100, 100, 100, 100, 100, 100, 100, 300, 250, 250,
    250, 150, 200, 150, 200, 200, 220, 250, 250, 150, 
    250, 150, 150, 150, 100, 100, 100, 100
  };

  // Iterate over the notes of the melody
  for (int i = 0; i < 29; i++) {
    int noteDuration = noteDurations[i];
    if (melody[i] == 0) {
      // If the note is zero, it's a rest, so delay without tone
      delay(noteDuration);
    } else {
      // Play the note with ledcWriteTone
      ledcWriteTone(buzzerPin, melody[i]);
      delay(noteDuration);
    }
    // Stop the tone between notes
    ledcWriteTone(buzzerPin, 0);
    delay(50); // Short delay between notes
  }
}

// Level Up melody
void levelUp(int buzzerPin) {
  // Define the frequencies (in Hz) for the level up sound
  int melody[] = {
    880, 1046, 1318, 1760, 2093, 2637
  };

  // Define the durations for each note (in ms)
  int noteDurations[] = {
    100, 100, 100, 100, 100, 200
  };

  // Iterate over the notes of the melody
  for (int i = 0; i < 6; i++) {
    int noteDuration = noteDurations[i];
    ledcWriteTone(buzzerPin, melody[i]);  // Play the note
    delay(noteDuration);                  // Hold the note for the specified duration
    ledcWriteTone(buzzerPin, 0);          // Stop the tone between notes
    delay(50);                            // Short delay between notes
  }
}

// Level Done melody
void levelDone(int buzzerPin) {
  // Define the frequencies (in Hz) for the level complete sound
  int melody[] = {
    261, 330, 392, 523, 659, 784, 523, 
    261, 311, 415, 523, 622, 831, 523, 
    294, 349, 466, 587, 698, 932, 988, 988, 988, 1047
  };

  // Define the durations for each note (in ms)
  int noteDurations[] = {
    100, 100, 100, 100, 100, 400, 275, 
    100, 100, 100, 100, 100, 400, 275,  
    100, 100, 100, 100, 100, 400, 125, 100, 100, 400
  };

  // Iterate over the notes of the melody
  for (int i = 0; i < 24; i++) {
    int noteDuration = noteDurations[i];
    ledcWriteTone(buzzerPin, melody[i]);  // Play the note
    delay(noteDuration);                  // Hold the note for the specified duration
    ledcWriteTone(buzzerPin, 0);          // Stop the tone between notes
    delay(50);                            // Short delay between notes
  }
}

// Game Over Melody
void gameOver(int buzzerPin) {
  // Define the frequencies (in Hz) for the game over sound
  int melody[] = {
    494, 698, 0, 698, 698, 659, 587, 523
  };

  // Define the durations for each note (in ms)
  int noteDurations[] = {
    100, 100, 100, 100, 100, 100, 100, 350
  };

  // Iterate over the notes of the melody
  for (int i = 0; i < 8; i++) {
    int noteDuration = noteDurations[i];
    ledcWriteTone(buzzerPin, melody[i]);  // Play the note
    delay(noteDuration);                  // Hold the note for the specified duration
    ledcWriteTone(buzzerPin, 0);          // Stop the tone between notes
    delay(80);                           // Short delay between notes
  }
}