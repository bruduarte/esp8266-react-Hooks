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
#include <WiFiManager.hpp>

const char* ssid     = "UPC7472663";          // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "a6Qmsbhnwrdk";        // The password of the Wi-Fi network
const char* ntpServer = "pool.ntp.org";       // default ntp server
int ntpOffset = 0;                            // default ntp offset


WiFiUDP wifiUDP;
AsyncWebServer server(80);
AsyncEventSource events("/events");
ConfigManager configurationManager;
WiFiManager wifiManager(&configurationManager);
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

void setupWebServer(){

	Serial.print("Setup webserver...\n");
	server.begin();
	Serial.println("Done :) ...\n");

}

void setupTime(){

	ConfigEntry config;
	Serial.println("Setup time...\n");
	ErrorType error = configurationManager.getConfig(&config, TIMEMAN_CONFIG_NTP_OFFSET);
	if (error == RET_OK){
		int offset = atoi(config.value.c_str());
		timeManager.setTimeOffset(offset, false);

		Serial.printf("\tNTP server: %s\n", ntpServer);
    	Serial.printf("\tTime offset: %d\n", offset);
	}else{
    	Serial.printf("\tCould not get config %s.\n", TIMEMAN_CONFIG_NTP_OFFSET);
  	}
	timeManager.begin();
	Serial.println("Done :) ...\n");
	
}

void setupConfig(){

	Serial.println("Initializing configuration...\n");
	configurationManager.begin();
	Serial.printf("\tLoaded %d configurations.\n", configurationManager.getConfigSize());
	Serial.println("Done :) ...");
}


void setupWifi(){
	wifiManager.begin();
	
}


void setup() {
  
	setupESP();
	setupConfig();
  	setupWifi();
  	setupWebServer();
	setupTime();

}

void loopTime(){
	ConfigEntry config;
	int ntpDSTstartDate = atoi(configurationManager.getConfigValue(TIMEMAN_CONFIG_NTP_DST_START_DATE).c_str()); //gets the start date of DST as ineger
	int ntpDSTendDate = atoi(configurationManager.getConfigValue(TIMEMAN_CONFIG_NTP_DST_END_DATE).c_str()); //gets the end date of DST as integer

	if (timeManager.getNTPClient()->getEpochTime() > 1609506000){
	// current time is after Friday, January 1, 2021 1:00:00 PM
    // it means we aleady have a valid time
		
		//check if DST is enabled
		if(configurationManager.getConfigValue(TIMEMAN_CONFIG_NTP_DST_ENABLED) == "true"){
			int now = timeManager.getNTPClient()->getEpochTime(); //gets current date
			//check if now is in the range of DST
			if(now >= ntpDSTstartDate &&  now <= ntpDSTendDate){	
				timeManager.setDSToffset(true);
			}else{
				timeManager.setDSToffset(false);
			}	
		}else{
			timeManager.setDSToffset(false);
		}

		if(printTime){
			Serial.print("Current time: ");
			Serial.println(timeManager.getFormattedTtime());
			printTime = false;
		}


  	}


	if(configurationManager.getConfigStatus(TIMEMAN_CONFIG_NTP_OFFSET).equals(CONFIG_STATUS_CHANGED)){
		// config has changed. Reload config and force update
		
		Serial.println("Time config changed");
		ErrorType error = configurationManager.getConfig(&config, TIMEMAN_CONFIG_NTP_OFFSET);
		if(error == RET_OK){
			int offset = atoi(config.value.c_str());
			Serial.printf("Set timeoffset to %d\n", offset);
			timeManager.setTimeOffset(offset, true);  // force update
			
			printTime = true;

			configurationManager.updateConfigStatus(TIMEMAN_CONFIG_NTP_OFFSET, "ok");
		}
	}else{
		// no config change. Just run normal update
		timeManager.update();
	}

	printTime = true;
	delay(1000);
	
}

void loopWifi(){
	// chechk if connected
	// check if config has changed and re-configure wifi


	// if connected, then do nothing related to AP


	// if not connected, then setup AP
	struct station_config conf;
    wifi_station_get_config(&conf);
}

void loop() { 

	loopTime();
	loopWifi();

}