#ifndef TIMEMANAGER
#define TIMEMANAGER

#include <NTPClient.h>
#include <Udp.h>

#define TIME_MAN_FLAG_CONFIG_CHANGED    (0x1)

class TimeManager {
    private:
        UDP& udp;
        NTPClient* ntpClient;
        unsigned int flags;

    public:
        TimeManager(UDP& udp, const char* server, int offset);
        ~TimeManager();
        void begin();
        void setTimeOffset(int offset, bool update);
        void update();
        void setFlag(unsigned int mask);
        void clearFlag(unsigned int mask);
        unsigned int getFlag();

        NTPClient* getNTPClient();
        
};




#endif