#include "ConfigManager.hpp"

ConfigManager::ConfigManager(){
    this->inited = false;
    this->configSize = 0;
}

ErrorType ConfigManager::begin(const char* filename){
    ErrorType ret = RET_OK;
    if(filename != NULL){
        File file = SPIFFS.open(filename, "r");
        if(!file){
            Serial.printf("Could not open %s\n", filename); // replace this with DEBUG printf
            file.close();
            return ERR_OPEN_CONFIG_FILE;
        }

        DynamicJsonDocument json(512);
        DeserializationError error = deserializeJson(json, file);
        if(error){
            Serial.print("Load config error: ");
            Serial.println(error.c_str());
            return ERR_JSON_CONFIG_FILE;
        }

        // https://arduinojson.org/v6/api/jsonobject/begin_end/
        JsonObject root = json.as<JsonObject>();
        Serial.printf("Loading configs...\n");
        for(JsonPair pair : root){
            ConfigEntry* entry = &this->configList[this->configSize];

            entry->key = pair.key().c_str();
            entry->value = pair.value().as<const char*>();
            Serial.printf("\t%s:%s\n", entry->key.c_str(), entry->key.c_str());
            this->configSize++;

            if(this->configSize == MAX_CONFIG_ARRAY) break; // array is full
        }
        Serial.printf("Loaded %d configs\n", this->configSize);


    }

    return ret;
}

unsigned int ConfigManager::getConfigSize(){
    return this->configSize;
}