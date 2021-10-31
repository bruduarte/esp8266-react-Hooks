#include "ConfigManager.hpp"

ConfigManager::ConfigManager() : jsonDoc(512), jsonDocStatus(512) {
    // initialize the jsonDoc and jsonDocStatus with 512 bytes
    this->configurationStatus = this->jsonDocStatus.to<JsonObject>();
}

//this function loads the configuration file from fileSystem
ErrorType ConfigManager::begin(){

    File file = SPIFFS.open(CONFIG_DEFAULT_FILENAME, "r");

    if (!file){
        Serial.printf("Could not open file %s\n", CONFIG_DEFAULT_FILENAME);
        file.close();
        return ERR_OPEN_CONFIG_FILE;
    }
    
    //deserialize the file into a Json document
    DeserializationError error = deserializeJson(this->jsonDoc, file);

    if (error){
        Serial.printf("Load config error: ");
        Serial.println(error.c_str());
        file.close();
        return ERR_JSON_CONFIG_FILE;
    }

    //convert to a JsonObject and use as the configuration
    this->configurations = jsonDoc.as<JsonObject>();
   
    Serial.printf("Loaded %d configurations.\n", this->configurations.size());

    //wrap up
    file.close();
    return RET_OK; //returns ok if everything goes well
    
}

//returns a String with all the configurations found in the config file.
String ConfigManager::getAllConfig(){
    String allConfigurations = {};
    
    // serialize the json object into a string
    serializeJson(this->configurations, allConfigurations);
    
    return allConfigurations;

}

ErrorType ConfigManager::getConfig(ConfigEntry *config, String key){
     if(config != NULL){
        if(this->configurations.containsKey(key)){
            config->key = key;
            config->value = this->configurations[key].as<String>();
            return RET_OK;

        }else return ERR_CONFIG_NOT_FOUND;
    }
    return RET_NOK;
}

String ConfigManager::getConfigValue(String key){
    
    if(this->configurations.containsKey(key)){
        return this->configurations[key].as<String>();
    }else{
        return "configuration_not_found.";
    }
    
}

ErrorType ConfigManager::updateConfig(String key, String value){
    //avoid using empty content
    if(key.length() > 0 && value.length() > 0){
        //Check if the key exists
        if(this->configurations.containsKey(key)){
            //updates the configuration
            this->configurations[key] = value;
            this->configurationStatus[key] = "changed";
            //save to file the modification
            return this->saveFile();
        } else {
            return ERR_CONFIG_NOT_FOUND;
        }
    } else {
        return ERR_CONFIG_EMPTY_KEY_VALUE;
    }
    return RET_OK;
    
}

//return size of configurations object
unsigned int ConfigManager::getConfigSize(){
    return this->configurations.size();
}

ErrorType ConfigManager::saveFile(){
    File file = SPIFFS.open(CONFIG_DEFAULT_FILENAME, "w");

    if(!file){
        Serial.printf("Could not open file %s.\n", CONFIG_DEFAULT_FILENAME);
        file.close();
        return ERR_OPEN_CONFIG_FILE;
    }

    //write to file
    size_t writtenBytes = serializeJson(jsonDoc, file);
    Serial.printf("File saved.\n");
    Serial.printf("Wrote %d bytes.\n", writtenBytes);

    file.flush();
    file.close();
    

    return RET_OK;
}


ErrorType ConfigManager::updateConfigStatus(String key, String status){
    //avoid using empty content
    if (key.length() > 0 && status.length() > 0){
        //if key doesn't exists, add it.
        this->configurationStatus[key] = status;
        return RET_OK;
    }else{
        return ERR_CONFIG_EMPTY_KEY_VALUE;
    }
    
}

String ConfigManager::getConfigStatus(String key){
    //avoid using empty content
    if (key.length() > 0){
        //check if key exists
        if (this->configurationStatus.containsKey(key)){
            return this->configurationStatus[key];
        }else{
            return "not_found";
        }
        
    }else{
        return "empty_key";
    }
    
}