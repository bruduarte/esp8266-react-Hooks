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

    /**
     * 
     * CUSTOM PAGE API
     * 
    */

   /**
    * CALL FOR PAGE COMPONENTS MOUNTING
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
            // Serial.println(customConfig);
            request->send(200, "application/json", customConfig);
        }else{
            request->send(507, "application/json", "Capacity of char* array is not sufficient.");
        }
        delete[] customConfig;

    });
    
    /**
     * Function call after a button is clicked on the frontend
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
     * Function call after a inputbox content is sent from the frontend
    */
    server->on("/inputbox", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, [this](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
        
        //it will send: "name inputValue"
      
        // get the inputbox identification
        data[len]='\0';
        char* input = new char [len];
        strcpy(input, reinterpret_cast<const char*>(data));
        Serial.println(input);

        char* inputSplit = strtok(input, "\" ");
        size_t size = strlen(inputSplit);

        // Serial.println(inputSplit);
        // Serial.println(size);

        char* inputName = new char[size+1];
        strncpy(inputName, inputSplit, size);
        inputName[size+1] = '\0';
        // Serial.println(inputName);

        if(strlen(inputName) > 0){
            
            // get the registered variable for the inputbox
            void* inputVariable = getIputVariable(inputName);

            if(inputVariable != NULL){

                while (inputSplit != NULL){

                    
                    inputSplit = strtok (NULL, "\"");
                    
                    if(inputSplit != NULL){

                        // assign value
                        size = strlen(inputSplit);
                        size_t characters = (MAX_INPUT_SIZE - 1) < size ?  (MAX_INPUT_SIZE - 1) : size;
                        strncpy((char*)inputVariable, inputSplit, characters);
                        ((char*)inputVariable)[characters+1] = '\0';

                        request->send(200, "application/json", "");

                    }   
                }
           }else{
                Serial.println("Variable not found");
                request->send(501, "text/plain", "Variable was not found!");
           }

        }else{
            request->send(406, "text/plain", "No input name was provided!");
        }
        delete [] input;
        delete [] inputName;
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

/**
 * 
 * Functions to register custom page components
 * 
*/


void RestApi::registerButton(void (*function)(), const char* buttonName){

    if(function != NULL && strlen(buttonName) > 0 && componentsCounter < MAX_PAGE_COMPONENTS){
        
        size_t characters = MAX_COMP_NAME < strlen(buttonName) ? MAX_COMP_NAME : strlen(buttonName);
       
        Button* button = new Button;
        button->function = function;
        button->type = TYPE_BUTTON;

        Serial.println(characters);
        
        strncpy(button->name, buttonName, characters);
        button->name[characters] = '\0';

        this->pageComponents[componentsCounter] = button;
    
        Serial.println(button->name);

        this->componentsCounter++;
    } 
    Serial.println("Button registered! \n");

}

void RestApi::registerInput(void* variable, const char* inputName, const char* placeholder, const char* label, const char* httpInputType){
    if(componentsCounter < MAX_PAGE_COMPONENTS && variable != NULL && strlen(inputName) > 0 && strlen(placeholder) > 0 && strlen(label) > 0 && strlen(httpInputType) > 0 ){
        Input* input = new Input;

        input->variable = variable;
        input->type = TYPE_INPUT;

        size_t characters = MAX_COMP_NAME < strlen(inputName) ? MAX_COMP_NAME : strlen(inputName);
        strncpy(input->name, inputName, characters);

        characters = MAX_INPUT_PLACEHOLDER < strlen(placeholder) ? MAX_INPUT_PLACEHOLDER : strlen(placeholder);
        strncpy(input->placeholder, placeholder, characters);

        characters = MAX_INPUT_LABEL < strlen(label) ? MAX_INPUT_LABEL : strlen(label);
        strncpy(input->label, label, characters);

        characters = MAX_INPUT_TYPE < strlen(httpInputType) ? MAX_INPUT_TYPE : strlen(httpInputType);
        strncpy(input->httpInputType, httpInputType, characters);

        this->pageComponents[componentsCounter] = input;

        Serial.println(input->name);
        this->componentsCounter++;
        Serial.println("Input registered! \n");
    }
}

void RestApi::registerCheckbox(bool* variable, const char* name, const char* label){
    if(componentsCounter < MAX_PAGE_COMPONENTS && variable != NULL && strlen(name) > 0 && strlen(label) > 0 ){
        Checkbox* checkbox = new Checkbox;

        checkbox->variable = variable;
        checkbox->type = TYPE_CHECKBOX;

        size_t characters = MAX_COMP_NAME < strlen(name) ? MAX_COMP_NAME : strlen(name);
        strncpy(checkbox->name, name, characters);

        characters = MAX_INPUT_LABEL < strlen(label) ? MAX_INPUT_LABEL : strlen(label);
        strncpy(checkbox->label, label, characters);

        this->pageComponents[componentsCounter] = checkbox;

        Serial.println(checkbox->name);
        this->componentsCounter++;
        Serial.println("Checkbox registered! \n");
    }
}


ErrorType RestApi::customPageObjects(char* pageObjects, size_t capacity){
    StaticJsonDocument<1000> doc;
    StaticJsonDocument<1000> page;
    JsonArray objectsJson = doc.to<JsonArray>();
    
    for (int i = 0; i < this->componentsCounter; i++){
        if(this->pageComponents[i]->type == TYPE_BUTTON){
            JsonObject object = page.to<JsonObject>();
            object["name"] = (const char*)this->pageComponents[i]->name;
            object["type"] = "button";
            objectsJson.add(object);
            // Serial.println("Object added!");

        }else if(this->pageComponents[i]->type == TYPE_INPUT){
            JsonObject object = page.to<JsonObject>();
            object["name"] = (const char*)this->pageComponents[i]->name;
            object["inputType"] = ((Input*)(const char*)this->pageComponents[i])->httpInputType;
            object["label"] = ((Input*)(const char*)this->pageComponents[i])->label;
            object["placeholder"] = ((Input*)(const char*)this->pageComponents[i])->placeholder;
            object["type"] = "input";

            objectsJson.add(object);
            Serial.println("Object added!");
        }else if(this->pageComponents[i]->type == TYPE_CHECKBOX){
            JsonObject object = page.to<JsonObject>();
            object["name"] = (const char*)this->pageComponents[i]->name;
            object["label"] = ((Checkbox*)(const char*)this->pageComponents[i])->label;
            object["type"] = "checkbox";

            objectsJson.add(object);
            Serial.println("Object added!");
        }
    }

    serializeJson(objectsJson, Serial);
    Serial.println("");
    size_t characters = serializeJson(objectsJson, pageObjects, capacity);
    
    
    if(characters >= capacity){
        return RET_WARNING;
    }

    return RET_OK;

}

buttonFunction RestApi::getButtonFunction(const char* buttonName){
    if(strlen(buttonName) > 0){
        for (int i = 0; i < this->componentsCounter; i++){
           if(strcmp(this->pageComponents[i]->name, buttonName) == 0 && this->pageComponents[i]->type == TYPE_BUTTON){
               return ((Button*)this->pageComponents[i])->function;
           }
        }
        Serial.println("Ops... Button - Function not found!\n");
    }
        return NULL;
} 

void* RestApi::getIputVariable(const char* inputName){
    if(strlen(inputName) > 0){
        for(int i = 0; i < this->componentsCounter; i++){
            if(strcmp(this->pageComponents[i]->name, inputName) == 0 && this->pageComponents[i]->type == TYPE_INPUT){
                return ((Input*)this->pageComponents[i])->variable;
            }
        }
        Serial.println("Ops... Input box not found!\n");
    }
    return NULL;
}