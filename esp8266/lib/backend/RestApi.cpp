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

   
   /**
     * Configuration API
     * */  

    

    // list
    server->on("/config/list", HTTP_GET, [configurationManager](AsyncWebServerRequest *request){
        String list = configurationManager->getAllConfig();
        request->send(200, "application/json", list);
    });

    // update
    // https://github.com/me-no-dev/ESPAsyncWebServer#json-body-handling-with-arduinojson
    // this callback can be used only for POST/PUT/PATCH HTTP methods
    // expected json format:
    // {
    // "wifiSSID": "the wifi ssid",
    // "wifiPassword": "the wifi password"
    // }
    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/config/update", [configurationManager](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonObject jsonObj = json.as<JsonObject>();
        Serial.println("REST:/config/update");

        // check if expected keys are provided
        if(jsonObj.containsKey("wifiSSID") && jsonObj.containsKey("wifiPassword")){
            // update config
            ErrorType error = configurationManager->updateConfig("wifiSSID", jsonObj["wifiPassword"]);

            // check if config update was ok
            if(error == RET_OK) request->send(200, "text/plain", "ok");
            else request->send(200, "text/plain", "error: key not found"); //Check errors!!! 

        }else{
            // json is in wrong format
            request->send(200, "text/plain", "error: wrong format");
        }

    });
    server->addHandler(handler);

    // add
    server->on("/config/add", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam("body", true)) {
            message = request->getParam("body", true)->value();
        } else {
            message = "No message sent";
        }
        Serial.println(message.c_str());

        int args = request->args();
        for(int i=0;i<args;i++){
        Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
        }

        request->send(200, "text/plain", "ok");
    });

    // delete
    server->on("/config/delete", HTTP_DELETE, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });

    Serial.println("Done.");
}


 
RestApi::~RestApi() {};

