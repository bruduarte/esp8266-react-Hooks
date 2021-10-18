#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncEventSource.h>
#include <FS.h>

// include framework dependcies
#include <RestApi.hpp>
#include <ConfigManager.hpp>

const char* ssid     = "UPC7472663";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "a6Qmsbhnwrdk";     // The password of the Wi-Fi network

AsyncWebServer server(80);
AsyncEventSource events("/events");
RestApi restApi(&server, &events);
ConfigManager configManager;

void setup() {
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  SPIFFS.begin();  // Start the SPI Flash Files System

  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

  configManager.begin(CONFIG_DEFAULT_FILENAME);

  server.begin();
}

void loop() { 

}