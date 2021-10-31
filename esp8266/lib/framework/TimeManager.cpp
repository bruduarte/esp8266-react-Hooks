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

void TimeManager::setDSToffset(bool DSTactive){
    this->daylightSavingTime = DSTactive;
}

unsigned long TimeManager::getCurrentTime(){
    if (this->daylightSavingTime){
       return this->ntpClient->getEpochTime() + TIMEMAN_CONFIG_NTP_DST_OFFSET;
    }
    else{
        return this->ntpClient->getEpochTime();
    }
    
}

String TimeManager::getFormattedTtime(){
  unsigned long rawTime = this->getCurrentTime();
  unsigned long hours = (rawTime % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = (rawTime % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = rawTime % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;

}


//to get access to all NTPClient functions
NTPClient* TimeManager::getNTPClient(){
    return this->ntpClient;
}