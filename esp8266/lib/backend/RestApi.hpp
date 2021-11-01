#ifndef MYRESTAPI
#define MYRESTAPI

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

//includes from project

#include <ConfigManager.hpp>

class RestApi {
    public:
        RestApi(AsyncWebServer* server, AsyncEventSource* events, ConfigManager *configurationManager);
        ~RestApi();
        

    protected:
};
#endif