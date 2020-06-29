#include <RtcDS1307.h>
#include <Wire.h>
class MyRTC
{
    private:
        RtcDS1307<TwoWire> *object;
        long _gmtOffestInSeconds;
        int _daylightOffsetInSeconds;
        char* _ntpServer;

    public:
        RTC(long gmtOffestInSeconds, int daylightOffsetInSeconds, char* ntpServer);
        String getTimestamp();
};