#include <WiFi.h>

class MyWiFi
{
    private:
        String _ssid;
        String _password;
        HardwareSerial *_debugger;
    public:
        MyWiFi(String ssid, String password, HardwareSerial *debugger);
        bool connect();
        void setSsid(String ssid);
        void setPassword(String password);
        const char* getSsid();
        const char* getPassword();
        wl_status_t getStatus();
};