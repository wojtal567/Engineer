#include <WiFi.h>

class MyWiFi
{
    private:
        String _ssid;
        String _password;
    public:
        MyWiFi(String ssid, String password);
        bool connect();
        void setSsid(String ssid);
        void setPassword(String password);
        const char* getSsid();
        const char* getPassword();
        wl_status_t getStatus();
};