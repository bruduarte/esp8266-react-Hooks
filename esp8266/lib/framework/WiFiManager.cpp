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
    
    //retrieving SSID information form the configurations file
	ErrorType getSSID = this->configurations->getConfig(&config, WIFIMAN_CONFIG_SSID);
	
	if(getSSID == RET_OK){
        Serial.println("SSID: ");
        Serial.println(config.value);
		this->wifiSSID = config.value;

        //retrieving password information form the configurations file
		ErrorType getPass = this->configurations->getConfig(&config, WIFIMAN_CONFIG_PASSWORD);

		if (getPass == RET_OK){
            Serial.println("Retrieved password!");
			this->wifiPassword = config.value;
            return true;

		}else{
    		this->wifiPassword = "";
            Serial.printf("\tCould not get config %s.\n", WIFIMAN_CONFIG_PASSWORD);
            return false;
		}
	
  	}else{
          this->wifiSSID = "";
		  Serial.printf("\tCould not get config %s.\n", WIFIMAN_CONFIG_SSID);
          return false;
	}
    
}

//functions that manages the connection to WiFi
bool WiFiManager::startConnection(){

    Serial.println("Connecting to...");
    Serial.println(this->wifiSSID);
    Serial.println(" ...");

    // Connect to the network
    WiFi.begin(this->wifiSSID, this->wifiPassword); 
	
    //Try connection...
    for(int tries = 0; tries < 10; tries++){
        if(WiFi.status() == WL_CONNECTED){
            Serial.println("\n\tConnection established!");  
	        Serial.print("\tIP address:\t");
	        Serial.println(getIPaddress());  // Send the IP address of the ESP8266 to the computer
            return true;
        }
        delay(1000);
        // Serial.println(WiFi.status());
    }
    Serial.println("Connection timed out. Please check your credentials.\n");
    return false;
}


bool WiFiManager::setupAccessPoint(){

    Serial.println("Setting Access Point ... ");
    WiFi.hostname(WIFIMAN_HOST_NAME);
    bool accessPoint = WiFi.softAP(WIFIMAN_AP_SSID);
    if (accessPoint){
        Serial.println("Ready for connection...");
        Serial.println("IP Address: ");
        Serial.println(WiFi.softAPIP());
        if(!this->dnsServer){
            this->dnsServer = new DNSServer;
        }
        this->dnsServer->start(WIFIMAN_DNS_PORT, "*", WiFi.softAPIP());
        MDNS.begin(WIFIMAN_HOST_NAME, WiFi.softAPIP());
        return true;
    }else{
        Serial.println("Failed to setup access point.");
        return false;
    }
    
}

void WiFiManager::updateCredentials(String& SSID, String& password){
    this->wifiSSID = SSID;
    this->wifiPassword = password;
}



IPAddress WiFiManager::getIPaddress(){
   return WiFi.localIP();
   
}

void WiFiManager::setupMdns() {
    if(MDNS.begin("esp8266manager", getIPaddress())){
        Serial.println("MDNS Responder Started.");
    }
    MDNS.addService("http","tcp",80);
}

bool WiFiManager::isConnected(){
    if(WiFi.status() == WL_CONNECTED){
        // Serial.println("\n\tConnection established!");  
        return true;
    }else{
        // Serial.println("Currently not connected to WiFi.\n");
        return false;
    }
    
}

void WiFiManager::disconnect(){
    WiFi.disconnect();
}