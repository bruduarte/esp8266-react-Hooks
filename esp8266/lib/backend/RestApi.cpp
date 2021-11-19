/*deal with comunication with frontend*/


#include "RestApi.hpp"

RestApi::RestApi(AsyncWebServer* server, AsyncEventSource* events, ConfigManager *configurationManager) {
    Serial.print("Setting up generic API Endpoints...");
    events->onConnect([](AsyncEventSourceClient *client){
        client->send("hello!", NULL, millis(), 1000);
    });

    server->addHandler(events);

    server->on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });

    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello ESP8266!");
    });

    
   /**
     * Configuration API
     * */  

    
    /**
     * LIST
     * */
    server->on("/config/list", HTTP_GET, [configurationManager](AsyncWebServerRequest *request){
        String list = configurationManager->getAllConfig();
        request->send(200, "application/json", list);
    });

    /**
     * Custom page API
    */
    server->on("/custompage", HTTP_GET, [this](AsyncWebServerRequest *request){

     /*   String customConfig = "[ \
        {\
            \"name\": \"Button1\",\
            \"type\": \"button\"\
        },\
        {\
            \"name\": \"Button2\",\
            \"type\": \"button\"\
        }\
    ]";*/
        size_t capacity = 500;
        char* customConfig = new char[capacity];
        ErrorType error = customPageObjects(customConfig, capacity);

        if(error == RET_OK){
            Serial.println(customConfig);
            request->send(200, "application/json", customConfig);
        }else{
            request->send(507, "application/json", "Capacity of char* array is not sufficient.");
        }
        delete[] customConfig;

    });
    
    /**
     * 
     * Function call after a button is clicked on the frontend
     * 
    */
    server->on("/button", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, [this](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
 
      
        // get the button identification
        data[len]='\0';
        char* buttonName = new char [len];
        strcpy(buttonName, reinterpret_cast<const char*>(data));
        Serial.println(buttonName);

        buttonName = strtok(buttonName, "\"");
       
        Serial.println(buttonName);
        

        // get the registered funtion for the clicked button
        void (*function)();
        function = getButtonFunction(buttonName);
        
        delete [] buttonName;
        // execute it
        if(function != NULL){
            function();
            request->send(200, "application/json", "");

        }else{
            Serial.println("Function returned null");
            request->send(501, "text/plain", "Function not implemented!");
        }

  });



    /**
     * UPDATE
     * 
     * https://github.com/me-no-dev/ESPAsyncWebServer#json-body-handling-with-arduinojson
     * 
     * This callback can be used only for POST/PUT/PATCH HTTP methods
     * 
     * Expected json format: 
     *    {
     *      "key": value,
     *      "key": value,
     *      .
     *      .
     *      .    
     *    }
     * 
     * */ 

    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/config/update", [configurationManager](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonObject jsonObj = json.as<JsonObject>();
        Serial.println("REST:/config/update");
        String txt = "";

        for (JsonPair kv : jsonObj){
            String key = kv.key().c_str();
            String value = kv.value();
            Serial.printf("\t%s : %s\n", key.c_str(), value.c_str());

            ErrorType error = configurationManager->updateConfig(key, value);

            // check if config update was ok
            if(error != RET_OK) {
                txt += "Failed to update ";
                txt += kv.key().c_str();
                Serial.println(txt);
                break;
            } else {txt = "Updated!";}

        }
        
        request->send(200, "text/plain", txt);
        

    });

    server->addHandler(handler);

    server->onNotFound([](AsyncWebServerRequest* request) {
        if (request->method() == HTTP_OPTIONS) {
        request->send(200);
        } else {
        request->send(404);
        }
    });


    // Disable CORS if required
    #if defined(ENABLE_CORS)
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", CORS_ORIGIN);
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
    #endif
    
}


 
RestApi::~RestApi() {};



void RestApi::registerButton(void (*function)(), const char* buttonName){

    if(function != NULL && strlen(buttonName) > 0 && componentsCounter < MAX_PAGE_COMPONENTS){
        
        size_t characters = MAX_PAGE_COMPONENTS < strlen(buttonName) ? MAX_PAGE_COMPONENTS : strlen(buttonName);
       
        Button* button = new Button;
        button->function = function;
        button->type = TYPE_BUTTON;

        Serial.println(characters);
        
        strncpy(button->name, buttonName, characters);
        button->name[characters] = '\0';

        this->pageComponents[componentsCounter] = button;
    
        Serial.println(button->name);
        Serial.println("\n");
        this->componentsCounter++;
    } 
    Serial.println("Button registered! \n");

}


ErrorType RestApi::customPageObjects(char* pageObjects, size_t capacity){
    StaticJsonDocument<200> doc;
    StaticJsonDocument<50> page;
    JsonArray objectsJson = doc.to<JsonArray>();
    
    for (int i = 0; i < this->componentsCounter; i++){
        if(this->pageComponents[i]->type == TYPE_BUTTON){
            JsonObject object = page.to<JsonObject>();
            object["name"] = (const char*)this->pageComponents[i]->name;
            object["type"] = "button";
            objectsJson.add(object);
            // Serial.println("Object added!");

        }
    }

    // serializeJson(objectsJson, Serial);
    size_t characters = serializeJson(objectsJson, pageObjects, capacity);
    
    
    if(characters >= capacity){
        return RET_WARNING;
    }

    return RET_OK;

}

buttonFunction RestApi::getButtonFunction(const char* buttonName){
    if(strlen(buttonName) > 0){
        for (int i = 0; i < this->componentsCounter; i++){
           if(strcmp(this->pageComponents[i]->name, buttonName) == 0 && this->pageComponents[i]->type == TYPE_BUTTON) {
               return ((Button*)this->pageComponents[i])->function;
           }
        }
        Serial.println("Ops... Button - Function not found!\n");
    }
        return NULL;
} 