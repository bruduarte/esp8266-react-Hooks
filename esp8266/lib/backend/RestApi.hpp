#ifndef MYRESTAPI
#define MYRESTAPI

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

//includes from project

#include <ConfigManager.hpp>
#define MAX_CUSTOM_BUTTONS      10

//struct to track custom buttons to the function
typedef struct ButtonFunctions_t{
    void (*function)();
    String buttonName;
}ButtonFucntions;

class RestApi {
    private:

    ButtonFucntions buttons[MAX_CUSTOM_BUTTONS];
    int buttonsCounter = 0;

    public:
        RestApi(AsyncWebServer* server, AsyncEventSource* events, ConfigManager *configurationManager);
        ~RestApi();
        void registerButton(void (*function)(), String buttonName); //receives a void function without args
        // void registerInput(void (*function)(), String inputName);
        // void registerCheckbox(bool* variable, String checkBoxName);
        String customPageObjects ();

    protected:
};
#endif