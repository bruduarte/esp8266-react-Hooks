#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <WiFiUdp.h>
#include <string>

// include framework dependcies
#include <RestApi.hpp>
#include <ConfigManager.hpp>
#include <TimeManager.hpp>
#include <WiFiManager.hpp>

//Configuration Manager
ConfigManager configurationManager;

//Time Management
bool printTime = true;
const char* ntpServer = "pool.ntp.org";       // default ntp server
int ntpOffset = 0;                            // default ntp offset
WiFiUDP wifiUDP;
TimeManager timeManager(wifiUDP, ntpServer, ntpOffset);

//WiFi Management
bool APMode = false;
enum class WifiStates {LOADING_CONFIGS, CHECK_STATUS, STARTING_CONNECTION, START_AP, WAIT_CHANGE};
WifiStates state = WifiStates::LOADING_CONFIGS;
WiFiManager wifiManager(&configurationManager);

//Server
AsyncWebServer server(80);
RestApi restApi(&server, &configurationManager);

//Custom page
char input[MAX_INPUT_SIZE]; //variable for input field
bool toggle; //variable for checkbox
//function to be called when a button is clicked
void testButtonFunction () {
	Serial.println("Button clicked!");
}

//helper variables
unsigned long timeElapsed = 0;
unsigned long timeElapsedWifi = 0;


void setupESP(){

	Serial.begin(9600);         // Start the Serial communication to send messages to the computer
	delay(10);
	Serial.println("Setup ESP8266...");

	SPIFFS.begin();  // Start the SPI Flash Files System

	timeElapsed = millis();
	timeElapsedWifi = millis();

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


void setup() {
  
	setupESP();
	setupConfig();
  	setupWebServer();
	setupTime();


	restApi.registerButton(testButtonFunction, "Button 1");
	restApi.registerInput(&input, "Example", "Type your name.", "Name", "text");
	restApi.registerButton(testButtonFunction, "Button 2");
	restApi.registerCheckbox(&toggle, "toggleLED", "On/Off");
	restApi.registerButton(testButtonFunction, "Button 3");
	

}

void loopTime(){
	//only retrieves time if connected to network
	if(!APMode){

		ConfigEntry config;
		int ntpDSTstartDate = atoi(configurationManager.getConfigValue(TIMEMAN_CONFIG_NTP_DST_START_DATE).c_str()); //gets the start date of DST as integer
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
			
			Serial.println("Time config changed.");
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

		//only to see if time is correct
		if(millis() - timeElapsed >= 1000){
			printTime = true;
			timeElapsed = millis();
		}
		
	}
	
}

void loopWifi(){
	
	switch (state)
	{
	case WifiStates::LOADING_CONFIGS :
		Serial.println("Loading wifi credentials...\n");
		wifiManager.begin();
		state = WifiStates::CHECK_STATUS;
		break;

	case WifiStates::CHECK_STATUS:
		Serial.println("Checking wifi status...\n");
		if(wifiManager.isConnected()){
			state = WifiStates::WAIT_CHANGE;
		}else{
			state = WifiStates::STARTING_CONNECTION;
		}
		break;

	case WifiStates::STARTING_CONNECTION:
		if(wifiManager.startConnection()){
			state = WifiStates::WAIT_CHANGE;
		}else{
			state = WifiStates::START_AP;
		}
		break;

	case WifiStates::START_AP:
		if(wifiManager.setupAccessPoint()){
			APMode = true;
			state = WifiStates::WAIT_CHANGE;
		}

		break;

	case WifiStates::WAIT_CHANGE:

		// Checks if wither SSID or Password has changed
		if( configurationManager.getConfigStatus(WIFIMAN_CONFIG_SSID).equals(CONFIG_STATUS_CHANGED) || 
			configurationManager.getConfigStatus(WIFIMAN_CONFIG_PASSWORD).equals(CONFIG_STATUS_CHANGED)){

			Serial.println("Wifi credentials changed.");
			configurationManager.updateConfigStatus(WIFIMAN_CONFIG_SSID, "ok");
			configurationManager.updateConfigStatus(WIFIMAN_CONFIG_PASSWORD, "ok");
			wifiManager.disconnect();
			APMode = false;
			state = WifiStates::LOADING_CONFIGS;

		}
		else if(APMode){
			if(millis() - timeElapsedWifi >= 5000){
				Serial.println("APMode on...");
				timeElapsedWifi = millis();
			}
		}
		else if(!wifiManager.isConnected()){
			state = WifiStates::LOADING_CONFIGS;
			Serial.println("Trying to connect to WiFi...");
			
		}else {
			//Just wait...
			if(millis() - timeElapsedWifi >= 1000){
				Serial.println("...");
				timeElapsedWifi = millis();
			}
		}

		break;
	
	default:
		state = WifiStates::LOADING_CONFIGS;
		break;
	}
	

}

void loop() { 
	
	loopTime();
	loopWifi();
	delay(10);

}	

