#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncEventSource.h>
#include <FS.h>
#include <WiFiUdp.h>
#include <string>

// include framework dependcies
#include <RestApi.hpp>
#include <ConfigManager.hpp>
#include <TimeManager.hpp>

const char* ssid     = "UPC7472663";          // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "a6Qmsbhnwrdk";        // The password of the Wi-Fi network
const char* ntpServer = "pool.ntp.org";       // default ntp server
int ntpOffset = 0;                            // default ntp offset

WiFiUDP wifiUDP;
AsyncWebServer server(80);
AsyncEventSource events("/events");
ConfigManager configurationManager;
TimeManager timeManager(wifiUDP, ntpServer, ntpOffset);
RestApi restApi(&server, &events, &configurationManager);

bool printTime = true;

void setupESP(){

	Serial.begin(9600);         // Start the Serial communication to send messages to the computer
	delay(10);
	Serial.println("Setup ESP8266...\n");

	SPIFFS.begin();  // Start the SPI Flash Files System

	Serial.println("Done :) ...\n");
}

void setupWifi(){

	Serial.println("Setup wifi...");
	WiFi.begin(ssid, password);             // Connect to the network
	Serial.print("\tConnecting to ");
	Serial.print(ssid); 
	Serial.print(" ...");

	int i = 0;
	while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
		delay(1000);
		Serial.print(++i); 
		Serial.print(' ');
	}

	Serial.println("\n\tConnection established!");  
	Serial.print("\tIP address:\t");
	Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

	Serial.println("Done :) ...\n");
}

void setupWebServer(){

	Serial.print("Setup webserver...\n");
	server.begin();
	Serial.println("Done :) ...\n");

}

void setupTime(){

	ConfigEntry config;
	Serial.println("Setup time...\n");
	ErrorType error = configurationManager.getConfig(&config, "timeOffset");
	if (error == RET_OK){
		int offset = atoi(config.key.c_str());
		timeManager.setTimeOffset(offset, false);
	}
	timeManager.begin();
	Serial.println("Done :) ...\n");
	
}

void setupConfig(){

	Serial.println("Initializing configuration...\n");
	configurationManager.begin();

	Serial.println("Done :) ...");
}



void setup() {
  
	setupESP();
	setupConfig();
  	setupWifi();
  	setupWebServer();
	setupTime();

}

void loopTime(){

	if (timeManager.getNTPClient()->getEpochTime() > 1609506000){
	// current time is after Friday, January 1, 2021 1:00:00 PM
    // it means we aleady have a valid time
		if(printTime){
			Serial.print("Current time: ");
			Serial.println(timeManager.getNTPClient()->getFormattedTime());
			printTime = false;
		}
  }
	if(timeManager.getFlag() & TIME_MAN_FLAG_CONFIG_CHANGED){
		// config has changed. Reload config and force update
		ConfigEntry config;
		ErrorType error = configurationManager.getConfig(&config, "timeOffset");
		if(error == RET_OK){
			int offset = atoi(config.key.c_str());
			timeManager.setTimeOffset(offset, true);  // force update
			timeManager.clearFlag(TIME_MAN_FLAG_CONFIG_CHANGED);
			printTime = true;
		}
	}else{
		// no config change. Just run normal update
		timeManager.update();
	}
	
}


void loop() { 

	loopTime();

}