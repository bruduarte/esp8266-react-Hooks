#ifndef MYRESTAPI
#define MYRESTAPI

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

//includes from project

#include <ConfigManager.hpp>
#include <Error.h>

#define MAX_PAGE_COMPONENTS     50
#define MAX_COMP_NAME           30
#define MAX_COMP_TYPE           10
#define MAX_INPUT_PLACEHOLDER   50
#define MAX_INPUT_LABEL         50
#define MAX_INPUT_TYPE          20
#define MAX_INPUT_SIZE          50


typedef void (*buttonFunction)();

//struct to track custom buttons to the function
typedef struct ButtonFunctions_t{
    void (*function)();
    String buttonName;

}ButtonFucntions;

// enum inputTypes_t {
//     TYPE_INT,
//     TYPE_FLOAT,
//     TYPE_CHAR,
//     TYPE_STRING,
//     TYPE_BOOL

// };

enum componentType_t {
    TYPE_BUTTON,
    TYPE_INPUT,
    TYPE_CHECKBOX  
};

class Components {
    public:
        char name[MAX_COMP_NAME + 1];
        componentType_t type;
};

class Button: public Components {
    public:
        void (*function)();

};

class Input: public Components {
    public:
        char placeholder[MAX_INPUT_PLACEHOLDER + 1];
        char label[MAX_INPUT_LABEL + 1];
        char httpInputType[MAX_INPUT_TYPE + 1];
        void* variable;
        // inputTypes_t inputType;


};

class Checkbox: public Components {
    public:
        char label[MAX_INPUT_LABEL];
        bool* variable;
};

class RestApi {
    private:

    Components* pageComponents[MAX_PAGE_COMPONENTS];
    int componentsCounter = 0;

    public:
        RestApi(AsyncWebServer* server, AsyncEventSource* events, ConfigManager *configurationManager);
        ~RestApi();
        void registerButton(void (*function)(), const char* buttonName); //receives a void function without args
        void registerInput(void* variable, const char* inputName, const char* placeholder, const char* label, const char* httpInputType);
        void registerCheckbox(bool* variable, const char* name, const char* label);


        ErrorType customPageObjects (char* pageObjects, size_t capacity);
        buttonFunction getButtonFunction(const char* buttonName); 
        void* getIputVariable(const char* inputName);

    protected:
};
#endif