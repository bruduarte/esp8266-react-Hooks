#include "TimeManager.hpp"

TimeManager::TimeManager(UDP& udp, const char* server, int offset) : udp(udp){
    this->ntpClient = new NTPClient(this->udp, server, offset);
}

TimeManager::~TimeManager(){
    this->ntpClient->end();
    delete this->ntpClient;
}

void TimeManager::begin(){
    this->ntpClient->begin();
}

void TimeManager::setTimeOffset(int offset, bool update){
    this->ntpClient->setTimeOffset(offset);

    if (update){
        Serial.println("NTP offset changed. Updating time...");
        this->ntpClient->forceUpdate();
    }
    
}

void TimeManager::update(){
    this->ntpClient->update();
}

void TimeManager::setFlag(unsigned int mask){
    this->flags |= mask;
}

void TimeManager::clearFlag(unsigned int mask){
    this->flags &= ~mask;
}

unsigned int TimeManager::getFlag(){
    return this->flags;
}


//to get access to all NTPClient functions
NTPClient* TimeManager::getNTPClient(){
    return this->ntpClient;
}