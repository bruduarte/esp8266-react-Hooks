#ifndef TIMEMANAGER
#define TIMEMANAGER

#include <NTPClient.h>
#include <Udp.h>

#define TIMEMAN_CONFIG_NTP_OFFSET           "ntpOffset"
#define TIMEMAN_CONFIG_NTP_DST_ENABLED      "ntpDSTenabled"
#define TIMEMAN_CONFIG_NTP_DST_START_DATE   "ntpDSTstartDate"
#define TIMEMAN_CONFIG_NTP_DST_END_DATE     "ntpDSTendDate"
#define TIMEMAN_CONFIG_NTP_DST_OFFSET       3600

class TimeManager {
    private:
        UDP& udp;
        NTPClient* ntpClient;
        bool daylightSavingTime;

    public:
        TimeManager(UDP& udp, const char* server, int offset);
        ~TimeManager();
        void begin();
        void setTimeOffset(int offset, bool update);
        void update();
        void setDSToffset(bool DSTactive);
        unsigned long getCurrentTime();
        String getFormattedTtime();

        NTPClient* getNTPClient();
        
};




#endif