#ifndef WIFIMANAGER
#define WIFIMANAGER

#include <ESP8266WiFi.h>
#include "ConfigManager.hpp"

#define WIFIMAN_CONFIG_SSID			"wifiSSID"
#define WIFIMAN_CONFIG_PASSWORD		"wifiPassword"
#define WIFIMAN_AP_SSID				"ESP_ACCESS_POINT"
#define WIFIMAN_AP_PASSWORD			"@StqTleN21"

class WiFiManager{
	private:
		String wifiSSID;
		String wifiPassword;
		ConfigManager* configurations;
		

	public:
		WiFiManager(ConfigManager* configurations);
		bool begin();
		void setupAccessPoint();
		void updateCredentials(String& SSID, String& password);
		unsigned long getIPaddress();
		bool isConnected();

};




#endif