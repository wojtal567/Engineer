#include <RtcDS1307.h>
#include <Wire.h>
#include <lvgl.h>
class MyRTC
{
    private:
        RtcDS1307<TwoWire> *object;
        long _gmtOffestInSeconds;
        int _daylightOffsetInSeconds;
        char* _ntpServer;

    public:
        MyRTC(long gmtOffestInSeconds, int daylightOffsetInSeconds, char* ntpServer);
        String getTimestamp();
        void config(bool synchronized);
};
