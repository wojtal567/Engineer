#include <MyWiFi.hpp>

MyWiFi::MyWiFi(String ssid, String password)
{
    _ssid = ssid;
    _password = password;
}

bool MyWiFi::connect()
{
    bool connected = false;
    uint8_t attempts = 10;
    WiFi.begin(_ssid.c_str(), _password.c_str());
    while(attempts >= 0 || WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        if(WiFi.status() == WL_CONNECTED) connected = true;
        attempts--;
    }
    return connected;
}

void MyWiFi::setSsid(String ssid)
{
    _ssid = ssid;
}

void MyWiFi::setPassword(String password)
{
    _password = password;
}

const char* MyWiFi::getSsid()
{
    return _ssid.c_str();
}

const char* MyWiFi::getPassword()
{
    return _password.c_str();
}

wl_status_t MyWiFi::getStatus()
{
    return WiFi.status();
}