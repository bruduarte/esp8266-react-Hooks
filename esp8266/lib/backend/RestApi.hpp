#ifndef MYRESTAPI
#define MYRESTAPI

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

//includes from project

#include <ConfigManager.hpp>

class RestApi {
    private:
    
    public:
        RestApi(AsyncWebServer* server, AsyncEventSource* events, ConfigManager *configurationManager);
        ~RestApi();
        void registerFunction(void (*f)(), String buttonName); //receives a void function without args

    protected:
};
#endif