#ifndef WIFIMANAGER
#define WIFIMANAGER

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include "ConfigManager.hpp"

#define WIFIMAN_CONFIG_SSID			"wifiSSID"
#define WIFIMAN_CONFIG_PASSWORD		"wifiPassword"
#define WIFIMAN_AP_SSID				"ESP_ACCESS_POINT"
#define WIFIMAN_AP_PASSWORD			"@StqTleN21"
#define WIFIMAN_HOST_NAME			"esp8266manager"
#define WIFIMAN_DNS_PORT			53

class WiFiManager{
	private:
		String wifiSSID;
		String wifiPassword;
		ConfigManager* configurations;
		DNSServer* dnsServer;

	public:
		WiFiManager(ConfigManager* configurations);
		bool begin();
		bool startConnection();
		void setupAccessPoint();
		void updateCredentials(String& SSID, String& password);
		IPAddress getIPaddress();
		void setupMdns();
		bool isConnected();

};




#endif