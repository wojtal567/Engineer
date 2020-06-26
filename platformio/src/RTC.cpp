#include <RTC.hpp>

RTC::RTC(long gmtOffestInSeconds, int daylightOffsetInSeconds, char* ntpServer)
{
    _gmtOffestInSeconds = gmtOffestInSeconds;
    _daylightOffsetInSeconds = daylightOffsetInSeconds;
    _ntpServer = ntpServer;

    configTime(gmtOffestInSeconds, daylightOffsetInSeconds, ntpServer);

    object = new RtcDS1307<TwoWire>(Wire);
    object->Begin();
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char year[5], month[5];
    strftime(year, sizeof(year), "%Y", timeinfo);
    strftime(month, sizeof(month), "%m", timeinfo);
    RtcDateTime dateTime = RtcDateTime(
        atoi(year),
        atoi(month),
        timeinfo->tm_mday,
        timeinfo->tm_hour, 
        timeinfo->tm_min, 
        timeinfo->tm_sec
    );
    object->SetDateTime(dateTime);
}

String RTC::getTimestamp()
{
    RtcDateTime now = object->GetDateTime();
    char buffer[20];
    snprintf_P(
        buffer, 
        20,
        PSTR("%02u.%02u.%04u %02u:%02u:%02u"),
        now.Day(),
        now.Month(),
        now.Year(),
        now.Hour(),
        now.Minute(),
        now.Second() 
    );
    return (String)buffer;
}