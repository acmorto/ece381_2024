#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"
#include <stdio.h>
#include <stdlib.h>

const int buzzerPin = 14;
const int resolution = 8;

int note[] = {262, 330, 294, 349, 330, 392, 349};

const int led0 = 25;
const int led1 = 26;
const int led2 = 27;

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
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

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
    int intValue = std::stoi(value);
    if (intValue < 8 && intValue > 0) {
      for (int i = 0; i < intValue; i++) {
        ledLit(i + 1);
        delay(100);
        playFreq(note[i]);
        delay(100);
      }
      delay(900);
      digitalWrite(led0, LOW);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
    }
    else {
      for (int i = 1; i < 8; i++) {
        errorLights(i);
        delay(100);
      }
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

void ledLit(int n) {
  if (n == 1) { // 001
    digitalWrite(led0, HIGH);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
  }
  if (n == 2) { // 010
    digitalWrite(led0, LOW);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }
  if (n == 3) { // 011
    digitalWrite(led0, HIGH);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }
  if (n == 4) { // 100
    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }
  if (n == 5) { // 101
    digitalWrite(led0, HIGH);
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }
  if (n == 6) { // 110
    digitalWrite(led0, LOW);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  }
  if (n == 7) {
    digitalWrite(led0, HIGH);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  }
};

void playFreq(int note) {
  ledcWriteTone(buzzerPin, note);
  delay(500);                
  ledcWriteTone(buzzerPin, 0);
};

void errorLights(int run) {
  if (run == 1) {
    digitalWrite(led0, HIGH);
  }
  if (run == 2) {
    digitalWrite(led0, LOW);
    digitalWrite(led1, HIGH);
  }
  if (run == 3) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }
  if (run == 4) {
    digitalWrite(led2, LOW);
  }
  if (run == 5) {
    digitalWrite(led2, HIGH);
  }
  if (run == 6) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }
  if (run == 6) {
    digitalWrite(led0, HIGH);
    digitalWrite(led1, LOW);
  }
  if (run == 7) {
    digitalWrite(led0, LOW);
  }
};
