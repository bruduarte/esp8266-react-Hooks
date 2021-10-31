#ifndef CONFIGMANAGER
#define CONFIGMANAGER

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Error.h>

#define MAX_CONFIG_ARRAY            (50)
#define CONFIG_DEFAULT_FILENAME     "/config/config.json"

typedef struct ConfigEntry_t{
    String key;
    String value;
}ConfigEntry;

class ConfigManager{
    private:
        DynamicJsonDocument jsonDoc;
        JsonObject configurations;

        ErrorType saveFile(); //internal function to save the file after modification

    public:
        ConfigManager();
        ErrorType begin();
        String getAllConfig();
        ErrorType getConfig(ConfigEntry *config, String key);
        ErrorType updateConfig(String key, String value);
        unsigned int getConfigSize();

};

#endif