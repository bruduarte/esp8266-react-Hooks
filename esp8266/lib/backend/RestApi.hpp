#pragma once

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>


class RestApi {
    public:
        RestApi(AsyncWebServer* server, AsyncEventSource* events);
        ~RestApi();

    protected:
};