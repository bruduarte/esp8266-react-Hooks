#include "ConfigManager.hpp"

ConfigManager::ConfigManager(){
    this->initiated = false;
    this->configSize = 0;
    this->fileName = "";
}

//this function loads the configuration file from fileSystem
ErrorType ConfigManager::begin(){
    ErrorType ret = RET_OK; //returns ok if everything goes well

    File file = SPIFFS.open(CONFIG_DEFAULT_FILENAME, "r");
    if (!file){
        Serial.printf("Could not open file %s\n", CONFIG_DEFAULT_FILENAME);
        file.close();
        return ERR_OPEN_CONFIG_FILE;
    }
    
    DynamicJsonDocument json(512);
    DeserializationError error = deserializeJson(json, file);

    if (error){
        Serial.printf("Load config error: ");
        Serial.println(error.c_str());
        return ERR_JSON_CONFIG_FILE;
    }

    JsonObject root = json.as<JsonObject>();
    Serial.printf("Loading configs...\n");

    for (JsonPair kv : root){
        this->configList[this->configSize].key = kv.key().c_str();
        this->configList[this->configSize].value = kv.value().as<const char*>();
        
        Serial.printf("\t %s : %s\n", this->configList[this->configSize].key.c_str(), this->configList[this->configSize].value.c_str());
        this->configSize++;

        if (this->configSize == MAX_CONFIG_ARRAY){
            //array is full
            break;
        }
        
    }
    
    Serial.printf("Loaded %d configurations. \n", this->configSize);

    //wrap up
    file.close();
    return ret;
    
}

unsigned int ConfigManager::getConfigSize(){
    return this->configSize;
}

//returns a String with all the configurations found in the config file.
String ConfigManager::getAllConfig(){
    String ret = {};

    File file = SPIFFS.open(CONFIG_DEFAULT_FILENAME, "r");

    if (!file){
        Serial.printf("Could not open file %s\n", CONFIG_DEFAULT_FILENAME);

        file.close();
        return ret;
    }

    while (file.available()){
        ret += file.readString();
    }
    
    //wrap up

    file.close();

    return ret;

}


ErrorType ConfigManager::updateConfig(String key, String value){
    unsigned int entry = 0;
    bool updated = false;

    for (entry = 0; entry < this->configSize ; entry++){
        if(this->configList[entry].key == key){
             //updates value for given key
            this->configList[entry].value = value;
            Serial.printf("%s: %s\n", key.c_str(), value.c_str());
            updated = true;
        }
    }

    return (updated) ? RET_OK : ERR_CONFIG_NOT_FOUND;
    
}