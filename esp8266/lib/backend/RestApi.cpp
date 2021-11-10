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



