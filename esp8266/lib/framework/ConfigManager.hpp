#ifndef CONFIGMANAGER
#define CONFIGMANAGER

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Error.h>

#define MAX_CONFIG_ARRAY            (50)
#define CONFIG_DEFAULT_FILENAME     "/config/config.json"
#define CONFIG_STATUS_CHANGED       "changed"

typedef struct ConfigEntry_t{
    String key;
    String value;
}ConfigEntry;

class ConfigManager{
    private:
        DynamicJsonDocument jsonDoc;
        DynamicJsonDocument jsonDocStatus;
        JsonObject configurations;          //stores configurations data
        JsonObject configurationStatus;     //stores configuration status
        ErrorType saveFile();               //internal function to save the file after modification

    public:
        ConfigManager();
        ErrorType begin();
        String getAllConfig();
        ErrorType getConfig(ConfigEntry *config, String key);
        String getConfigValue(String key);
        ErrorType updateConfig(String key, String value);
        String getConfigStatus(String key);
        ErrorType updateConfigStatus(String key, String status);
        unsigned int getConfigSize();

};

#endif