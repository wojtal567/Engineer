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
        String getDate();
        String getTime();
        void config(bool synchronized);
        void setMemory(int address, int value);
};
