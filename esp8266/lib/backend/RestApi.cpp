/*deal with comunication with frontend*/

#include "RestApi.hpp"

RestApi::RestApi(AsyncWebServer* server, AsyncEventSource* events) {
    Serial.print("Setting up generic API Endpoints...");
    events->onConnect([](AsyncEventSourceClient *client){
        client->send("hello!", NULL, millis(), 1000);
    });

    server->addHandler(events);

    server->on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });

    Serial.println("Done.");
}

RestApi::~RestApi() {};