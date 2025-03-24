#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Replace with your network credentials
const char* ssid = "HowdyThere";
const char* password = "Password123";

// Set up web server on port 80
WebServer server(80);

// OLED setup
#include <Wire.h>
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_WIDTH 128 // OLED display width, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Define the HTML form for text input
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html",
      "<form action=\"/send\" method=\"POST\">"
      "<input type=\"text\" name=\"message\" placeholder=\"Enter message\">"
      "<input type=\"submit\" value=\"Send\">"
      "</form>");
  });

  // Handle form submission
  server.on("/send", HTTP_POST, []() {
    String message = server.arg("message");  // Get the text input
    Serial.println("Received message: " + message);
    displayMessage(message);
    server.send(200, "text/html", "<p>Message sent!</p><a href=\"/\">Go back</a>");
  });

  // Start server
  server.begin();
  Serial.println("Server started");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}

// Function to display message on OLED
void displayMessage(String message) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(message);
  display.display();
}

