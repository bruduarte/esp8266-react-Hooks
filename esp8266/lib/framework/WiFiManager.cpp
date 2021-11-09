#include "WiFiManager.hpp"


WiFiManager::WiFiManager(ConfigManager* configurations){
    
    //.reserve() allocates memmory for strings
    // bool errorSSID = this->wifiSSID.reserve(32); 
    // if (errorSSID){
    //     Serial.println("Could not alocate memory for SSID");
    //     return;
    // }

    // bool errorPass = this->wifiPassword.reserve(64);
    // if (errorPass)
    // {
    //     Serial.println("Could not alocate memory for Password");
    //     return;
    // }

	this->configurations = configurations;
    // Serial.println(this->configurations);
    
}

bool WiFiManager::begin(){
    ConfigEntry config; //struct to use the getConfig().
    //retrieving credentials information form the configurations file

	ErrorType getSSID = this->configurations->getConfig(&config, WIFIMAN_CONFIG_SSID);
	
	if(getSSID == RET_OK){
        Serial.println("got ssid");
		this->wifiSSID = config.value;

		ErrorType getPass = this->configurations->getConfig(&config, WIFIMAN_CONFIG_PASSWORD);

		if (getPass == RET_OK){
            Serial.println("got psswd");
			this->wifiPassword = config.value;
		}else{
    		this->wifiPassword = "";
            Serial.printf("\tCould not get config %s.\n", WIFIMAN_CONFIG_PASSWORD);
		}
	
  	}else{
          this->wifiSSID = "";
		  Serial.printf("\tCould not get config %s.\n", WIFIMAN_CONFIG_SSID);
	}
    
    Serial.println("Setup wifi...");
    Serial.println(this->wifiSSID);
    WiFi.begin(this->wifiSSID, this->wifiPassword); // Connect to the network
    Serial.print("\tConnecting to ");
	Serial.print(this->wifiSSID); 
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
    return true;
}

void WiFiManager::setupAccessPoint(){
    Serial.print("Setting Access Point ... ");
    bool softAP = WiFi.softAP(WIFIMAN_AP_SSID, WIFIMAN_AP_PASSWORD);
    if (softAP){
        Serial.println("Ready for connection...");
    }else{
        Serial.println("Failed to setup access point.");
    }
    
}

void WiFiManager::updateCredentials(String& SSID, String& password){
    this->wifiSSID = SSID;
    this->wifiPassword = password;
}



unsigned long WiFiManager::getIPaddress(){
   return WiFi.localIP();
   
}

// bool WiFiManager::isConnected() {
//     Serial.print("Waiting for Wifi to connect...");
//     for(int i = 0; i < 10; ++i) {
//         if (WiFi.status() == WL_CONNECTED)
//         {
//             Serial.println("Done.");
//             return true;
//         }
//         delay(500);
//         Serial.print(WiFi.status());
//     }
//     Serial.println("Connection timed out, opening AP");
//     return false;
// }